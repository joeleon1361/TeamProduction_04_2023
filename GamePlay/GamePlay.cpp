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
	bossPartsRing = BossPartsRing::Create();
	bossPartsSphere = BossPartsSphere::Create();
	bossCore_1 = BossCore::Create();
	bossCore_2 = BossCore::Create();
	bossCore_3 = BossCore::Create();
	bossCore_4 = BossCore::Create();
	bossCoreBox_1 = BossPartsCoreBox::Create();
	bossCoreBox_2 = BossPartsCoreBox::Create();
	bossCoreBox_3 = BossPartsCoreBox::Create();
	bossCoreBox_4 = BossPartsCoreBox::Create();
	bossTurret_1 = BossTurret::Create();
	bossTurret_2 = BossTurret::Create();

	// モデルセット
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	objSkydome->SetModel(modelSkydome);
	modelBullet = ObjModel::CreateFromOBJ("bullet2");

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
	bossTurret_1->SetPosition({ 0.0f, 1.0f, 0.0f });
	bossTurret_1->SetScale({ 0.4f, 0.4f, 0.4f });
	bossTurret_1->SetParent({ bossPartsSphere });

	// ボスの砲台2
	bossTurret_2->SetPosition({ 0.0f, -1.0, 0.0f });
	bossTurret_2->SetScale({ 0.4f, 0.4f, 0.4f });
	bossTurret_2->SetParent({ bossPartsSphere });

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
				CreateBossHitParticles(worldPos);
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
				CreateBossHitParticles(worldPos2);
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
				CreateBossHitParticles(worldPos3);
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
				CreateBossHitParticles(worldPos3);
			}
			bossCore_4->colorTimeRate = 0.0f;
			bossCore_4->life--;
			bullet->deathFlag = true;
		}
	}

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
	//test->Draw();
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

bool GamePlay::BasicCollisionDetection(XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize)
{
	XMVECTOR s1Pos = XMLoadFloat3(&bulletPos);
	XMVECTOR s2Pos = XMLoadFloat3(&bossPos);
	float distance = XMVectorGetX(XMVector3Length(s1Pos - s2Pos));
	float radiusSum = bulletSize + bossSize;

	return distance <= radiusSum;
}

void GamePlay::CreateBossHitParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = position.x/*((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x*/;
		pos.y = position.y/*((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y*/;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = -10 / 5.0f;

		XMFLOAT3 acc{};

		// 追加
		bossHitParticle->Add(40, pos, vel, acc, { 1.0f,0.1f, 0.1f, 1.0f }, { 1.0f,0.1f, 0.1f, 1.0f }, 20.0f, 0.0f);
	}
}
