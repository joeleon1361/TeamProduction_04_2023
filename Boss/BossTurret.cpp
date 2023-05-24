#include "BossTurret.h"

using namespace DirectX;


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

	// �O���[
	SetColor({ 0.3f, 0.3f, 0.3f, 1.0f });

	return true;
}

void BossTurret::Update()
{
	if (input->PushKey(DIK_UP) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x -= 1.0f;
	}
	if (input->PushKey(DIK_DOWN) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x += 1.0f;
	}

	// X���𐧌�
	rotation.x = max(rotation.x, -limitRot);
	rotation.x = min(rotation.x, +limitRot);

	ObjObject::Update();
}