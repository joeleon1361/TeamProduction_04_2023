#include "GamePlay.h"

using namespace DirectX;
extern HWND hwnd;

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

	//レティクルテクスチャ
	if (!Sprite::LoadTexture(TextureNumber::reticle, L"Resources/Re.png")) {
		assert(0);
		return;
	}

	// ボス
	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_1, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_2, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_gauge, L"Resources/Sprite/GameUI/BossHpUI/game_boss_gauge.png")) {
		assert(0);
		return;
	}

	//黒背景
	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	//黒背景
	if (!Sprite::LoadTexture(TextureNumber::rule, L"Resources/Sprite/TitleUI/Rule.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト初期化
	debugText.Initialize(0);

	// スプライト
	gameBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	Reticle = Sprite::Create(TextureNumber::reticle, ReticlePos);

	test = Sprite::Create(TextureNumber::reticle, { (float)mousePosition.x, (float)mousePosition.y });

	// ボス
	bossHpUI = Sprite::Create(TextureNumber::game_boss_frame_1, { bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });
	bossHpGage = Sprite::Create(TextureNumber::game_boss_gauge, bossHpUIPosition);
	bossDamageGage = Sprite::Create(TextureNumber::game_boss_gauge, bossHpUIPosition);
	bossHpUICover = Sprite::Create(TextureNumber::game_boss_frame_2, { bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });

	// ブーストゲージ
	boostUI = Sprite::Create(TextureNumber::game_boss_frame_1, { boostUIPosition.x + 10.0f, boostUIPosition.y });
	boostGage = Sprite::Create(TextureNumber::game_boss_gauge, boostUIPosition);
	boostUICover = Sprite::Create(TextureNumber::game_boss_frame_2, { boostUIPosition.x + 10.0f, boostUIPosition.y });

	Black = Sprite::Create(TextureNumber::black, {0.0f, 0.0f});

	BlackAlpha = 1.0f;
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	Rule = Sprite::Create(TextureNumber::rule, { 0.0f, 0.0f });

	// パーティクル
	circleParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

	// プレイヤー
	player = Player::Create();

	// スカイドーム
	objSkydome = ObjObject::Create();

	// ボス
	boss = Boss::Create();
	bossPartsRing = BossParts::Create();
	bossPartsSphere = BossParts::Create();
	bossCore_1 = BossCore::Create();
	bossCore_2 = BossCore::Create();
	bossCore_3 = BossCore::Create();
	bossCore_4 = BossCore::Create();
	bossCoreBox_1 = BossParts::Create();
	bossCoreBox_2 = BossParts::Create();
	bossCoreBox_3 = BossParts::Create();
	bossCoreBox_4 = BossParts::Create();
	bossTurretStand_1 = BossParts::Create();
	//bossTurretStand_2 = BossParts::Create();
	bossTurret_1 = BossTurret::Create();
	//bossTurret_2 = BossTurret::Create();
	bossPartsCoreStand = BossParts::Create();
	bossMainCore = BossMainCore::Create();

	// モデルセット
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	objSkydome->SetModel(modelSkydome);

	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	modelBossPartsCoreBox = ObjModel::CreateFromOBJ("bossPartsCoreBox");
	bossCoreBox_1->SetModel(modelBossPartsCoreBox);
	bossCoreBox_2->SetModel(modelBossPartsCoreBox);
	bossCoreBox_3->SetModel(modelBossPartsCoreBox);
	bossCoreBox_4->SetModel(modelBossPartsCoreBox);

	modelBossPartsRing = ObjModel::CreateFromOBJ("bossPartsRing");
	bossPartsRing->SetModel(modelBossPartsRing);

	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	bossPartsSphere->SetModel(modelBossPartsSphere);

	modelBossPartsTurretStand = ObjModel::CreateFromOBJ("TurretStand");
	bossTurretStand_1->SetModel(modelBossPartsTurretStand);
	//bossTurretStand_2->SetModel(modelBossPartsTurretStand);

	modelBossPartsCoreStand = ObjModel::CreateFromOBJ("bossPartsCoreStand");
	bossPartsCoreStand->SetModel(modelBossPartsCoreStand);

	// ボスのHPゲージ
	bossHpGage->SetColor({ 0.1f, 0.6f, 0.1f, 1.0f });
	bossHpGage->SetSize({ 530.0f, 30.0f });
	bossHpGage->SetAnchorPoint({ 1.0f, 0.5f });

	bossDamageGage->SetColor({ 1.0f, 0, 0.2f, 1.0f });
	bossDamageGage->SetSize({ 530.0f, 30.0f });
	bossDamageGage->SetAnchorPoint({ 1.0f, 0.5f });

	bossHpUI->SetAnchorPoint({ 1.0f, 0.5f });

	bossHpUICover->SetAnchorPoint({ 1.0f, 0.5f });

	// ブーストゲージ
	boostGage->SetColor({ 0.6f, 0.6f, 0.1f, 1.0f });
	boostGage->SetSize({ 530.0f, 30.0f });
	boostGage->SetAnchorPoint({ 1.0f, 0.5f });

	boostUI->SetAnchorPoint({ 1.0f, 0.5f });

	boostUICover->SetAnchorPoint({ 1.0f, 0.5f });

	// 座標のセット
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 30, -10 });
	camera->SetUp({ 0, 1, 0 });
	camera->SetDistance(20.0f);

	// プレイヤー
	player->SetPosition({ 0.0f, 0.0f, -200.0f });
	player->SetRotation({ 0.0f, 180.0f, 0.0f });
	player->SetScale({ 2.0f, 2.0f, 2.0f });

	// ボスのベース
	boss->SetPosition({ 0.0f,0.0f,0.0f });
	boss->SetRotation({ 0.0f, 0.0f, 0.0f });
	boss->SetScale({ 7.0f, 7.0f, 7.0f });

	// ボスのリングパーツ
	bossPartsRing->SetScale({ 4.0f, 4.0f, 4.0f });
	bossPartsRing->SetParent(boss);

	// ボスの球パーツ
	bossPartsSphere->SetScale({ 4.2f, 4.2f, 4.2f });
	bossPartsSphere->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	bossPartsSphere->SetParent(boss);

	bossPartsCoreStand->SetPosition({ 0.0f, -3.5f, 0.0f });
	bossPartsCoreStand->SetRotation({ 180.0f, 0.0f, 0.0f });
	bossPartsCoreStand->SetScale({ 2.0f, 2.0f, 2.0f });
	bossPartsCoreStand->SetColor({ 0.647f, 0.619f, 0.658f, 1.0f });
	bossPartsCoreStand->SetParent(boss);

	// ボスのコアボックス1
	bossCoreBox_1->SetPosition({ 0.0f , 0.0f, -1.2f });
	bossCoreBox_1->SetRotation({ 0.0f, 180.0f, 0.0f });
	bossCoreBox_1->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_1->SetParent(bossPartsRing);

	// ボスのコアボックス2
	bossCoreBox_2->SetPosition({ 1.2f , 0.0f, 0.0f });
	bossCoreBox_2->SetRotation({ 0.0f, 90.0f, 0.0f });
	bossCoreBox_2->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_2->SetParent(bossPartsRing);

	// ボスのコアボックス3
	bossCoreBox_3->SetPosition({ 0.0f , 0.0f, 1.2f });
	bossCoreBox_3->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_3->SetParent(bossPartsRing);

	// ボスのコアボックス4
	bossCoreBox_4->SetPosition({ -1.2f , 0.0f, 0.0f });
	bossCoreBox_4->SetRotation({ 0.0f, 270.0f, 0.0f });
	bossCoreBox_4->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_4->SetParent(bossPartsRing);

	// ボスのコア1
	bossCore_1->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_1->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_1->SetParent(bossCoreBox_1);

	// ボスのコア2
	bossCore_2->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_2->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_2->SetParent(bossCoreBox_2);

	// ボスのコア3
	bossCore_3->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_3->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_3->SetParent(bossCoreBox_3);

	// ボスのコア4
	bossCore_4->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_4->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_4->SetParent(bossCoreBox_4);

	// ボスのメインコア
	bossMainCore->SetPosition({0.0f, 0.0f, 0.0f});
	bossMainCore->SetScale({ 0.75f, 0.75f, 0.75f });
	bossMainCore->SetParent(bossPartsCoreStand);

	// ボスの砲台1
	bossTurretStand_1->SetPosition({ 0.0f, 0.9f, 0.0f });
	bossTurretStand_1->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_1->SetParent({ bossPartsSphere });

	bossTurret_1->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_1->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_1->SetParent({ bossTurretStand_1 });

	// ボスの砲台2
	/*bossTurretStand_2->SetPosition({ 0.0f, -0.9f, 0.0f });
	bossTurretStand_2->SetRotation({ 0.0f, 0.0f, 180.0f });
	bossTurretStand_2->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_2->SetParent({ bossPartsSphere });

	bossTurret_2->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_2->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_2->SetParent({ bossTurretStand_2 });*/

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	camera->SetTarget(boss->GetPosition());
	camera->SetEye({ 0, 0, -10 });
	camera->SetUp({ 0, 1, 0 });
	//camera->SetDistance(48.0f);
	camera->bossPos = boss->GetPosition();
	camera->playerPos = player->GetPosition();

	ShowCursor(false);
}

void GamePlay::Finalize()
{
}

void GamePlay::Update()
{
	BlackAlpha -= 0.02f;
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	//RECT構造体へのポインタ
	RECT rect;

	//ウィンドウの外側のサイズを取得
	GetClientRect(hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//マウスの(スクリーン)座標を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入
	ReticlePos.x = ((float)(mousePosition.x) / (float)width) * WinApp::window_width;
	ReticlePos.y = ((float)(mousePosition.y) / (float)height) * WinApp::window_height;

	if (input->TriggerKey(DIK_C))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	bossHpGageSize = bossHpGage->GetSize();
	bossDamageGageSize = Lerp::LerpFloat2(bossDamageGage->GetSize(), bossHpGageSize, 0.1f);
	
		//circleParticle->SparkParticle(20, 50, bossCore_1->GetWorldPosition(), 10.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
	

	boostGageSize = boostGage->GetSize();

	// コアヒットエフェクト
	CoreHitEffect();

	// ボスパーツヒットエフェクト
	BossPartsHitEffect();

	// プレイヤーの球発射処理
	Shoot();

	// プレイヤーの狙い弾を更新
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		circleParticle->BulletParticle(5, 10, bullet->GetPosition(), { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 5.0f);
		bullet->Update();
	}

	// プレイヤーの狙い弾を消去
	playerBullets.remove_if([](std::unique_ptr<TargetBullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, player->GetPosition(), 3.0f))
		{
			bullet->deathFlag = true;
		}
	}

	// ボスの狙い弾を更新
	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		circleParticle->BulletParticle(5, 10, bullet->GetPosition(), { 1.0f,0.1f, 0.1f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 7.0f);
		bullet->Update();
	}

	// ボスの狙い弾を消去
	bossTargetBullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// コア撃破エフェクト
	CoreBreakEffect();

	// 全てのコアを破壊した後
	CoreAllBreak();

	// ボスのHPゲージ
	// ボスのHP計算
	bossMainCore->lifeRatio = bossMainCore->life / bossMainCore->lifeMax;
	bossHpGageSize.x = bossMainCore->lifeRatio * 530.0f;

	bossHpGage->SetPosition(bossHpUIPosition);
	bossHpGage->SetSize(bossHpGageSize);

	bossDamageGage->SetPosition(bossHpUIPosition);
	bossDamageGage->SetSize(bossDamageGageSize);

	bossHpUI->SetPosition({ bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });

	bossHpUICover->SetPosition({ bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });

	// ブーストゲージ
	float ratio;
	ratio = player->GetBoostPow() / 100.0f;
	boostGageSize.x = ratio * 530.0f;

	boostGage->SetPosition(boostUIPosition);
	boostGage->SetSize(boostGageSize);

	boostUI->SetPosition({ boostUIPosition.x + 10.0f, boostUIPosition.y });

	boostUICover->SetPosition({ boostUIPosition.x + 10.0f, boostUIPosition.y });

	// カメラターゲットのセット
	// camera->SetTarget(boss->GetPosition());
	// camera->SetDistance(sqrtf(pow(boss->GetPosition().x - player->GetPosition().x, 2) + pow(boss->GetPosition().y - player->GetPosition().y, 2) + pow(boss->GetPosition().z - player->GetPosition().z, 2)) + 48.0f);
	camera->bossPos = boss->GetPosition();
	camera->playerPos = player->GetPosition();
	camera->playerRot = player->GetRotation();

	// カメラの更新
	camera->Update();

	// プレイヤーの更新
	player->Update();
	boss->Update();
	bossPartsRing->Update();
	bossPartsSphere->Update();
	bossCoreBox_1->Update();
	bossCoreBox_2->Update();
	bossCoreBox_3->Update();
	bossCoreBox_4->Update();
	bossCore_1->Update();
	bossCore_2->Update();
	bossCore_3->Update();
	bossCore_4->Update();
	bossTurret_1->Update();
	//bossTurret_2->Update();
	bossTurretStand_1->Update();
	//bossTurretStand_2->Update();
	bossPartsCoreStand->Update();
	bossMainCore->Update();

	PlayerMovementBoundaryChecking();

	objSkydome->Update();
	Reticle->SetAnchorPoint({ 0.5f, 0.5f });
	Reticle->SetPosition(ReticlePos);

	test->SetPosition({ (float)mousePosition.x, (float)mousePosition.y });

	//DrawDebugText();

	circleParticle->JettParticle(5, 10, player->GetPosition(), player->GetVel(), 1.0f, 0.0f, { 0.941f, 0.231f, 0.156f, 1.0f }, { 0.941f, 0.862f, 0.156f, 1.0f });

	if (player->GetBoostFlag() == true)
	{
		circleParticle->BoostParticle(5, 50, player->GetPosition(), 1.0f, 0.0f, player->GetBoostPartColor(), player->GetBoostPartColor());
	}
	// パーティクル更新
	circleParticle->Update();

	// HPが0になったら撃破
	if (bossMainCore->life <= 0.0f)
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}
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

	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		//bullet->Draw();
	}

	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		//bullet->Draw();
	}

	bossPartsRing->Draw();
	bossPartsSphere->Draw();
	bossCoreBox_1->Draw();
	bossCoreBox_2->Draw();
	bossCoreBox_3->Draw();
	bossCoreBox_4->Draw();
	bossTurret_1->Draw();
	//bossTurret_2->Draw();
	bossTurretStand_1->Draw();
	//bossTurretStand_2->Draw();
	bossPartsCoreStand->Draw();

	bossCore_1->Draw();
	bossCore_2->Draw();
	bossCore_3->Draw();
	bossCore_4->Draw();

	bossMainCore->Draw();

#pragma region 部位破壊処理を一時コメントアウト
	/*if (bossCore_1->isAlive || bossCore_2->isAlive || bossCore_3->isAlive || bossCore_4->isAlive)
	{
		bossPartsRing->Draw();
		bossPartsSphere->Draw();
		bossCoreBox_1->Draw();
		bossCoreBox_2->Draw();
		bossCoreBox_3->Draw();
		bossCoreBox_4->Draw();
		bossTurret_1->Draw();
		bossTurret_2->Draw();
		bossTurretStand_1->Draw();
		bossTurretStand_2->Draw();
	}

	if (bossCore_1->isAlive)
	{
		bossCore_1->Draw();
	}

	if (bossCore_2->isAlive)
	{
		bossCore_2->Draw();
	}

	if (bossCore_3->isAlive)
	{
		bossCore_3->Draw();
	}

	if (bossCore_4->isAlive)
	{
		bossCore_4->Draw();
	}*/
#pragma endregion

	objSkydome->Draw();

	// パーティクルの描画
	circleParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	
	Reticle->Draw();

	bossHpUI->Draw();
	bossDamageGage->Draw();
	bossHpGage->Draw();
	bossHpUICover->Draw();

	boostUI->Draw();
	boostGage->Draw();
	boostUICover->Draw();

	//player->DebugTextDraw();
	debugText.DrawAll(cmdList);
	Rule->Draw();
	Black->Draw();


	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

// マウスの処理
void GamePlay::GetMouse()
{

	//マウスの(スクリーン)座標を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInst()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入
	ReticlePos.x = (float)mousePosition.x;
	ReticlePos.y = (float)mousePosition.y;
}

// デバックテキスト
void GamePlay::DrawDebugText()
{
	//マウスの座標
	std::ostringstream MousePosition;
	MousePosition << "MousePosition("
		<< std::fixed << std::setprecision(5)
		<< mousePosition.x << ","
		<< mousePosition.y << ")";

	debugText.Print(MousePosition.str(), 0, 0, 1.0f);

	//レティクルの座標
	std::ostringstream ReticlePosition;
	ReticlePosition << "ReticlePosition("
		<< std::fixed << std::setprecision(5)
		<< ReticlePos.x << ","
		<< ReticlePos.y << ")";

	debugText.Print(ReticlePosition.str(), 0, 60, 1.0f);

	std::ostringstream CoreLife_1;
	CoreLife_1 << "CoreLife_1:("
		<< std::fixed << std::setprecision(2)
		<< bossCore_1->life << ")"; // z
	debugText.Print(CoreLife_1.str(), 10, 210, 1.0f);

	std::ostringstream CoreLife_2;
	CoreLife_2 << "CoreLife_2:("
		<< std::fixed << std::setprecision(2)
		<< bossCore_2->life << ")"; // z
	debugText.Print(CoreLife_2.str(), 10, 230, 1.0f);

	std::ostringstream CoreLife_3;
	CoreLife_3 << "CoreLife_3:("
		<< std::fixed << std::setprecision(2)
		<< bossCore_3->life << ")"; // z
	debugText.Print(CoreLife_3.str(), 10, 250, 1.0f);

	std::ostringstream CoreLife_4;
	CoreLife_4 << "CoreLife_4:("
		<< std::fixed << std::setprecision(2)
		<< bossCore_4->life << ")"; // z
	debugText.Print(CoreLife_4.str(), 10, 270, 1.0f);

	std::ostringstream MainCoreLife;
	MainCoreLife << "MainCoreLife:("
		<< std::fixed << std::setprecision(2)
		<< bossMainCore->life << ")"; // z
	debugText.Print(MainCoreLife.str(), 10, 290, 1.0f);
}

// プレイヤー弾発射
void GamePlay::Shoot()
{
	shotRate -= 0.1f;

	XMVECTOR bulletVelocity = { 0,0,1.0f };

	if (Input::GetInstance()->PushKey(DIK_SPACE) || Input::GetInstance()->PushMouseLeft())
	{
		if (shotRate <= 0)
		{
			shotFlag = true;
		}

		if (shotFlag == true)
		{
			std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
			newBullet = TargetBullet::Create(modelBullet, { player->GetPosition().x, player->GetPosition().y + 0.3f, player->GetPosition().z }, { 1.0f, 1.0f, 1.0f }, boss->GetPosition(), 15.0f, camera->GetEye(), camera->GetTarget(), camera->GetUp());
			newBullet->eyePosition = camera->GetEye();
			//newBullet->eyePosition.y += 10.0f;
			newBullet->targetPosition = camera->GetTarget();
			newBullet->upVector = camera->GetUp();
			playerBullets.push_back(std::move(newBullet));

			shotFlag = false;
			shotRate = 1.5f;
		}
	}
}

void GamePlay::PlayerMovementBoundaryChecking()
{
	if (player->CheckCollisionWithBoss(boss->GetPosition(), 80.0f))
	{
		float dx = player->GetPosition().x - boss->GetPosition().x;
		float dy = player->GetPosition().y - boss->GetPosition().y;
		float dz = player->GetPosition().z - boss->GetPosition().z;

		float length = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

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

		while (player->CheckCollisionWithBoss(boss->GetPosition(), 80.0f))
		{
			player->MoveTowards(newX, player->GetPosition().x + dx, 1.0f, elapsedTime);
			player->MoveTowards(newY, player->GetPosition().y + dy, 1.0f, elapsedTime);
			player->MoveTowards(newZ, player->GetPosition().z + dz, 1.0f, elapsedTime);
			player->SetPosition({ newX, newY, newZ });
			elapsedTime += 0.1f;
		}
	}
	if (!player->CheckCollisionWithBoss(boss->GetPosition(), 440.0f))
	{
		float dx = player->GetPosition().x - boss->GetPosition().x;
		float dy = player->GetPosition().y - boss->GetPosition().y;
		float dz = player->GetPosition().z - boss->GetPosition().z;

		float length = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

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

		while (!player->CheckCollisionWithBoss(boss->GetPosition(), 440.0f))
		{
			player->MoveTowards(newX, player->GetPosition().x - dx, 1.0f, elapsedTime);
			player->MoveTowards(newY, player->GetPosition().y - dy, 1.0f, elapsedTime);
			player->MoveTowards(newZ, player->GetPosition().z - dz, 1.0f, elapsedTime);
			player->SetPosition({ newX, newY, newZ });
			elapsedTime += 0.1f;
		}
	}
}

void GamePlay::BossTargetShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet = Bullet::Create(modelBullet, startPosition, { 1.0f, 1.0f, 1.0f }, endPosition, bulletSpeed);

	bossTargetBullets.push_back(std::move(newBullet));
}

// コア撃破エフェクト
void GamePlay::CoreBreakEffect()
{
	//ライフが0になった時にオブジェクトの位置から撃破パーティクルを発生
	// コア1
	if (bossCore_1->GetAliveFlag() == false)
	{
		if (bossCore_1->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_1(20, bossCore_1->GetWorldPosition(), 20.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
		}
	}

	// コア2
	if (bossCore_2->GetAliveFlag() == false)
	{
		if (bossCore_2->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_2(20, bossCore_2->GetWorldPosition(), 20.0f, 0.0f, bossCore_2->GetColorRed(), bossCore_2->GetColorRed());
		}
	}

	// コア3
	if (bossCore_3->GetAliveFlag() == false)
	{
		if (bossCore_3->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_3(20, bossCore_3->GetWorldPosition(), 20.0f, 0.0f, bossCore_3->GetColorRed(), bossCore_3->GetColorRed());
		}
	}

	// コア4
	if (bossCore_4->GetAliveFlag() == false)
	{
		if (bossCore_4->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_4(20, bossCore_4->GetWorldPosition(), 20.0f, 0.0f, bossCore_4->GetColorRed(), bossCore_4->GetColorRed());
		}
	}
}

// コアヒットエフェクト
void GamePlay::CoreHitEffect()
{
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// コア1の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossCore_1->GetWorldPosition(), 8.0f))
		{
			if (bossCore_1->GetLife() <= 1 && bossCore_1->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_1->GetWorldPosition(), 50.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_1->GetWorldPosition(), 25.0f, 0.0f, bossCore_1->GetColorYellow(), bossCore_1->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_1->GetWorldPosition(), 25.0f, 0.0f, bossCore_1->GetColorOrange(), bossCore_1->GetColorOrange());
			}
			bossCore_1->colorTimeRate = 0.0f;
			bossCore_1->colorTimeRate2 = 0.0f;
			bossCore_1->life--;
			bullet->deathFlag = true;
		}

		// コア2の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossCore_2->GetWorldPosition(), 8.0f))
		{
			if (bossCore_2->GetLife() <= 1 && bossCore_2->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_2->GetWorldPosition(), 50.0f, 0.0f, bossCore_2->GetColorRed(), bossCore_2->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_2->GetWorldPosition(), 25.0f, 0.0f, bossCore_2->GetColorYellow(), bossCore_2->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_2->GetWorldPosition(), 25.0f, 0.0f, bossCore_2->GetColorOrange(), bossCore_2->GetColorOrange());
			}
			bossCore_2->colorTimeRate = 0.0f;
			bossCore_2->colorTimeRate2 = 0.0f;
			bossCore_2->life--;
			bullet->deathFlag = true;
		}

		// コア3の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossCore_3->GetWorldPosition(), 8.0f))
		{
			if (bossCore_3->GetLife() <= 1 && bossCore_3->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_3->GetWorldPosition(), 50.0f, 0.0f, bossCore_3->GetColorRed(), bossCore_3->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_3->GetWorldPosition(), 25.0f, 0.0f, bossCore_3->GetColorYellow(), bossCore_3->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_3->GetWorldPosition(), 25.0f, 0.0f, bossCore_3->GetColorOrange(), bossCore_3->GetColorOrange());
			}
			bossCore_3->colorTimeRate = 0.0f;
			bossCore_3->colorTimeRate2 = 0.0f;
			bossCore_3->life--;
			bullet->deathFlag = true;
		}

		// コア4の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossCore_4->GetWorldPosition(), 8.0f))
		{
			if (bossCore_4->GetLife() <= 1 && bossCore_4->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_4->GetWorldPosition(), 50.0f, 0.0f, bossCore_4->GetColorRed(), bossCore_4->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_4->GetWorldPosition(), 25.0f, 0.0f, bossCore_4->GetColorYellow(), bossCore_4->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_4->GetWorldPosition(), 25.0f, 0.0f, bossCore_4->GetColorOrange(), bossCore_4->GetColorOrange());
			}
			bossCore_4->colorTimeRate = 0.0f;
			bossCore_4->colorTimeRate2 = 0.0f;
			bossCore_4->life--;
			bullet->deathFlag = true;
		}

		// メインコアの疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossMainCore->GetWorldPosition(), 8.0f))
		{
			if (bossMainCore->GetLife() <= 1 && bossMainCore->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossMainCore->GetWorldPosition(), 50.0f, 0.0f, bossMainCore->GetColorRed(), bossMainCore->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossMainCore->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorYellow(), bossMainCore->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossMainCore->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorOrange(), bossMainCore->GetColorOrange());
			}

			if (bossMainCore->isAlive == true)
			{
				bossMainCore->life--;
			}
			
			bossMainCore->colorTimeRate = 0.0f;
			bossMainCore->colorTimeRate2 = 0.0f;
			bullet->deathFlag = true;
		}
	}
}

// ボスパーツヒットエフェクト
void GamePlay::BossPartsHitEffect()
{
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// Turret 1 collision detection
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossTurret_1->GetWorldPosition(), 8.0f))
		{
			// 必要なときはいつでも、次の3行を自由に復元してください。
			if (bossTurret_1->isAlive == true)
			{
				bossTurret_1->colorTimeRate = 0.0f;
				bossTurret_1->life--;
			}
			bullet->deathFlag = true;
		}

		// Turret 2 collision detection
		//if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossTurret_2->GetWorldPosition(), 8.0f))
		//{
		//	// 必要なときはいつでも、次の3行を自由に復元してください。
		//	if (bossTurret_2->isAlive == true)
		//	{
		//		bossTurret_2->colorTimeRate = 0.0f;
		//		bossTurret_2->life--;
		//	}
		//	bullet->deathFlag = true;
		//}
	}
}

void GamePlay::CoreAllBreak()
{
	if (bossMainCore->isBreak == true)
	{
		bossCore_1->VarReset();
		bossCore_2->VarReset();
		bossCore_3->VarReset();
		bossCore_4->VarReset();

		bossMainCore->VarReset();

		if (bossCore_1->isAlive && bossCore_2->isAlive && bossCore_3->isAlive && bossCore_4->isAlive && bossMainCore->isAlive == false)
		{
			bossMainCore->isBreak = false;
		}
	}

	if (bossMainCore->isAlive == false)
	{
		if (!bossCore_1->isAlive && !bossCore_2->isAlive && !bossCore_3->isAlive && !bossCore_4->isAlive)
		{
			bossMainCore->isAlive = true;
		}
	}
}

// 当たり判定
bool GamePlay::BasicCollisionDetection(XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize)
{
	XMVECTOR s1Pos = XMLoadFloat3(&bulletPos);
	XMVECTOR s2Pos = XMLoadFloat3(&bossPos);
	float distance = XMVectorGetX(XMVector3Length(s1Pos - s2Pos));
	float radiusSum = bulletSize + bossSize;

	return distance <= radiusSum;
}