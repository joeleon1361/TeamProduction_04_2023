#include "BossParts.h"

BossParts* BossParts::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossParts* instance = new BossParts();
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

bool BossParts::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void BossParts::Update()
{
	ObjObject::Update();
}
