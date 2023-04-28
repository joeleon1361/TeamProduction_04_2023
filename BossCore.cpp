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

	return true;
}

void BossCore::Update()
{
	ObjObject::Update();
}
