#include "Player.h"

using namespace DirectX;

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

	
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// �ړ���̍��W���v�Z
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

		// X���𐧌�
		playerSpeed.x = max(playerSpeed.x, -0.4f);
		playerSpeed.x = min(playerSpeed.x, +0.4f);

		// Y���𐧌�
		playerSpeed.y = max(playerSpeed.y, -0.4f);
		playerSpeed.y = min(playerSpeed.y, +0.4f);

		// �X����߂�
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
