#include "BossTurret.h"

BossTurret* BossTurret::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossTurret* instance = new BossTurret();
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

bool BossTurret::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelBossTurret = ObjModel::CreateFromOBJ("Turret");
	SetModel(modelBossTurret);

	SetColor({ 0.2f, 0.9f, 0.2f, 1.0f });

	return true;
}

void BossTurret::Update()
{
	ObjObject::Update();
}
