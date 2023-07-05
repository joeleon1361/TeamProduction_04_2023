#include "DeltaGageUI.h"

DeltaGageUI* DeltaGageUI::Create(XMFLOAT2 position, XMFLOAT2 size)
{
	// �C���X�^���X�𐶐�
	DeltaGageUI* instance = new DeltaGageUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, size)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool DeltaGageUI::Initialize(XMFLOAT2 position, XMFLOAT2 size)
{
	// UI�̃x�[�X
	gageBase = Sprite::Create(TextureNumber::game_boss_frame_1, { position.x + 10.0f, position.y });
	gageBase->SetAnchorPoint({ 1.0f, 0.5f });

	// �����Q�[�W
	gageDelta = Sprite::Create(TextureNumber::game_boss_gauge, position);
	gageDelta->SetColor({ 1.0f, 0, 0.2f, 1.0f });
	gageDelta->SetSize(size);
	gageDelta->SetAnchorPoint({ 1.0f, 0.5f });

	// ���C���Q�[�W
	gageMain = Sprite::Create(TextureNumber::game_boss_gauge, position);
	gageMain->SetColor({ 0.1f, 0.6f, 0.1f, 1.0f });
	gageMain->SetSize(size);
	gageMain->SetAnchorPoint({ 1.0f, 0.5f });

	// UI�̃t���[��
	gageCover = Sprite::Create(TextureNumber::game_boss_frame_2, { position.x + 10.0f, position.y });
	gageCover->SetAnchorPoint({ 1.0f, 0.5f });

	maxSize = size;

	return true;
}

void DeltaGageUI::Update(float nowVal, float maxVal, XMFLOAT2 position)
{
	// �Q�[�W�ɓ����l�̊������v�Z
	ratio = nowVal / maxVal;
	nowSize.x = ratio * maxSize.x;

	// �����Q�[�W�̌v�Z
	delatSize = Lerp::LerpFloat2(gageDelta->GetSize(), nowSize, 0.1f);

	// ���W���Z�b�g
	gageBase->SetPosition({ position.x + 10.0f, position.y });
	gageDelta->SetPosition(position);
	gageMain->SetPosition(position);
	gageCover->SetPosition({ position.x + 10.0f, position.y });

	// �T�C�Y���Z�b�g
	gageMain->SetSize(nowSize);
	gageDelta->SetSize(delatSize);
}

void DeltaGageUI::Draw()
{
	gageBase->Draw();
	gageDelta->Draw();
	gageMain->Draw();
	gageCover->Draw();
}
