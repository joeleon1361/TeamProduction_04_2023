#include "BossPartsSphere.h"

BossPartsSphere* BossPartsSphere::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossPartsSphere* instance = new BossPartsSphere();
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

bool BossPartsSphere::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// モデルのセット
	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBossPartsSphere);

	return true;
}

void BossPartsSphere::Update()
{
	ObjObject::Update();
}
