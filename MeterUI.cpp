#include "MeterUI.h"

MeterUI* MeterUI::Create(XMFLOAT2 position, float rotation, XMFLOAT4 color)
{
	// �C���X�^���X�𐶐�
	MeterUI* instance = new MeterUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, rotation, color)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool MeterUI::Initialize(XMFLOAT2 position, float rotation, XMFLOAT4 color)
{
	//meterBase = 
	meterNeedle = Sprite::Create(TextureNumber::speed, position);

	meterNeedle->SetRotation(rotation);
	meterNeedle->SetColor(color);
	meterNeedle->SetAnchorPoint({ 0.9f, 0.5 });

	return true;
}

void MeterUI::Update(float nowVal, float maxVal, XMFLOAT2 position)
{
	// ���[�^�[�ɓ����l�̊������v�Z
	ratio = nowVal / maxVal;
	nowRotation = ratio * 180.0f;

	meterNeedle->SetRotation(nowRotation);
}

void MeterUI::Draw()
{
	meterNeedle->Draw();
}
