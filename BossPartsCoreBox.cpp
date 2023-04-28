#include "BossPartsCoreBox.h"

BossPartsCoreBox* BossPartsCoreBox::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossPartsCoreBox* instance = new BossPartsCoreBox();
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

bool BossPartsCoreBox::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// モデルのセット
	modelBossPartsCoreBox = ObjModel::CreateFromOBJ("bossPartsCoreBox");
	SetModel(modelBossPartsCoreBox);

	return true;
}

void BossPartsCoreBox::Update()
{
	ObjObject::Update();
}
