#include "BossPartsCoreBox.h"

BossPartsCoreBox* BossPartsCoreBox::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossPartsCoreBox* instance = new BossPartsCoreBox();
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

bool BossPartsCoreBox::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// ���f���̃Z�b�g
	modelBossPartsCoreBox = ObjModel::CreateFromOBJ("bossPartsCoreBox");
	SetModel(modelBossPartsCoreBox);

	return true;
}

void BossPartsCoreBox::Update()
{
	ObjObject::Update();
}
