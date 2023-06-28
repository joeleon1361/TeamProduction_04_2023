#include "GageUI.h"

GageUI* GageUI::Create(XMFLOAT2 position, XMFLOAT2 size)
{
	// インスタンスを生成
	GageUI* instance = new GageUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, size)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool GageUI::Initialize(XMFLOAT2 position, XMFLOAT2 size)
{
	// UIのベース
	gageBase = Sprite::Create(TextureNumber::game_boss_frame_1, { position.x + 10.0f, position.y });
	gageMain = Sprite::Create(TextureNumber::game_boss_gauge, position);
	gageCover = Sprite::Create(TextureNumber::game_boss_frame_2, { position.x + 10.0f, position.y });

	// 差分ゲージ
	gageMain->SetColor({ 0.6f, 0.6f, 0.1f, 1.0f });
	gageMain->SetSize(size);
	gageMain->SetAnchorPoint({ 1.0f, 0.5f });

	// メインゲージ
	gageBase->SetAnchorPoint({ 1.0f, 0.5f });

	// UIのフレーム
	gageCover->SetAnchorPoint({ 1.0f, 0.5f });

	maxSize = size;

	return true;
}

void GageUI::Update(float nowVal, float maxVal)
{
	// ゲージに入れる値の割合を計算
	ratio = nowVal / maxVal;
	nowSize.x = ratio * maxSize.x;

	// サイズをセット
	gageMain->SetSize(nowSize);
}

void GageUI::Draw()
{
	gageBase->Draw();
	gageMain->Draw();
	gageCover->Draw();
}
