﻿#include "FbxLoader.h"
#include <cassert>

using namespace DirectX;

// 静的メンバ変数の実体
const std::string FbxLoader::baseDirectory = "Resources/FbxModel/";

const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

// 行列の変換(定義)
void FbxLoader::ConvertMatrixFromFbx( DirectX::XMMATRIX *dst, const FbxAMatrix &src )
{
    // 行
    for ( int i = 0; i < 4; i++ )
    {
        // 列
        for ( int j = 0; j < 4; j++ )
        {
            // 1要素コピー
            dst->r[i].m128_f32[j] = (float)src.Get( i, j );
        }
    }
}

// FBX SDK の初期化
void FbxLoader::Initialize( ID3D12Device *device )
{
    // 再初期化チェック
    assert( fbxManager == nullptr );

    // 引数からメンバ変数に代入
    this->device = device;

    // FBXマネージャーの生成
    fbxManager = FbxManager::Create();

    // FBXマネージャーの入出力設定
    FbxIOSettings *ios = FbxIOSettings::Create( fbxManager, IOSROOT );
    fbxManager->SetIOSettings( ios );

    // FBXインポータの生成
    fbxImporter = FbxImporter::Create( fbxManager, "" );
}

// FBX SDK の後始末
void FbxLoader:: Finalize()
{
    // 各種FBXインスタンスの破棄
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

// FBX ファイルの読み込み
FbxModel* FbxLoader::LoadModelFromFile( const string &modelName )
{
#pragma region FBX ファイルの読み込み
    // モデルと同じ名前のフォルダから読み込む
    const string directoryPath = baseDirectory + modelName + "/";

    // 拡張子.fbxを付加
    const string fileName = modelName + ".fbx";

    // 連結してフルパスを得る
    const string fullpath = directoryPath + fileName;

    // ファイル名を指定してFBXファイルを読み込む
    if ( !fbxImporter->Initialize( fullpath.c_str(), -1, fbxManager->GetIOSettings() ) )
    {
        assert( 0 );
    }
#pragma endregion

#pragma region シーンのインポート
    // シーン生成
    FbxScene *fbxScene = FbxScene::Create( fbxManager, "fbxScene" );

    // ファイルからロードしたFBXの情報をシーンにインポート
    fbxImporter->Import( fbxScene );
#pragma endregion

    // モデル生成
    FbxModel *fbxmodel = new FbxModel();
    fbxmodel->name = modelName;

    // FBXノードの数を取得
    int nodeCount = fbxScene->GetNodeCount();

    // あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防
    fbxmodel->nodes.reserve( nodeCount );

    // ルートノードから順に解析してモデルに流し込む
    ParseNodeRecursive( fbxmodel, fbxScene->GetRootNode() );

    // FBXシーン解放
    // fbxScene->Destroy();
    fbxmodel->fbxScene = fbxScene;

    // バッファ生成
    fbxmodel->CreatBuffers( device );

    return fbxmodel;
}

// 解析関数(再帰)
void FbxLoader::ParseNodeRecursive( FbxModel *fbxmodel, FbxNode *fbxNode, Node* parent )
{
    // モデルにノードを追加
    fbxmodel->nodes.emplace_back();
    Node &node = fbxmodel->nodes.back();

    // ノード名を取得
    node.name = fbxNode->GetName();

    // FBXノードのローカル移動情報
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    // 形式変換して代入
    node.rotation = { (float)rotation[0], (float)rotation[1],(float)rotation[2],0.0f };
    node.scaling = { (float)scaling[0], (float)scaling[1],(float)scaling[2],0.0f };
    node.translation = { (float)translation[0], (float)translation[1],(float)translation[2],1.0f };

    // スケール、回転、平行移動行列の計算
    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector( node.scaling );
    matRotation = XMMatrixRotationRollPitchYawFromVector( node.rotation );
    matTranslation = XMMatrixTranslationFromVector( node.translation );

    // ローカル変形行列
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling;
    node.transform *= matRotation;
    node.transform *= matTranslation;

    // グローバル変形行列の計算
    node.globalTransform = node.transform;
    if ( parent )
    {
        node.parent = parent;

        // 親の変形を乗算
        node.globalTransform *= parent->globalTransform;
    }

    // FBXノードのメッシュ情報を解析
    FbxNodeAttribute *fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if ( fbxNodeAttribute )
    {
        if ( fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh )
        {
            fbxmodel->meshNode = &node;
            ParseMesh( fbxmodel, fbxNode );
        }
    }

    // 子ノードに対して再帰呼び出し
    for ( int i = 0; i < fbxNode->GetChildCount(); i++ )
    {
        ParseNodeRecursive( fbxmodel, fbxNode->GetChild( i ), &node );
    }
}

// メッシュ解析関数
void FbxLoader::ParseMesh( FbxModel *fbxmodel, FbxNode *fbxNode )
{
    // ノードのメッシュを取得
    FbxMesh *fbxMesh = fbxNode->GetMesh();

    // 頂点座標読み取り
    ParseMeshVertices( fbxmodel, fbxMesh );

    // 面を構成するデータの読み取り
    ParseMeshFaces( fbxmodel, fbxMesh );

    // マテリアルの読み取り
    ParseMaterial( fbxmodel, fbxNode );

    // スキニング情報読み取り
    ParseSkin( fbxmodel, fbxMesh );
}

#pragma region メッシュ解析用の関数(サブ関数)
// 頂点座標読み取り
void FbxLoader::ParseMeshVertices( FbxModel *fbxmodel, FbxMesh *fbxMesh )
{
    auto &vertices = fbxmodel->vertices;

    // 頂点座標データの数
    const int controlPointsCount = fbxMesh->GetControlPointsCount();

    // 必要数だけ頂点データ配列を確保
    FbxModel::VertexPosNormalUvSkin vert{};
    fbxmodel->vertices.resize( controlPointsCount, vert );

    // FBXメッシュの頂点座標配列を取得
    FbxVector4 *pCoord = fbxMesh->GetControlPoints();

    // FBXメッシュの全頂点座標をモデル内の配列にコピーする
    for ( int i = 0; i < controlPointsCount; i++ )
    {
        FbxModel::VertexPosNormalUvSkin &vertex = vertices[i];

        // 座標のコピー
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }
}

// 面情報読み取り
void FbxLoader::ParseMeshFaces( FbxModel *fbxmodel, FbxMesh *fbxMesh )
{
    auto &vertices = fbxmodel->vertices;
    auto &indices = fbxmodel->indices;

    // 1ファイルに複数メッシュのモデルは非対応
    assert( indices.size() == 0 );

    // 面の数
    const int polygonCount = fbxMesh->GetPolygonCount();
    // UVデータの数
    const int textureUVCount = fbxMesh->GetTextureUVCount();
    // UV名リスト
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames( uvNames );

    // 面ごとの情報読み取り
    for ( int i = 0; i < polygonCount; i++ )
    {
        // 面を構成する頂点の数を取得(3なら三角形ポリゴン)
        const int polygonSize = fbxMesh->GetPolygonSize( i );
        assert( polygonSize <= 4 );

        // 1頂点ずつ処理
        for ( int j = 0; j < polygonSize; j++ )
        {
            // FBX頂点配列のインデックス
            int index = fbxMesh->GetPolygonVertex( i, j );
            assert( index >= 0 );

#pragma region 法線情報読み取り
            // 頂点法線読み込み
            FbxModel::VertexPosNormalUvSkin &vertex = vertices[index];
            FbxVector4 normal;
            if ( fbxMesh->GetPolygonVertexNormal( i, j, normal ) )
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }
#pragma endregion

#pragma region UV情報読み取り
            // テクスチャUV読み込み
            if ( textureUVCount > 0 )
            {
                FbxVector2 uvs;
                bool lUnmappedUV;

                // 0番決め打ちで読み込み
                if ( fbxMesh->GetPolygonVertexUV( i, j, uvNames[0], uvs, lUnmappedUV ) )
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1];
                }
            }
#pragma endregion

#pragma region 頂点インデックス追加
            // インデックス配列に頂点インデックス追加
            // 3頂点までなら
            if ( j < 3 )
            {
                // 1点追加し、他の2点と三角形を構築する
                indices.push_back( index );
            }
            // 4頂点目
            else
            {
                // 3点追加し、
                // 四角形の0,1,2,3の内 2,3,0で三角形を構築する
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back( index2 );
                indices.push_back( index3 );
                indices.push_back( index0 );
            }
#pragma endregion
        }
    }
}

// マテリアル読み取り
void FbxLoader::ParseMaterial( FbxModel *fbxmodel, FbxNode *fbxNode )
{
    const int materialCount = fbxNode->GetMaterialCount();
    if ( materialCount > 0 )
    {
        // 先頭のマテリアルを取得
        FbxSurfaceMaterial *material = fbxNode->GetMaterial( 0 );
        // テクスチャを読み込んだかどうかを表すフラグ
        bool textureLoaded = false;

        if ( material )
        {
            // FbxSurfaceLambertクラスかどうかを調べる
            if ( material->GetClassId().Is( FbxSurfaceLambert::ClassId ) )
            {
                FbxSurfaceLambert *lambert = static_cast<FbxSurfaceLambert *>(material);

                // 環境光係数
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                fbxmodel->ambient.x = (float)ambient.Get()[0];
                fbxmodel->ambient.y = (float)ambient.Get()[1];
                fbxmodel->ambient.z = (float)ambient.Get()[2];

                // 拡散反射光係数
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                fbxmodel->diffuse.x = (float)diffuse.Get()[0];
                fbxmodel->diffuse.y = (float)diffuse.Get()[1];
                fbxmodel->diffuse.z = (float)diffuse.Get()[2];
            }

            // ディフューズテクスチャを取り出す
            const FbxProperty diffuseProperty = material->FindProperty( FbxSurfaceMaterial::sDiffuse );
            if ( diffuseProperty.IsValid() )
            {
                const FbxFileTexture *texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if ( texture )
                {
                    const char *filepath = texture->GetFileName();

                    // ファイルパスからファイル名抽出
                    string path_str( filepath );
                    string name = ExtractFileName( path_str );

                    // テクスチャ読み込み
                    LoadTexture( fbxmodel, baseDirectory + fbxmodel->name + "/" + name );
                    textureLoaded = true;
                }
            }

        }

        // テクスチャがない場合は白テクスチャを貼る
        if ( !textureLoaded )
        {
            LoadTexture( fbxmodel, baseDirectory + defaultTextureFileName );
        }
    }
}

// テクスチャの読み込み
void FbxLoader::LoadTexture(FbxModel* fbxmodel, const std::string& fullpath)
{
    HRESULT result = S_FALSE;

    // WICテクスチャのロード
    TexMetadata& metadata = fbxmodel->metadata;
    ScratchImage& scratchImg = fbxmodel->scratchImg;

    // ユニコード文字列に変換
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(
        wfilepath, WIC_FLAGS_NONE,
        &metadata, scratchImg);
    if (FAILED(result))
    {
        assert(0);
    }
}
#pragma endregion

// スキニング情報読み取り
void FbxLoader::ParseSkin( FbxModel *fbxmodel, FbxMesh *fbxMesh )
{
#pragma region スキニング情報読み取り
    // スキニング情報
    FbxSkin *fbxSkin =
        static_cast<FbxSkin *>(fbxMesh->GetDeformer( 0,
            FbxDeformer::eSkin ));
    // スキニング情報がなければ終了
    if ( fbxSkin == nullptr )
    {
        for ( int i = 0; i < fbxmodel->vertices.size(); i++ )
        {
            fbxmodel->vertices[i].boneIndex[0] = 0;
            fbxmodel->vertices[i].boneWeight[0] = 1.0f;
        }

        return;
    }

    // ボーン配列の参照
    std::vector<FbxModel::Bone> &bones = fbxmodel->bones;

    // ボーンの数
    int clusterCount = fbxSkin->GetClusterCount();
    bones.reserve( clusterCount );
#pragma endregion

#pragma region 初期姿勢行列読み取り
    // 全てのボーンについて
    for ( int i = 0; i < clusterCount; i++ )
    {
        // FBXボーン情報
        FbxCluster *fbxCluster = fbxSkin->GetCluster( i );

        // ボーン自体のノードの名前を取得
        const char *boneName = fbxCluster->GetLink()->GetName();

        // 新しくボーンを追加し、追加したボーンの参照を得る
        bones.emplace_back( FbxModel::Bone( boneName ) );
        FbxModel::Bone &bone = bones.back();
        // 自作ボーンと紐づける
        bone.fbxCluster = fbxCluster;

        // FBXから初期姿勢行列を所得する
        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix( fbxMat );

        // XMMatrix型に変換する
        XMMATRIX initialPose;
        ConvertMatrixFromFbx( &initialPose, fbxMat );

        // 初期姿勢行列の逆行列を得る
        bone.invInitialPose = XMMatrixInverse( nullptr, initialPose );
    }
#pragma endregion

#pragma region スキンウェイトの読み取り
    // ボーン番号とスキンウェイトのペア
    struct WeightSet
    {
        UINT index;
        float weight;
    };

    // 二次元配列
    std::vector<std::list<WeightSet>>
        weightLists( fbxmodel->vertices.size() );

    // 全てのボーンについて
    for ( int i = 0; i < clusterCount; i++ )
    {
        // FBXボーン情報
        FbxCluster *fbxCluster = fbxSkin->GetCluster( i );
        // このボーンに影響を与える頂点の数
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        // このボーンに影響を与える頂点の配列
        int *controlPointIndices = fbxCluster->GetControlPointIndices();
        double *controlPointWeights = fbxCluster->GetControlPointWeights();

        // 影響を受ける頂点について
        for ( int j = 0; j < controlPointIndicesCount; j++ )
        {
            // 頂点番号
            int vertIndex = controlPointIndices[j];
            // スキンウェイト
            float weight = (float)controlPointWeights[j];
            // その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
            weightLists[vertIndex].emplace_back( WeightSet{ (UINT)i, weight } );
        }
    }
#pragma endregion

#pragma region スキンウェイトの整理
    // 頂点配列書き換え用の参照
    auto &vertices = fbxmodel->vertices;
    // 各頂点について処理
    for ( int i = 0; i < vertices.size(); i++ )
    {
        // 頂点のウェイトから最も大きい4つを選択
        auto &weightList = weightLists[i];
        // 大小比較用のラムダ式を指定して降順にソート
        weightList.sort(
            []( auto const &lhs, auto const &rhs )
            {
                return lhs.weight > rhs.weight;
            }
        );

        int weightArrayIndex = 0;
        // 降順ソート済みのウェイトリストから
        for ( auto &WeightSet : weightList )
        {
            // 頂点データに書き込み
            vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;
            // 4つに達したら終了
            if ( ++weightArrayIndex >= FbxModel::MAX_BONE_INDICES )
            {
                float weight = 0.0f;
                // 2番目以降のウェイトを合計
                for ( int j = 1; j < FbxModel::MAX_BONE_INDICES; j++ )
                {
                    weight += vertices[i].boneWeight[j];
                }
                // 合計で1.0f(100%)になるように調整
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
#pragma endregion
}

// ファイル名抽出(定義)
std::string FbxLoader::ExtractFileName( const std::string &path )
{
    size_t pos1;

    // 区切り文字 '\\' が出てくる一番最後の部分を検索
    pos1 = path.rfind( '\\' );
    if ( pos1 != string::npos )
    {
        return path.substr( pos1 + 1, path.size() - pos1 - 1 );
    }

    // 区切り文字 '/' が出てくる一番最後の部分を検索
    pos1 = path.rfind( '/' );
    if ( pos1 != string::npos )
    {
        return path.substr( pos1 + 1, path.size() - pos1 - 1 );
    }

    return path;
}
