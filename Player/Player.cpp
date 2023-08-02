#include "Player.h"

using namespace DirectX;

ID3D12Device* Player::device = nullptr;
//Camera* Player::camera = nullptr;

Player* Player::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
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

	// ���f���̃Z�b�g
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	SetModel(modelPlayer);

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// �f�o�b�O�e�L�X�g������
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

	//�u�[�X�g
	//Boost();
	BoostTest();

	// �I�u�W�F�N�g�ړ�
	Move();

	// ���[�����O
	Rolling();

	DebugTextUpdate();
}

void Player::Draw()
{
	ObjObject::Draw();
}

void Player::DebugTextUpdate()
{
	// �v���C���[�̍��W��\��
	std::ostringstream PlayerPosition;
	PlayerPosition << "PlayerPosition:("
		<< std::fixed << std::setprecision(2)
		<< position.x << "," // x
		<< position.y << "," // y
		<< position.z << ") Local"; // z
	debugText.Print(PlayerPosition.str(), 10, 90, 1.0f);

	// �v���C���[�̊p�x��\��
	std::ostringstream PlayerRotation;
	PlayerRotation << "PlayerRotation:("
		<< std::fixed << std::setprecision(2)
		<< rotation.x << "," // x
		<< rotation.y << "," // y
		<< rotation.z << ")"; // z
	debugText.Print(PlayerRotation.str(), 10, 110, 1.0f);

	std::ostringstream TotalSpeed;
	TotalSpeed << "TotalSpeed:("
		<< std::fixed << std::setprecision(2)
		<< totalSpeed << ")"; // z
	debugText.Print(TotalSpeed.str(), 10, 130, 1.0f);

	std::ostringstream BaseSpeed;
	BaseSpeed << "BaseSpeed:("
		<< std::fixed << std::setprecision(2)
		<< baseSpeed << ")"; // z
	debugText.Print(BaseSpeed.str(), 10, 150, 1.0f);

	std::ostringstream ShootSpeed;
	ShootSpeed << "ShootSpeed:("
		<< std::fixed << std::setprecision(2)
		<< shootSpeed << ")"; // z
	debugText.Print(ShootSpeed.str(), 10, 170, 1.0f);

	std::ostringstream BoostSpeed;
	BoostSpeed << "BoostSpeed:("
		<< std::fixed << std::setprecision(2)
		<< boostSpeed << ")"; // z
	debugText.Print(BoostSpeed.str(), 10, 190, 1.0f);

	//std::ostringstream RollRotation;
	//RollRotation << "RollRotation:("
	//	<< std::fixed << std::setprecision(2)
	//	<< rollRotation.x << "," // x
	//	<< rollRotation.y << "," // y
	//	<< rollRotation.z << ")"; // z
	//debugText.Print(RollRotation.str(), 10, 170, 1.0f);

	std::ostringstream BoostPow_;
	BoostPow_ << "BoostPowNow:("
		<< std::fixed << std::setprecision(2)
		<< BoostPowNow << ")"; // z
	debugText.Print(BoostPow_.str(), 10, 210, 1.0f);

	std::ostringstream BoostFlag_;
	BoostFlag_ << "BoostFlag:("
		<< std::fixed << std::setprecision(2)
		<< BoostFlag << ")"; // z
	debugText.Print(BoostFlag_.str(), 10, 230, 1.0f);
}

void Player::DebugTextDraw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(cmdList);
}

// �O�����ړ�����
void Player::Move()
{
	prevPos = position;

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
				yVel -= 0.4f;
			}
			else if (input->PushKey(DIK_W))
			{
				yVel += 0.4f;
			}

			if (yVel > 0.8f)
			{
				yVel = 0.8f;
			}
			else if (yVel < -0.8f)
			{
				yVel = -0.8f;
			}
		}

		hypotenuse = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		radians = atan2(z, x);
		degrees = XMConvertToDegrees(radians);
	}
	else
	{
		yVel = 0.0f;
	}

	axis.y += yVel;
	if (axis.y > 140.0f)
	{
		axis.y = 140.0f;
		yVel = 0.0f;
	}
	else if (axis.y < -140.0f)
	{
		axis.y = -140.0f;
		yVel = 0.0f;
	}

	y = (axis.y - position.y);
	if (!std::isnan((y / hypotenuse)))
	{
		radians2 = asin(y / hypotenuse);
		degrees2 = XMConvertToDegrees(radians2);
	}

	if (degrees2 > 30.0f)
	{
		degrees2 = 30.0f;
	}
	else if (degrees2 < -30.0f)
	{
		degrees2 = -30.0f;
	}

	if (totalSpeed == 0.0f)
	{
		Vel = { 0.0f, 0.0f, 0.0f };
	}
	else
	{
		Vel.x = (x / hypotenuse);
		Vel.y = (y / hypotenuse);
		Vel.z = (z / hypotenuse);
	}

	position.x += totalSpeed * Vel.x;
	position.y += totalSpeed * Vel.y;
	position.z += totalSpeed * Vel.z;

	/*if (!input->PushKey(DIK_S) && !input->PushKey(DIK_W))
	{
		axis.y = position.y + rotation.x;
	}*/

	SetPosition(position);
	SetRotation({ -degrees2, -degrees + 90.0f, rotation.z });
}

// �O�������̎��@�̌X��
void Player::Rolling()
{
	Input* input = Input::GetInstance();

	// ���[��
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

	// �X����߂�
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

void Player::Boost()
{
	BoostPartColor = { (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , 1.0f };
	if(Input::GetInstance()->PushMouseRight() == true)
	{
		BoostFlag = true;
	}

	if(Input::GetInstance()->PushMouseRight() == false)
	{
		BoostFlag = false;
	}

	if (BoostFlag == true)
	{
		if (BoostPowNow > 0)
		{
			BoostPowNow--;
			totalSpeed = 3.0f;
		}

		if (BoostPowNow <= 0)
		{
			BoostFlag = false;
		}
	}

	if (BoostFlag == false)
	{
		totalSpeed = 2.0f;
		if (BoostPowNow < BoostPowMax)
		{
			BoostPowNow++;
		}
	}
}

void Player::BoostTest()
{
	BoostPartColor = { (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , (float)rand() / RAND_MAX , 1.0f };
	if (Input::GetInstance()->PushMouseRight() == true && boostCancel == false)
	{
		BoostFlag = true;
	}

	if (Input::GetInstance()->PushMouseRight() == false)
	{
		BoostFlag = false;
	}

	if (BoostFlag == true)
	{
		if (BoostPowNow > 0)
		{
			BoostPowNow--;
			boostTimeRate += 0.1f;
		}

		if (BoostPowNow <= 0)
		{
			boostCancel = true;
		}
	}

	if (BoostFlag == false)
	{
		boostTimeRate -= 0.1f;
		if (BoostPowNow < BoostPowMax)
		{
			BoostPowNow++;
		}
	}

	if (boostCancel)
	{
		BoostFlag = false;
		if (BoostPowNow >= BoostPowMax)
		{
			boostCancel = false;
		}
	}

	boostSpeed = Lerp::LerpFloat(boostSpeedMin, boostSpeedMax, boostTimeRate);

	boostSpeed = max(boostSpeed, boostSpeedMin);
	boostSpeed = min(boostSpeed, boostSpeedMax);

	boostTimeRate = max(boostTimeRate, 0.0f);
	boostTimeRate = min(boostTimeRate, 1.0f);

	totalSpeed = (baseSpeed + boostSpeed) - shootSpeed;
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