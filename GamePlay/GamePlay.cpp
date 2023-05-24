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
	if (!Sprite::LoadTexture(TextureNumber::reticle, L"Resources/Reticle.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト初期化
	debugText.Initialize(0);

	// スプライト
	gameBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	Reticle = Sprite::Create(TextureNumber::reticle, ReticlePos);

	test = Sprite::Create(TextureNumber::reticle, { (float)mousePosition.x, (float)mousePosition.y });

	// パーティクル
	bossHitParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

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
	bossTurretStand_2 = BossParts::Create();
	bossTurret_1 = BossTurret::Create();
	bossTurret_2 = BossTurret::Create();

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
	bossTurretStand_2->SetModel(modelBossPartsTurretStand);

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

	// ボスの砲台1
	bossTurretStand_1->SetPosition({ 0.0f, 0.9f, 0.0f });
	bossTurretStand_1->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_1->SetParent({ bossPartsSphere });

	bossTurret_1->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_1->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_1->SetParent({ bossTurretStand_1 });

	// ボスの砲台2
	bossTurretStand_2->SetPosition({ 0.0f, -0.9f, 0.0f });
	bossTurretStand_2->SetRotation({0.0f, 0.0f, 180.0f});
	bossTurretStand_2->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_2->SetParent({ bossPartsSphere });

	bossTurret_2->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_2->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_2->SetParent({ bossTurretStand_2 });

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
	worldPos = Transform::TransformWorldPosition(bossCore_1->GetPosition(), bossCoreBox_1->GetMatWorld());
	worldPos2 = Transform::TransformWorldPosition(bossCore_2->GetPosition(), bossCoreBox_2->GetMatWorld());
	worldPos3 = Transform::TransformWorldPosition(bossCore_3->GetPosition(), bossCoreBox_3->GetMatWorld());
	worldPos4 = Transform::TransformWorldPosition(bossCore_4->GetPosition(), bossCoreBox_4->GetMatWorld());

	bossTurretWorldPosition_1 = Transform::TransformWorldPosition(bossTurret_1->GetPosition(), bossTurretStand_1->GetMatWorld());
	bossTurretWorldPosition_2 = Transform::TransformWorldPosition(bossTurret_2->GetPosition(), bossTurretStand_2->GetMatWorld());

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

	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// コア1の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, worldPos, 8.0f))
		{
			if (bossCore_1->GetLife() <= 1 && bossCore_1->GetLife() >= 0)
			{
				DefaultParticle(20, 50, worldPos, 50.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
				DefaultParticle(10, 50, worldPos, 25.0f, 0.0f, bossCore_1->GetColorYellow(), bossCore_1->GetColorYellow());
				DefaultParticle(10, 50, worldPos, 25.0f, 0.0f, bossCore_1->GetColorOrange(), bossCore_1->GetColorOrange());
			}
			bossCore_1->colorTimeRate = 0.0f;
			bossCore_1->life--;
			bullet->deathFlag = true;
		}

		// コア2の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, worldPos2, 8.0f))
		{
			if (bossCore_2->GetLife() <= 1 && bossCore_2->GetLife() >= 0)
			{
				DefaultParticle(20, 50, worldPos2, 50.0f, 0.0f, bossCore_2->GetColorRed(), bossCore_2->GetColorRed());
				DefaultParticle(10, 50, worldPos2, 25.0f, 0.0f, bossCore_2->GetColorYellow(), bossCore_2->GetColorYellow());
				DefaultParticle(10, 50, worldPos2, 25.0f, 0.0f, bossCore_2->GetColorOrange(), bossCore_2->GetColorOrange());
			}
			bossCore_2->colorTimeRate = 0.0f;
			bossCore_2->life--;
			bullet->deathFlag = true;
		}

		// コア3の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, worldPos3, 8.0f))
		{
			if (bossCore_3->GetLife() <= 1 && bossCore_3->GetLife() >= 0)
			{
				DefaultParticle(20, 50, worldPos3, 50.0f, 0.0f, bossCore_3->GetColorRed(), bossCore_3->GetColorRed());
				DefaultParticle(10, 50, worldPos3, 25.0f, 0.0f, bossCore_3->GetColorYellow(), bossCore_3->GetColorYellow());
				DefaultParticle(10, 50, worldPos3, 25.0f, 0.0f, bossCore_3->GetColorOrange(), bossCore_3->GetColorOrange());
			}
			bossCore_3->colorTimeRate = 0.0f;
			bossCore_3->life--;
			bullet->deathFlag = true;
		}

		// コア4の疑似ヒット処理
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, worldPos4, 8.0f))
		{
			if (bossCore_4->GetLife() <= 1 && bossCore_4->GetLife() >= 0)
			{
				DefaultParticle(20, 50, worldPos4, 50.0f, 0.0f, bossCore_4->GetColorRed(), bossCore_4->GetColorRed());
				DefaultParticle(10, 50, worldPos4, 25.0f, 0.0f, bossCore_4->GetColorYellow(), bossCore_4->GetColorYellow());
				DefaultParticle(10, 50, worldPos4, 25.0f, 0.0f, bossCore_4->GetColorOrange(), bossCore_4->GetColorOrange());
			}
			bossCore_4->colorTimeRate = 0.0f;
			bossCore_4->life--;
			bullet->deathFlag = true;
		}

		// Turret 1 collision detection
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossTurretWorldPosition_1, 8.0f))
		{
			// 必要なときはいつでも、次の3行を自由に復元してください。
			bossTurret_1->colorTimeRate = 0.0f;
			bossTurret_1->life--;
			bullet->deathFlag = true;
		}

		// Turret 2 collision detection
		if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossTurretWorldPosition_2, 8.0f))
		{
			// 必要なときはいつでも、次の3行を自由に復元してください。
			bossTurret_2->colorTimeRate = 0.0f;
			bossTurret_2->life--;
			bullet->deathFlag = true;
		}
	}

	// プレイヤーの球発射処理
	Shoot();

	// プレイヤーの狙い弾を更新
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
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
			BossTargetShoot(bossTurretWorldPosition_1, player->GetPosition(), 10.0f);
			bossTurret_1->shotTimer = bossTurret_1->ShotInterval;
		}
	}
	
	// ボスの砲台2を一定間隔で発射
	if (bossTurret_2->isAlive == true)
	{
		bossTurret_2->shotTimer--;
		if (bossTurret_2->shotTimer <= 0)
		{
			BossTargetShoot(bossTurretWorldPosition_2, player->GetPosition(), 10.0f);
			bossTurret_2->shotTimer = bossTurret_2->ShotInterval;
		}
	}
	

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
		bullet->Update();
	}

	// ボスの狙い弾を消去
	bossTargetBullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	//ライフが0になった時にオブジェクトの位置から撃破パーティクルを発生
	if (bossCore_1->GetAliveFlag() == false)
	{
		if (bossCore_1->GetDestroyPartTime() > 0)
		{
			BlastPart_1(20, worldPos, 20.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
		}
	}

	if (bossCore_2->GetAliveFlag() == false)
	{
		if (bossCore_2->GetDestroyPartTime() > 0)
		{
			BlastPart_2(20, worldPos2, 20.0f, 0.0f, bossCore_2->GetColorRed(), bossCore_2->GetColorRed());
		}
	}


	if (bossCore_3->GetAliveFlag() == false)
	{
		if (bossCore_3->GetDestroyPartTime() > 0)
		{
			BlastPart_3(20, worldPos3, 20.0f, 0.0f, bossCore_3->GetColorRed(), bossCore_3->GetColorRed());
		}
	}

	if (bossCore_4->GetAliveFlag() == false)
	{
		if (bossCore_4->GetDestroyPartTime() > 0)
		{
			BlastPart_4(20, worldPos4, 20.0f, 0.0f, bossCore_4->GetColorRed(), bossCore_4->GetColorRed());
		}
	}

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
	bossTurret_2->Update();
	bossTurretStand_1->Update();
	bossTurretStand_2->Update();

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

	objSkydome->Update();
	Reticle->SetAnchorPoint({ 0.5f, 0.5f });
	Reticle->SetPosition(ReticlePos);

	test->SetPosition({ (float)mousePosition.x, (float)mousePosition.y });

	DrawDebugText();

	// パーティクル更新
	bossHitParticle->Update();
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
		bullet->Draw();
	}

	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		bullet->Draw();
	}

	if (bossCore_1->isAlive || bossCore_2->isAlive || bossCore_3->isAlive || bossCore_4->isAlive)
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
	}

	objSkydome->Draw();

	// パーティクルの描画
	bossHitParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	player->DebugTextDraw();
	Reticle->Draw();
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

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

void GamePlay::DrawDebugText()
{
	//マウスの座標
	std::ostringstream MousePosition;
	MousePosition << "MousePosition("
		<< std::fixed << std::setprecision(5)
		<< mousePosition.x << ","
		<< mousePosition.y << ")";

	debugText.Print(MousePosition.str(), 0, 0, 2.0f);

	//レティクルの座標
	std::ostringstream ReticlePosition;
	ReticlePosition << "ReticlePosition("
		<< std::fixed << std::setprecision(5)
		<< ReticlePos.x << ","
		<< ReticlePos.y << ")";

	debugText.Print(ReticlePosition.str(), 0, 60, 2.0f);
}

void GamePlay::Shoot()
{
	shotRate -= 0.1f;

	XMVECTOR bulletVelocity = { 0,0,1.0f };

	if (Input::GetInstance()->PushKey(DIK_SPACE) || Input::GetInstance()->PushKey(DIK_Z))
	{
		if (shotRate <= 0)
		{
			shotFlag = true;
		}

		if (shotFlag == true)
		{
			std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
			newBullet = TargetBullet::Create(modelBullet, { player->GetPosition().x, player->GetPosition().y + 0.3f, player->GetPosition().z }, { 1.0f, 1.0f, 1.0f }, boss->GetPosition(), 15.0f);
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

void GamePlay::BossTargetShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet = Bullet::Create(modelBullet, startPosition, { 1.0f, 1.0f, 1.0f }, endPosition, bulletSpeed);

	bossTargetBullets.push_back(std::move(newBullet));
}

bool GamePlay::BasicCollisionDetection(XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize)
{
	XMVECTOR s1Pos = XMLoadFloat3(&bulletPos);
	XMVECTOR s2Pos = XMLoadFloat3(&bossPos);
	float distance = XMVectorGetX(XMVector3Length(s1Pos - s2Pos));
	float radiusSum = bulletSize + bossSize;

	return distance <= radiusSum;
}

void GamePlay::DefaultParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor)
{
	for (int i = 0; i < PartNum; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x =((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y =((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = 10.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		// 追加
		bossHitParticle->Add(Life, pos, vel, acc, StartColor, EndColor, StartScale, EndScale);
	}
}

void GamePlay::BlastPart_1(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor)
{
	for (int i = 0; i < 20; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = -10.0f;
		XMFLOAT3 vel{};
		vel.z = (float)rand() / RAND_MAX * rnd_vel + rnd_vel / 2.0f;

		XMFLOAT3 acc{};

		// 追加
		bossHitParticle->Add(Life, pos, vel, acc, StartColor, EndColor, StartScale,EndScale);
	}
}

void GamePlay::BlastPart_2(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor)
{
	for (int i = 0; i < 20; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = position.y;
		pos.z = position.z;

		const float rnd_vel = 10.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};

		// 追加
		bossHitParticle->Add(Life, pos, vel, acc, StartColor, EndColor, StartScale, EndScale);
	}
}

void GamePlay::BlastPart_3(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor)
{
	for (int i = 0; i < 20; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = 10.0f;
		XMFLOAT3 vel{};
		vel.z = (float)rand() / RAND_MAX * rnd_vel + rnd_vel / 2.0f;

		XMFLOAT3 acc{};

		// 追加
		bossHitParticle->Add(Life, pos, vel, acc, StartColor, EndColor, StartScale, EndScale);
	}
}

void GamePlay::BlastPart_4(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor)
{
	for (int i = 0; i < 20; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = position.y;
		pos.z = position.z;;

		const float rnd_vel = -10.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel + rnd_vel / 2.0f;

		XMFLOAT3 acc{};

		// 追加
		bossHitParticle->Add(Life, pos, vel, acc, StartColor, EndColor, StartScale, EndScale);
	}
}
