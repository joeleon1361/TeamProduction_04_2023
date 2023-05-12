#include "BossCore.h"

using namespace DirectX;

BossCore* BossCore::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossCore* instance = new BossCore();
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

bool BossCore::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	// モデルのセット
	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	SetModel(modelBossPartsSphere);

	// 生存フラグの初期化
	isAlive = true;

	// 体力の初期化
	life = lifeMax;

	// カラー変更タイムレートの初期化
	colorTimeRate = 1.0f;

	//破壊タイマーのリセット
	DestroyPartTime = DestroyDefault;

	return true;
}

void BossCore::Update()
{
	ObjObject::Update();

	// ヒット時のカラー変更
	HitChangeColor();

	// HPが0になったら撃破
	if (life <= 0.0f)
	{
		if (DestroyPartTime > 0)
		{
			DestroyPartTime--;
		}

		isAlive = false;
	}

}

// ヒット時のカラー変更
void BossCore::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);
}

void BossCore::TimerReset(int Timer, int ResetValue)
{
	Timer = ResetValue;
}
