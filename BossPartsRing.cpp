#include "BossPartsRing.h"

using namespace DirectX;

BossPartsRing* BossPartsRing::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossPartsRing* instance = new BossPartsRing();
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
