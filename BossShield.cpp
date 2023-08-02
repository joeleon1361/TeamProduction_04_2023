#include "BossShield.h"

BossShield* BossShield::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossShield* instance = new BossShield();
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

bool BossShield::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelBossPartsShield = ObjModel::CreateFromOBJ("bossPartsShield");
	SetModel(modelBossPartsShield);

	return true;
}

void BossShield::Update()
{
	ObjObject::Update();

	ReflectMotion();
	Revival();
}

void BossShield::HitChangeColor()
{
}

void BossShield::AliveChangeColor()
{
}

void BossShield::ReflectMotion()
{
	timeRate += 0.05f;
	if (timeRate > 1.0f)
	{
		timeRate = 1.0f;
	}

	rotation.y = Easing::OutQuadFloat(180.0f, 540.0f , timeRate);
}

void BossShield::Revival()
{
	if (isRevival)
	{
		revivalTimeRate += 0.05f;
		if (revivalTimeRate > 1.0f)
		{
			isAlive = true;
			isRevival = false;
		}
	}

	scale = Lerp::LerpFloat3({ 0.0f,0.0f,0.0f }, { 2.0f, 2.0f, 2.0f }, revivalTimeRate);

	revivalTimeRate = max(revivalTimeRate, 0.0f);
	revivalTimeRate = min(revivalTimeRate, 1.0f);
}
