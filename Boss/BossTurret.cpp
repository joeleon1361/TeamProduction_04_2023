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

	// �����t���O�̏�����
	isAlive = true;

	// �̗͂̏�����
	life = lifeMax;

	// �J���[�ύX�^�C�����[�g�̏�����
	colorTimeRate = 1.0f;

	return true;
}

void BossTurret::Update()
{
	ObjObject::Update();

	// �q�b�g���̃J���[�ύX
	HitChangeColor();

	if (input->PushKey(DIK_UP) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x -= 1.0f;
	}
	if (input->PushKey(DIK_DOWN) && input->PushKey(DIK_LCONTROL) == 0)
	{
		rotation.x += 1.0f;
	}

	// HP��0�ɂȂ����猂�j
	if (life <= 0.0f)
	{
		isAlive = false;
	}

	// X���𐧌�
	rotation.x = max(rotation.x, -limitRot);
	rotation.x = min(rotation.x, +limitRot);
}

void BossTurret::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);
}
