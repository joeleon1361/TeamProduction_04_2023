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
	modelBoss = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBoss);

	SetColor({ 0.9f, 0.2f, 0.2f, 1.0f });

	return true;
}

void Boss::Update()
{
	ObjObject::Update();

	// ��](���f���m�F�p)
	/*if (input->PushKey(DIK_UP) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x += 1.0f;
	}
	if (input->PushKey(DIK_DOWN) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x -= 1.0f;
	}*/
	if (input->PushKey(DIK_RIGHT) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.y -= 1.0f;
	}
	if (input->PushKey(DIK_LEFT) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.y += 1.0f;
	}

	// �ړ�(���f���m�F�p)
	if (input->PushKey(DIK_UP) && input->PushKey(DIK_LCONTROL))
	{
		position.y += 1.0f;
	}
	if (input->PushKey(DIK_DOWN) && input->PushKey(DIK_LCONTROL))
	{
		position.y -= 1.0f;
	}
	if (input->PushKey(DIK_RIGHT) && input->PushKey(DIK_LCONTROL))
	{
		position.x += 1.0f;
	}
	if (input->PushKey(DIK_LEFT) && input->PushKey(DIK_LCONTROL))
	{
		position.x -= 1.0f;
	}
}
