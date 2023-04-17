#include "Boss.h"
#include "Input.h"

using namespace DirectX;

Boss* Boss::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Boss* instance = new Boss();
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

bool Boss::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void Boss::Update()
{
	ObjObject::Update();
}
