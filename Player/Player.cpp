#include "Player.h"

using namespace DirectX;

Player* Player::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	return true;
}

void Player::Update()
{
	ObjObject::Update();

	rotation.z = rollRotation.z;
	position.x += playerSpeed.x;
	position.y += playerSpeed.y;

	// オブジェクト移動
	Move();

	// ローリング
	Rolling();

	DebugTextUpdate();
}

void Player::Draw()
{
	ObjObject::Draw();
}

void Player::DebugTextUpdate()
{
	// プレイヤーの座標を表示
	std::ostringstream PlayerPosition;
	PlayerPosition << "PlayerPosition:("
		<< std::fixed << std::setprecision(2)
		<< position.x << "," // x
		<< position.y << "," // y
		<< position.z << ") Local"; // z
	debugText.Print(PlayerPosition.str(), 10, 90, 1.0f);

	// プレイヤーの角度を表示
	std::ostringstream PlayerRotation;
	PlayerRotation << "PlayerRotation:("
		<< std::fixed << std::setprecision(2)
		<< rotation.x << "," // x
		<< rotation.y << "," // y
		<< rotation.z << ")"; // z
	debugText.Print(PlayerRotation.str(), 10, 110, 1.0f);

	std::ostringstream PlayerSpeed;
	PlayerSpeed << "PlayerSpeed:("
		<< std::fixed << std::setprecision(2)
		<< playerSpeed.x << "," // x
		<< playerSpeed.y << "," // y
		<< playerSpeed.z << ")"; // z
	debugText.Print(PlayerSpeed.str(), 10, 130, 1.0f);

	std::ostringstream RollRotation;
	RollRotation << "RollRotation:("
		<< std::fixed << std::setprecision(2)
		<< rollRotation.x << "," // x
		<< rollRotation.y << "," // y
		<< rollRotation.z << ")"; // z
	debugText.Print(RollRotation.str(), 10, 170, 1.0f);
}

void Player::DebugTextDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);
}

// 前方向移動処理
void Player::Move()
{
	Input* input = Input::GetInstance();

	
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W))
			{
				playerSpeed.y += 0.05f;
			}
			if (input->PushKey(DIK_S))
			{
				playerSpeed.y -= 0.05f;
			}
			if (input->PushKey(DIK_D))
			{
				playerSpeed.x += 0.05f;
			}
			if (input->PushKey(DIK_A))
			{
				playerSpeed.x -= 0.05f;
			}
		}

		// X軸を制限
		playerSpeed.x = max(playerSpeed.x, -0.4f);
		playerSpeed.x = min(playerSpeed.x, +0.4f);

		// Y軸を制限
		playerSpeed.y = max(playerSpeed.y, -0.4f);
		playerSpeed.y = min(playerSpeed.y, +0.4f);

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && playerSpeed.x != 0.0f)
		{
			if (playerSpeed.x > 0.0f)
			{
				playerSpeed.x -= 0.02f;
			}

			if (playerSpeed.x < 0.0f)
			{
				playerSpeed.x += 0.02f;
			}
		}

		if (input->PushKey(DIK_W) == 0 && input->PushKey(DIK_S) == 0 && playerSpeed.y != 0.0f)
		{
			if (playerSpeed.y > 0.0f)
			{
				playerSpeed.y -= 0.02f;
			}

			if (playerSpeed.y < 0.0f)
			{
				playerSpeed.y += 0.02f;
			}
		}
}

// 前方向時の自機の傾き
void Player::Rolling()
{
	Input* input = Input::GetInstance();

	
		// ロール
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_A) && rollRotation.z <= +40.0f)
			{
				rollRotation.z += 5.0f;
			}

			if (input->PushKey(DIK_D) && rollRotation.z >= -40.0f)
			{
				rollRotation.z -= 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
			}
		}
}
