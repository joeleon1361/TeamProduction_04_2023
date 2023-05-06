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

	// �����t���O�̏�����
	isAlive = true;

	// �̗͂̏�����
	life = lifeMax;

	// �J���[�ύX�^�C�����[�g�̏�����
	colorTimeRate = 1.0f;

	return true;
}

void BossCore::Update()
{
	ObjObject::Update();

	// �q�b�g���̃J���[�ύX
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);

	// HP��0�ɂȂ����猂�j
	if (life <= 0.0f)
	{
		isAlive = false;
	}
}
