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

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// �f�o�b�O�e�L�X�g������
	debugText.Initialize(0);

	return true;
}

void Player::Update()
{
	ObjObject::Update();

	rotation.z = rollRotation.z;
	position.x += playerSpeed.x;
	position.y += playerSpeed.y;

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
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(cmdList);
}

// �O�����ړ�����
void Player::Move()
{
	Input* input = Input::GetInstance();

	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, ObjObject::camera->GetViewMatrix());
	const Vector3 camDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], camMatWorld.r[2].m128_f32[1], camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 camDirectionY = Vector3(camMatWorld.r[1].m128_f32[0], camMatWorld.r[1].m128_f32[1], camMatWorld.r[1].m128_f32[2]).Normalize();
	const Vector3 camDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], camMatWorld.r[0].m128_f32[1], camMatWorld.r[0].m128_f32[2]).Normalize();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// �ړ���̍��W���v�Z
		/*if (input->PushKey(DIK_W))
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
		}*/

		moveDirection = {};

		if (input->PushKey(DIK_A))
		{
			moveDirection += camDirectionX * -1;
		}
		else if (input->PushKey(DIK_D))
		{
			moveDirection += camDirectionX;
		}
		if (input->PushKey(DIK_S))
		{
			moveDirection += camDirectionZ * -1;
			//position.y -= 1.0f;
		}
		else if (input->PushKey(DIK_W))
		{
			moveDirection += camDirectionZ;
			//position.y += 1.0f;
		}

		moveDirection.Normalize();
		direction.Normalize();

		float cosA;
		cosA = direction.Dot(moveDirection);

		if (cosA > 1.0f)
		{
			cosA = 1.0f;
		}
		else if (cosA < -1.0f)
		{
			cosA = -1.0f;
		}

		float rotY = (float)acos(cosA) * 180 / 3.14159365f;
		const Vector3 CrossVec = direction.Cross(moveDirection);

		float rotSpeed = rotationSpeed;
		if (abs(rotY) < 55)
		{
			position.x += moveDirection.x * speed;
			position.y += moveDirection.y * speed;
			position.z += moveDirection.z * speed;
		}

		if (rotSpeed > abs(rotY))
		{
			rotSpeed = rotY;
		}

		if (CrossVec.y < 0)
		{
			rotSpeed *= -1;
		}

		//rotation.y += rotSpeed;

		XMMATRIX matRotation = XMMatrixRotationY(XMConvertToRadians(rotSpeed));
		XMVECTOR dir = { direction.x, direction.y, direction.z, 0 };
		dir = XMVector3TransformNormal(dir, matRotation);
		direction = dir;

		SetPosition(position);
	}

	// X���𐧌�
	/*playerSpeed.x = max(playerSpeed.x, -0.4f);
	playerSpeed.x = min(playerSpeed.x, +0.4f);*/

	// Y���𐧌�
	/*playerSpeed.y = max(playerSpeed.y, -0.4f);
	playerSpeed.y = min(playerSpeed.y, +0.4f);*/

	// �X����߂�
	/*if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && playerSpeed.x != 0.0f)
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
	}*/
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
}
