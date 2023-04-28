#include "BossPartsRing.h"

using namespace DirectX;

BossPartsRing* BossPartsRing::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossPartsRing* instance = new BossPartsRing();
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

bool BossPartsRing::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void BossPartsRing::Update()
{
	ObjObject::Update();
}
