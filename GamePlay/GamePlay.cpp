#include "GamePlay.h"

using namespace DirectX;

GamePlay::GamePlay()
{
}

GamePlay::~GamePlay()
{
}

void GamePlay::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);
	FbxObject3d::SetCamera(camera);
	Player::SetCamera(camera);
	Boss::SetCamera(camera);

	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());

	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	if (!Sprite::LoadTexture(TextureNumber::game_bg, L"Resources/Sprite/GameUI/game_bg.png")) {
		assert(0);
		return;
	}

	gameBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	player = Player::Create();
	objSkydome = ObjObject::Create();
	boss = Boss::Create();
	bossPartsRing = BossPartsRing::Create();
	bossCore_1 = BossCore::Create();
	bossCore_2 = BossCore::Create();
	bossCore_3 = BossCore::Create();
	bossCore_4 = BossCore::Create();

	// モデルセット
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	objSkydome->SetModel(modelSkydome);

	// 座標のセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 30, -10 });
	camera->SetUp({ 0, 1, 0 });
	camera->SetDistance(20.0f);

	player->SetPosition({ 0.0f,0.0f,0.0f });
	player->SetRotation({ 0.0f, 0.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	boss->SetPosition({ 0.0f,0.0f,0.0f });
	boss->SetRotation({ 0.0f, 0.0f, 0.0f });
	boss->SetScale({ 1.0f, 1.0f, 1.0f });

	bossPartsRing->SetScale({ 3.0f, 3.0f ,3.0f });
	bossPartsRing->SetParent(boss);

	bossCore_1->SetPosition({ 0.0f , 0.0f, -1.0f });
	bossCore_1->SetScale({0.3f, 0.3f, 0.3f });
	bossCore_1->SetParent(bossPartsRing);

	bossCore_2->SetPosition({ 1.0f , 0.0f, 0.0f });
	bossCore_2->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCore_2->SetParent(bossPartsRing);

	bossCore_3->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_3->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCore_3->SetParent(bossPartsRing);

	bossCore_4->SetPosition({ -1.0f , 0.0f, 0.0f });
	bossCore_4->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCore_4->SetParent(bossPartsRing);

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });
}

void GamePlay::Finalize()
{
}

void GamePlay::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	camera->SetTarget(boss->GetPosition());

	// カメラの更新
	camera->Update();

	// プレイヤーの更新
	player->Update();
	boss->Update();
	bossPartsRing->Update();
	bossCore_1->Update();
	bossCore_2->Update();
	bossCore_3->Update();
	bossCore_4->Update();

	objSkydome->Update();
}

void GamePlay::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	gameBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画

	player->Draw();
	boss->Draw();
	bossPartsRing->Draw();
	bossCore_1->Draw();
	bossCore_2->Draw();
	bossCore_3->Draw();
	bossCore_4->Draw();
	objSkydome->Draw();

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	player->DebugTextDraw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}