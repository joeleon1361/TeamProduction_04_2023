#include "SecondStage.h"

using namespace DirectX;
extern HWND hwnd;

SecondStage::SecondStage()
{
}

SecondStage::~SecondStage()
{
}

void SecondStage::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);
	Player::SetCamera(camera);
	Boss::SetCamera(camera);

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

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	secondStageBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	Reticle = Sprite::Create(TextureNumber::reticle, ReticlePos);

	test = Sprite::Create(TextureNumber::reticle, { (float)mousePosition.x, (float)mousePosition.y });

	// ブーストゲージ
	gageBoost = GageUI::Create(boostUIPosition, { 530.0f, 30.0f }, { 0.6f, 0.6f, 0.1f, 1.0f });
	gageBossHp = DeltaGageUI::Create(bossHpUIPosition, { 530.0f, 30.0f });
	gagePlayerHp = DeltaGageUI::Create(playerHpUIPosition, { 530.0f, 30.0f });
	gageCharge = GageUI::Create(playerChargeUIPosition, { 530.0f, 30.0f }, { 0.6f, 0.1f, 0.1f, 1.0f });

	// 速度ゲージ
	gageSpeed = GageUI::Create(playerSpeedUIPosition, { 530.0f, 30.0f }, { 0.1f, 0.6f, 0.6f, 1.0f });

	Black = Sprite::Create(TextureNumber::black, { 0.0f, 0.0f });

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
	bossTurretStand_1 = BossParts::Create();
	//bossTurretStand_2 = BossParts::Create();
	bossTurret_1 = BossTurret::Create();
	//bossTurret_2 = BossTurret::Create();
	bossPartsCoreStand = BossParts::Create();
	bossMainCore = BossMainCore::Create();
	bossShield = BossShield::Create();

	// モデルセット
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	objSkydome->SetModel(modelSkydome);

	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	modelBossPartsCoreBox = ObjModel::CreateFromOBJ("bossPartsCoreBox");
	

	modelBossPartsRing = ObjModel::CreateFromOBJ("bossPartsRing");
	bossPartsRing->SetModel(modelBossPartsRing);

	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	bossPartsSphere->SetModel(modelBossPartsSphere);

	modelBossPartsTurretStand = ObjModel::CreateFromOBJ("TurretStand");
	bossTurretStand_1->SetModel(modelBossPartsTurretStand);
	//bossTurretStand_2->SetModel(modelBossPartsTurretStand);

	modelBossPartsCoreStand = ObjModel::CreateFromOBJ("bossPartsCoreStand");
	bossPartsCoreStand->SetModel(modelBossPartsCoreStand);



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

	bossPartsCoreStand->SetPosition({ 0.0f, 0.0f, -3.5f });
	bossPartsCoreStand->SetRotation({ -90.0f, 0.0f, 0.0f });
	bossPartsCoreStand->SetScale({ 2.0f, 2.0f, 2.0f });
	bossPartsCoreStand->SetColor({ 0.647f, 0.619f, 0.658f, 1.0f });
	bossPartsCoreStand->SetParent(boss);

	// ボスのメインコア
	bossMainCore->SetPosition({ 0.0f, 0.0f, 0.0f });
	bossMainCore->SetScale({ 0.75f, 0.75f, 0.75f });
	bossMainCore->SetParent(bossPartsCoreStand);

	// ボスの砲台1
	bossTurretStand_1->SetPosition({ 0.0f, 0.9f, 0.0f });
	bossTurretStand_1->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_1->SetParent({ bossPartsSphere });

	bossTurret_1->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_1->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_1->SetParent({ bossTurretStand_1 });

	// ボスの盾
	bossShield->SetPosition({ 0.0f, 0.0f, -9.0f });
	bossShield->SetRotation({ 0.0f,180.0f, 0.0f });
	bossShield->SetScale({ 2.0f, 2.0f, 2.0f });
	bossShield->SetParent(boss);

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	camera->SetTarget(boss->GetPosition());
	camera->SetEye({ 0, 0, -10 });
	camera->SetUp({ 0, 1, 0 });
	//camera->SetDistance(48.0f);

	ShowCursor(false);
}

void SecondStage::Finalize()
{
}

void SecondStage::Update()
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

	// Debug Use Only
	if (input->TriggerKey(DIK_C))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	//circleParticle->SparkParticle(20, 50, bossCore_1->GetWorldPosition(), 10.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());

	// コアヒットエフェクト
	CoreHitEffect();

	// ボスパーツヒットエフェクト
	BossPartsHitEffect();

	// プレイヤーの球発射処理
	if (playerBulletType == Normal)
	{
		Shoot();

		if (input->TriggerKey(DIK_T))
		{
			playerBulletType = Charge;
		}
	}
	else if (playerBulletType == Charge)
	{
		chargeShoot();

		if (input->TriggerKey(DIK_T))
		{
			playerBulletType = Normal;
		}
	}

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

	// ボスの砲台1を一定間隔で発射
	if (bossTurret_1->isAlive == true)
	{
		bossTurret_1->shotTimer--;
		if (bossTurret_1->shotTimer <= 0)
		{
			//BossReflectShoot(bossTurret_1->GetWorldPosition(), player->GetPosition(), 1.0f);
			//BossTargetShoot(bossTurret_1->GetWorldPosition(), player->GetPosition(), 10.0f);
			//bossShield->isAlive = true;
			bossTurret_1->shotTimer = 720.0f;
		}
	}

	if (input->TriggerKey(DIK_U))
	{
		bossTurret_1->isShot = true;
		bossShield->isAlive = true;
	}

	// 反射弾弾をフラグが立ったら発射
	if (bossTurret_1->isShot == true)
	{
		BossReflectShoot(bossTurret_1->GetWorldPosition(), player->GetPosition(), 1.0f);
		bossTurret_1->isShot = false;
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

	// ボスの反射弾を更新
	for (std::unique_ptr<ReflectBullet>& reflectBullet : bossReflectBullets)
	{
		if (reflectBullet->isReflect)
		{
			circleParticle->BulletParticle(5, 15, reflectBullet->GetPosition(), { 1.0f,0.1f, 0.1f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 30.0f);
		}
		reflectBullet->Update(bossShield->GetWorldPosition());
	}

	// ボスの反射弾を消去
	bossReflectBullets.remove_if([](std::unique_ptr<ReflectBullet>& reflectBullet)
		{
			return reflectBullet->GetDeathFlag();
		}
	);

	//オブジェクトパーティクルを更新
	for (std::unique_ptr<ObjectParticle>& part : particle)
	{
		//part->Update();
	}

	particle.remove_if([](std::unique_ptr<ObjectParticle>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// コア撃破エフェクト
	CoreBreakEffect();

	// 全てのコアを破壊した後
	CoreAllBreak();

	if (input->PushKey(DIK_H))
	{
		playerSpeedUIPosition.y += 1.0f;
	}

	PlayerHitEffect();

	ReflectHitEffect();

	// ブーストゲージ
	gageBoost->Update(player->GetBoostPowNow(), player->GetBoostPowMax(), boostUIPosition, { 0.6f, 0.6f, 0.1f, 1.0f }, { 0.6f, 0.1f, 0.1f, 1.0f });
	// ボスのHPゲージ
	gageBossHp->Update(bossMainCore->life, bossMainCore->lifeMax, bossHpUIPosition);
	// プレイヤーの速度ゲージ
	gageSpeed->Update(player->GetTotalSpeed(), player->GetTotalSpeedMax(), playerSpeedUIPosition, { 0.1f, 0.6f, 0.6f, 1.0f }, { 0.1f, 0.6f, 0.6f, 1.0f });

	gagePlayerHp->Update(player->HP, player->HPMAX, playerHpUIPosition);

	gageCharge->Update(chargeNow, chargeMax, playerChargeUIPosition, { 0.1f, 0.6f, 0.1f, 1.0f }, { 0.6f, 0.1f, 0.1f, 1.0f });

	// カメラターゲットのセット
	// camera->SetTarget(boss->GetPosition());
	// camera->SetDistance(sqrtf(pow(boss->GetPosition().x - player->GetPosition().x, 2) + pow(boss->GetPosition().y - player->GetPosition().y, 2) + pow(boss->GetPosition().z - player->GetPosition().z, 2)) + 48.0f);

	// カメラの更新
	camera->Update(player->GetPosition(), boss->GetPosition());

	// プレイヤーの更新
	player->Update();
	boss->Update();
	bossPartsRing->Update();
	bossPartsSphere->Update();
	
	bossTurret_1->Update(player->GetPosition());
	//bossTurret_2->Update();
	bossTurretStand_1->SetRotation({ bossTurretStand_1->GetRotation().x, bossTurret_1->GetExternRotationY(), bossTurretStand_1->GetRotation().z });
	bossTurretStand_1->Update();
	//bossTurretStand_2->Update();
	bossPartsCoreStand->Update();
	bossMainCore->Update();
	bossShield->Update();

	PlayerMovementBoundaryChecking();

	objSkydome->Update();
	Reticle->SetAnchorPoint({ 0.5f, 0.5f });
	Reticle->SetPosition(ReticlePos);

	test->SetPosition({ (float)mousePosition.x, (float)mousePosition.y });

	DrawDebugText();

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

void SecondStage::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	secondStageBG->Draw();

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

	for (std::unique_ptr<ReflectBullet>& reflectBullet : bossReflectBullets)
	{
		reflectBullet->Draw();
	}

	bossPartsRing->Draw();
	bossPartsSphere->Draw();
	
	bossTurret_1->Draw();
	
	bossTurretStand_1->Draw();
	
	bossPartsCoreStand->Draw();

	bossMainCore->Draw();

	bossShield->Draw();
	
	

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

	// 前景スプライト描画
	Reticle->Draw();

	gageBoost->Draw();
	gageBossHp->Draw();
	gageSpeed->Draw();
	gagePlayerHp->Draw();
	gageCharge->Draw();

	player->DebugTextDraw();
	debugText.DrawAll(cmdList);
	//Rule->Draw();
	Black->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

// マウスの処理
void SecondStage::GetMouse()
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

void SecondStage::DrawDebugText()
{
	std::ostringstream SecondStage;
	SecondStage << "SecondStage";
	debugText.Print(SecondStage.str(), 10, 10, 2.0f);

	std::ostringstream PlayerHP;
	PlayerHP << "Turn:("
		<< std::fixed << std::setprecision(2)
		<< turnCount << ")"; // z
	debugText.Print(PlayerHP.str(), 10, 560, 1.0f);

}

// プレイヤー弾発射
void SecondStage::Shoot()
{
	shotRate -= 0.1f;

	PlayerPos = player->GetPosition();

	XMVECTOR bulletVelocity = { 0,0,1.0f };

	player->shootSpeed = Lerp::LerpFloat(player->GetShootSpeedMin(), player->GetNormalShootSpeedMax(), player->shootSpeedTimeRate);

	if (Input::GetInstance()->PushKey(DIK_SPACE) || Input::GetInstance()->PushMouseLeft())
	{
		if (shotRate <= 0)
		{
			shotFlag = true;
		}

		if (shotFlag == true)
		{
			std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
			newBullet = TargetBullet::Create(modelBullet, { player->GetPosition().x, player->GetPosition().y + 0.3f, player->GetPosition().z }, { 1.0f, 1.0f, 1.0f }, boss->GetPosition(), 30.0f, camera->GetEye(), camera->GetTarget(), camera->GetUp(), player->GetPosition());
			newBullet->eyePosition = camera->GetEye();
			//newBullet->eyePosition.y += 10.0f;
			newBullet->targetPosition = camera->GetTarget();
			newBullet->upVector = camera->GetUp();
			playerBullets.push_back(std::move(newBullet));

			shotFlag = false;
			shotRate = 1.5f;
		}

		player->shootSpeedTimeRate += 0.05f;
	}
	else if (!Input::GetInstance()->PushMouseLeft())
	{
		player->shootSpeedTimeRate -= 0.1f;
	}

	player->shootSpeed = max(player->shootSpeed, player->GetShootSpeedMin());
	player->shootSpeed = min(player->shootSpeed, player->GetNormalShootSpeedMax());

	player->shootSpeedTimeRate = max(player->shootSpeedTimeRate, 0.0f);
	player->shootSpeedTimeRate = min(player->shootSpeedTimeRate, 1.0f);
}

void SecondStage::chargeShoot()
{
	chargeRatio = chargeNow / chargeMax;

	chargeSize = Easing::InOutQuadFloat(0.0, 10.0, chargeRatio);

	circleParticle->BulletParticle(5, 2, player->GetPosition(), { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, chargeSize);

	player->shootSpeed = Lerp::LerpFloat(player->GetShootSpeedMin(), player->GetChargeShootSpeedMax(), player->shootSpeedTimeRate);

	if (isCharge == false)
	{
		if (chargeNow < chargeMax)
		{
			if (Input::GetInstance()->PushMouseLeft())
			{
				chargeNow++;
				player->shootSpeedTimeRate += 0.05f;
			}
			else if (!Input::GetInstance()->PushMouseLeft())
			{
				chargeNow -= 2.0f;
				player->shootSpeedTimeRate -= 0.1f;
			}
		}
		else
		{
			if (!Input::GetInstance()->PushMouseLeft())
			{
				isCharge = true;
			}
		}
	}
	else
	{
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, { player->GetPosition().x, player->GetPosition().y + 0.3f, player->GetPosition().z }, { 1.0f, 1.0f, 1.0f }, boss->GetPosition(), 15.0f, camera->GetEye(), camera->GetTarget(), camera->GetUp(), player->GetPosition());
		newBullet->eyePosition = camera->GetEye();
		//newBullet->eyePosition.y += 10.0f;
		newBullet->targetPosition = camera->GetTarget();
		newBullet->upVector = camera->GetUp();
		playerBullets.push_back(std::move(newBullet));

		chargeNow = 0.0f;
		isCharge = false;
	}

	player->shootSpeed = max(player->shootSpeed, player->GetShootSpeedMin());
	player->shootSpeed = min(player->shootSpeed, player->GetChargeShootSpeedMax());

	player->shootSpeedTimeRate = max(player->shootSpeedTimeRate, 0.0f);
	player->shootSpeedTimeRate = min(player->shootSpeedTimeRate, 1.0f);

	chargeNow = max(chargeNow, 0.0);
	chargeNow = min(chargeNow, chargeMax);
}

void SecondStage::PlayerMovementBoundaryChecking()
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

void SecondStage::BossTargetShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet = Bullet::Create(modelBullet, startPosition, { 1.0f, 1.0f, 1.0f }, endPosition, bulletSpeed);

	bossTargetBullets.push_back(std::move(newBullet));
}

void SecondStage::BossReflectShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed)
{
	std::unique_ptr<ReflectBullet> newBullet = std::make_unique<ReflectBullet>();
	newBullet = ReflectBullet::Create(modelBossPartsSphere, startPosition, { 10.0f, 10.0f, 10.0f }, endPosition, bulletSpeed);

	bossReflectBullets.push_back(std::move(newBullet));
}

// コア撃破エフェクト
void SecondStage::CoreBreakEffect()
{
	
}

// コアヒットエフェクト
void SecondStage::CoreHitEffect()
{
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// メインコアの疑似ヒット処理
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossMainCore->GetWorldPosition(), 24.0f))
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
void SecondStage::BossPartsHitEffect()
{
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// Turret 1 collision detection
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossTurret_1->GetWorldPosition(), 24.0f))
		{
			// 必要なときはいつでも、次の3行を自由に復元してください。
			if (bossTurret_1->isAlive == true)
			{
				bossTurret_1->colorTimeRate = 0.0f;
				bossTurret_1->life--;
			}
			bullet->deathFlag = true;
		}

		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossShield->GetWorldPosition(), 24.0f) && bossShield->isAlive == true)
		{
			bullet->deathFlag = true;
		}
	}
}

void SecondStage::PlayerHitEffect()
{
	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, player->GetPosition(), 8.0f))
		{
			player->HP -= 1.0f;
			circleParticle->DefaultParticle(20, 50, player->GetPosition(), 20.0f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

			bullet->deathFlag = true;
		}
	}

	if (player->HP <= 0.0f)
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}
}

void SecondStage::ReflectHitEffect()
{
	for (std::unique_ptr<ReflectBullet>& reflectBullet : bossReflectBullets)
	{
		for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
		{
			if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, reflectBullet->GetPosition(), 19.0f))
			{
				reflectBullet->colorTimeRate = 0.0f;
				reflectBullet->life -= 1.0f;

				bullet->deathFlag = true;
			}
		}

		if (reflectBullet->isReflect)
		{
			if (Collision::CCDCollisionDetection(reflectBullet->prevPosition, reflectBullet->GetPosition(), 18.0f, bossShield->GetWorldPosition(), 5.0f))
			{
				// ラリーON
				if (turnCount == 1) // 1ターン目
				{
					if (reflectBullet->rallyCount == 1)
					{
						circleParticle->DefaultParticle(20, 50, bossShield->GetWorldPosition(), 50.0f, 0.0f, bossMainCore->GetColorRed(), bossMainCore->GetColorRed());
						circleParticle->DefaultParticle(10, 50, bossShield->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorYellow(), bossMainCore->GetColorYellow());
						circleParticle->DefaultParticle(10, 50, bossShield->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorOrange(), bossMainCore->GetColorOrange());

						bossMainCore->life -= 5.0f;
						bossShield->isAlive = false;
						bossShield->revivalTimeRate = 0.0f;
						reflectBullet->deathFlag = true;
						reflectBullet->rallyCount = 1;
						turnCount++;
					}
				}
				else if (turnCount == 2) // 2ターン目
				{
					if (reflectBullet->rallyCount == 1)
					{
						bossShield->timeRate = 0.0f;
						reflectBullet->RallyReset(reflectBullet->GetPosition(), player->GetPosition(), 2.0f, 5.0f);
						reflectBullet->rallyCount++;
						reflectBullet->isReflect = false;
					}
					else if (reflectBullet->rallyCount == 2)
					{
						circleParticle->DefaultParticle(20, 50, bossShield->GetWorldPosition(), 50.0f, 0.0f, bossMainCore->GetColorRed(), bossMainCore->GetColorRed());
						circleParticle->DefaultParticle(10, 50, bossShield->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorYellow(), bossMainCore->GetColorYellow());
						circleParticle->DefaultParticle(10, 50, bossShield->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorOrange(), bossMainCore->GetColorOrange());

						bossMainCore->life -= 5.0f;
						bossShield->isAlive = false;
						bossShield->revivalTimeRate = 0.0f;
						reflectBullet->deathFlag = true;
						reflectBullet->rallyCount = 1;
						turnCount++;
					}
				}
				else if (turnCount == 3) // 3ターン目
				{
					if (reflectBullet->rallyCount == 1)
					{
						bossShield->timeRate = 0.0f;
						reflectBullet->RallyReset(reflectBullet->GetPosition(), player->GetPosition(), 2.0f, 5.0f);
						reflectBullet->rallyCount++;
						reflectBullet->isReflect = false;
					}
					else if (reflectBullet->rallyCount == 2)
					{
						bossShield->timeRate = 0.0f;
						reflectBullet->RallyReset(reflectBullet->GetPosition(), player->GetPosition(), 3.0f, 3.0f);
						reflectBullet->rallyCount++;
						reflectBullet->isReflect = false;
					}
					else if (reflectBullet->rallyCount == 3)
					{
						circleParticle->DefaultParticle(20, 50, bossShield->GetWorldPosition(), 50.0f, 0.0f, bossMainCore->GetColorRed(), bossMainCore->GetColorRed());
						circleParticle->DefaultParticle(10, 50, bossShield->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorYellow(), bossMainCore->GetColorYellow());
						circleParticle->DefaultParticle(10, 50, bossShield->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorOrange(), bossMainCore->GetColorOrange());

						bossMainCore->life -= 5.0f;
						bossShield->isAlive = false;
						bossShield->revivalTimeRate = 0.0f;
						reflectBullet->deathFlag = true;
						reflectBullet->rallyCount = 1;
					}
				}
			}
		}
	}
}

void SecondStage::CoreAllBreak()
{
	if (bossMainCore->isBreak == true)
	{
		bossShield->isRevival = true;

		bossMainCore->VarReset();

		if (bossShield->isAlive)
		{
			bossMainCore->isBreak = false;
		}
	}

	if (bossMainCore->isAlive == false)
	{
		if (bossShield->isAlive == false)
		{
			bossMainCore->isAlive = true;
		}
	}
}