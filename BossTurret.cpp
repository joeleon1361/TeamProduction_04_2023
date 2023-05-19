#include "BossTurret.h"

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

	SetColor({ 0.2f, 0.9f, 0.2f, 1.0f });

	return true;
}

void BossTurret::Update()
{
	ObjObject::Update();
}
