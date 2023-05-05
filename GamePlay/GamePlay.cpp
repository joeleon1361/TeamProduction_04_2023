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

	modelPlayer = ObjModel::CreateFromOBJ("bullet");
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelBoss = ObjModel::CreateFromOBJ("bullet2");

	player->SetModel(modelPlayer);
	objSkydome->SetModel(modelSkydome);
	boss->SetModel(modelBoss);

	// 座標のセット
	player->SetPosition({ 0.0f,0.0f, -200.0f });
	player->SetRotation({ 0.0f, 180.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	boss->SetPosition({ 0.0f,0.0f,0.0f });
	boss->SetRotation({ 0.0f, 0.0f, 0.0f });
	boss->SetScale({ 7.0f, 7.0f, 7.0f });

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	camera->SetTarget(boss->GetPosition());
	camera->SetEye({ 0, 0, -10 });
	camera->SetUp({ 0, 1, 0 });
	//camera->SetDistance(48.0f);
	camera->bossPos = boss->GetPosition();
	camera->playerPos = player->GetPosition();
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

	//camera->SetDistance(sqrtf(pow(boss->GetPosition().x - player->GetPosition().x, 2) + pow(boss->GetPosition().y - player->GetPosition().y, 2) + pow(boss->GetPosition().z - player->GetPosition().z, 2)) + 48.0f);
	camera->bossPos = boss->GetPosition();
	camera->playerPos = player->GetPosition();
	camera->playerRot = player->GetRotation();

	// カメラの更新
	camera->Update();

	// プレイヤーの更新
	player->Update();
	boss->Update();

	if (player->CheckCollisionWithBoss(boss->GetPosition(), 20.0f))
	{
		float dx = player->GetPosition().x - boss->GetPosition().x;
		float dy = player->GetPosition().y - boss->GetPosition().y;
		float dz = player->GetPosition().z - boss->GetPosition().z;

		float length = sqrtf(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

		if (length > 0.0f)
		{
			dx /= length;
			dy /= length;
			dz /= length;
		}

		float newX = player->GetPosition().x;
		float newY = player->GetPosition().y;
		float newZ = player->GetPosition().z;

		float elapsedTime = 0.0f;

		while (player->CheckCollisionWithBoss(boss->GetPosition(), 20.0f))
		{
			player->MoveTowards(newX, player->GetPosition().x + dx, 1.0f, elapsedTime);
			player->MoveTowards(newY, player->GetPosition().y + dy, 1.0f, elapsedTime);
			player->MoveTowards(newZ, player->GetPosition().z + dz, 1.0f, elapsedTime);
			player->SetPosition({ newX, newY, newZ });
			elapsedTime += 0.1f;
		}
	}

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