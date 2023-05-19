#include "Player.h"

using namespace DirectX;

ID3D12Device* Player::device = nullptr;
//Camera* Player::camera = nullptr;

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

	// モデルのセット
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	SetModel(modelPlayer);

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	SetScale({ 10.0f, 10.0f, 10.0f });

	axis = { position.x + cosf(XMConvertToRadians(xAngle)) * 50.0f, 0.0f, position.z + sinf(XMConvertToRadians(xAngle)) * 50.0f };
	x = (axis.x - position.x);
	z = (axis.z - position.z);
	y = (axis.y - position.y);
	hypotenuse = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	return true;
}

void Player::Update()
{
	ObjObject::Update();

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

	//std::ostringstream RollRotation;
	//RollRotation << "RollRotation:("
	//	<< std::fixed << std::setprecision(2)
	//	<< rollRotation.x << "," // x
	//	<< rollRotation.y << "," // y
	//	<< rollRotation.z << ")"; // z
	//debugText.Print(RollRotation.str(), 10, 170, 1.0f);
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
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_A))
			{
				xAngle += 2.0f;
			}
			else if (input->PushKey(DIK_D))
			{
				xAngle -= 2.0f;
			}
			if (xAngle > 360.0f)
			{
				xAngle -= 360.0f;
			}
			else if (xAngle < 0.0f)
			{
				xAngle += 360.0f;
			}
			axis.x = position.x + cosf(XMConvertToRadians(xAngle)) * 50.0f;
			axis.z = position.z + sinf(XMConvertToRadians(xAngle)) * 50.0f;
			x = (axis.x - position.x);
			z = (axis.z - position.z);
		}
		if (input->PushKey(DIK_S) || input->PushKey(DIK_W))
		{
			if (input->PushKey(DIK_S))
			{
				axis.y -= 2.0f;
			}
			else if (input->PushKey(DIK_W))
			{
				axis.y += 2.0f;
			}

			y = (axis.y - position.y);
		}

		hypotenuse = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		radians = atan2(z, x);
		radians2 = asin(y / hypotenuse);
		degrees = XMConvertToDegrees(radians);
		degrees2 = XMConvertToDegrees(radians2);
	}

	Vel.x = (x / hypotenuse);
	Vel.y = (y / hypotenuse);
	Vel.z = (z / hypotenuse);

	position.x += 2.0f * Vel.x;
	position.y += 2.0f * Vel.y;
	position.z += 2.0f * Vel.z;

	if (axis.y > 180.0f)
	{
		axis.y = 180.0f;
	}
	else if (axis.y < -180.0f)
	{
		axis.y = -180.0f;
	}

	SetPosition(position);
	SetRotation({ -degrees2, -degrees + 90.0f, rotation.z });
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

	SetRotation({ rotation.x, rotation.y, rollRotation.z });
}

bool Player::CheckCollisionWithBoss(XMFLOAT3 bossPos, float collisionRadius)
{
	float dx = bossPos.x - position.x;
	float dy = bossPos.y - position.y;
	float dz = bossPos.z - position.z;

	float distance = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

	return distance <= collisionSphereRadius + collisionRadius;
}

void Player::MoveTowards(float& current, float target, float speed, float elapsedTime)
{
	float delta = target - current;
	float step = speed * elapsedTime;
	if (step > abs(delta))
	{
		current = target;
	}
	else
	{
		current += step * (delta < 0 ? -1 : 1);
	}
}