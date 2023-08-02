#include "ProcessUI.h"

ProcessUI* ProcessUI::Create(XMFLOAT2 position)
{
	// �C���X�^���X�𐶐�
	ProcessUI* instance = new ProcessUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool ProcessUI::Initialize(XMFLOAT2 position)
{
	processBase = Sprite::Create(TextureNumber::process, position);
	return true;
}

void ProcessUI::Update(XMFLOAT2 position)
{
	processBase->SetPosition(position);
}

void ProcessUI::Draw()
{
	processBase->Draw();
}
