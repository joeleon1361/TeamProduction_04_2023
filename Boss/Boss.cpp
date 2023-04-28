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

	// ���f���̃Z�b�g
	modelBoss = ObjModel::CreateFromOBJ("bullet2");
	SetModel(modelBoss);

	return true;
}

void Boss::Update()
{
	ObjObject::Update();

	if (input->PushKey(DIK_UP))
	{
		rotation.x += 1.0f;
	}
	if (input->PushKey(DIK_DOWN))
	{
		rotation.x -= 1.0f;
	}
	if (input->PushKey(DIK_RIGHT))
	{
		rotation.y -= 1.0f;
	}
	if (input->PushKey(DIK_LEFT))
	{
		rotation.y += 1.0f;
	}
}
