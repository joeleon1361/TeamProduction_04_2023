#include "BossMainCore.h"

using namespace DirectX;

BossMainCore* BossMainCore::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossMainCore* instance = new BossMainCore();
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

bool BossMainCore::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBossPartsSphere);

	// �̗͂̏�����
	life = lifeMax;

	// �J���[�ύX�^�C�����[�g�̏�����
	colorTimeRate = 1.0f;

	//�j��^�C�}�[�̃��Z�b�g
	DestroyPartTime = DestroyDefault;

	// �ϐ��̃��Z�b�g
	VarReset();

	return true;
}

void BossMainCore::Update()
{
	ObjObject::Update();

	ChangePosition();

	if (isAlive)
	{
		// �������̃J���[�ύX
		AliveChangeColor();
		// �q�b�g���̃J���[�ύX
		HitChangeColor();

		// HP��0�ɂȂ����猂�j
		if (life <= 0.0f)
		{
			if (DestroyPartTime > 0)
			{
				DestroyPartTime--;
			}

			isAlive = false;
		}

		onTimer--;
		if (onTimer < 0.0f)
		{
			isBreak = true;
		}
	}

	if (!isAlive)
	{
		color = breakColor;
	}
}

void BossMainCore::ChangePosition()
{
	if (isAlive)
	{
		positionTimeRate += 0.01f;
		if (positionTimeRate > 1.0f)
		{
			positionTimeRate = 1.0f;
		}
	}

	if (!isAlive)
	{
		positionTimeRate -= 0.01f;
		if (positionTimeRate < 0.0f)
		{
			positionTimeRate = 0.0f;
		}
	}

	position = Lerp::LerpFloat3(OffPosition, OnPosition, positionTimeRate);
}

void BossMainCore::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, aliveColor, colorTimeRate);
}

void BossMainCore::AliveChangeColor()
{
	colorTimeRate2 += 0.02;
	if (colorTimeRate2 > 1.0f)
	{
		colorTimeRate2 = 0.0f;
	}
	aliveColor = Lerp::LerpFloat4(baseColor, breakColor, colorTimeRate2);
}

void BossMainCore::TimerReset(int Timer, int ResetValue)
{
	Timer = ResetValue;
}

void BossMainCore::VarReset()
{
	// �����t���O�̏�����
	isAlive = false;

	// ���C���R�A�̏o������
	onTimer = 400.0f;
}
