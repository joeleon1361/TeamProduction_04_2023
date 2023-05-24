#include "BossTurret.h"

using namespace DirectX;


BossTurret* BossTurret::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossTurret* instance = new BossTurret();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット
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

	// モデルのセット
	modelBossTurret = ObjModel::CreateFromOBJ("Turret");
	SetModel(modelBossTurret);

	// グレー
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

	// X軸を制限
	rotation.x = max(rotation.x, -limitRot);
	rotation.x = min(rotation.x, +limitRot);

	ObjObject::Update();
}