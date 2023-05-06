#include "BossPartsSphere.h"

BossPartsSphere* BossPartsSphere::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossPartsSphere* instance = new BossPartsSphere();
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

bool BossPartsSphere::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBossPartsSphere);

	return true;
}

void BossPartsSphere::Update()
{
	ObjObject::Update();
}
