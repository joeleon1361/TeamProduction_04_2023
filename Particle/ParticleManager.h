﻿#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

// パーティクルマネージャ
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
		XMFLOAT4 color;
		float rotation;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ビュープロジェクション行列
		XMMATRIX matBillboard;	// ビルボード行列
	};

	// パーティクル1粒
	class Particle
	{
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
	public:
		// 座標
		XMFLOAT3 position = {};
		// 速度
		XMFLOAT3 velocity = {};
		// 加速度
		XMFLOAT3 accel = {};
		// 色
		XMFLOAT4 color = {};
		// スケール
		float scale = 1.0f;
		// 回転
		float rotation = 0.0f;
		// 初期値
		XMFLOAT4 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// 最終値
		XMFLOAT4 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// 現在フレーム
		int frame = 0;
		// 終了フレーム
		int num_frame = 0;
	};

private: // 定数
	static const int vertexCount = 65536;		// 頂点数

public: // 静的メンバ関数
	// インスタンス生成
	static ParticleManager* Create(ID3D12Device* device, Camera* camera,int blensType, const wchar_t* filename);

public: // メンバ関数	
	// コンストラクタ
	ParticleManager(ID3D12Device* device, Camera* camera);

	// 初期化
	void Initialize(int blendType, const wchar_t* filename );

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// パーティクルの追加
	void Add(int life, const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel, const XMFLOAT4& start_color, const XMFLOAT4& end_color, float start_scale, float end_scale, float start_rotation, float end_rotation);

	// デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	// グラフィックパイプライン生成
	void InitializeGraphicsPipeline(int blendType);

	// テクスチャ読み込み
	void LoadTexture(const wchar_t* filename);

	// モデル作成
	void CreateModel();

	//---パーティクル関数---//
	//基本パーティクル
	void DefaultParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	 
	//ボス部位破壊パーティクル
	void BlastPart_1(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void BlastPart_2(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void BlastPart_3(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void BlastPart_4(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//プレイヤージェットパーティクル
	void JettParticle(int PartNum, int Life, XMFLOAT3 position, XMFLOAT3 velosity,  int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//ブーストパーティクル
	void BoostParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//火花パーティクル
	void SparkParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	// 弾パーティクル生成
	void BulletParticle(int PartNum, int Life, XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale);

private: // メンバ変数
	// デバイス
	ID3D12Device* device = nullptr;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize = 0;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// パーティクル配列
	std::forward_list<Particle> particles;
	// カメラ
	Camera* camera = nullptr;

	enum blendType
	{
		translucent,
		add,
		sub
	};

	//重力加速度
	float Gravity = 9.8f;

	//Y軸の移動量
	float VY = 0.0f;

	//経過時間
	int elapsedTime = 0;

};

