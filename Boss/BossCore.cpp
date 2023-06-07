#include "BossCore.h"

using namespace DirectX;

BossCore* BossCore::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossCore* instance = new BossCore();
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

bool BossCore::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBossPartsSphere);


	// �ϐ��̃��Z�b�g
	VarReset();

	return true;
}

void BossCore::Update()
{
	ObjObject::Update();

	if (isAlive)
	{
		// �������̃J���[�ύX
		AliveChangeColor();
		// �q�b�g���̃J���[�ύX
		HitChangeColor();
	}
	
	if (!isAlive)
	{
		color = breakColor;
	}

	// HP��0�ɂȂ����猂�j
	if (life <= 0.0f)
	{
		if (DestroyPartTime > 0)
		{
			DestroyPartTime--;
		}

		isAlive = false;
	}
}

// �q�b�g���̃J���[�ύX
void BossCore::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, aliveColor, colorTimeRate);
}

// �������̃J���[�ύX
void BossCore::AliveChangeColor()
{
	colorTimeRate2 += 0.02;
	if (colorTimeRate2 > 1.0f)
	{
		colorTimeRate2 = 0.0f;
	}
	aliveColor = Lerp::LerpFloat4(baseColor, breakColor, colorTimeRate2);
}

void BossCore::TimerReset(int Timer, int ResetValue)
{
	Timer = ResetValue;
}

void BossCore::VarReset()
{
	// �����t���O�̏�����
	isAlive = true;

	// �̗͂̏�����
	life = lifeMax;

	// �J���[�ύX�^�C�����[�g�̏�����
	colorTimeRate = 1.0f;

	//�j��^�C�}�[�̃��Z�b�g
	DestroyPartTime = DestroyDefault;
}
