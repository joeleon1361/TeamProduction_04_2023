#include "ProcessUI.h"

ProcessUI* ProcessUI::Create(XMFLOAT2 position, int type)
{
	// �C���X�^���X�𐶐�
	ProcessUI* instance = new ProcessUI();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, type)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool ProcessUI::Initialize(XMFLOAT2 position, int type)
{
	processBase = Sprite::Create(TextureNumber::process, position);
	breakCore = Sprite::Create(TextureNumber::breakcore, { position.x + 201.0f , position.y + 32.0f });
	breakMainCore = Sprite::Create(TextureNumber::breakmaincore, { position.x + 201.0f , position.y + 32.0f });
	breakShield = Sprite::Create(TextureNumber::breakshield, { position.x + 201.0f , position.y + 32.0f });

	breakCore->SetAnchorPoint({0.5f, 0.5f});
	breakMainCore->SetAnchorPoint({ 0.5f, 0.5f });
	breakShield->SetAnchorPoint({ 0.5f, 0.5f });

	this->type = type;

	return true;
}

void ProcessUI::Update(XMFLOAT2 position)
{
	processBase->SetPosition(position);
}

void ProcessUI::Draw()
{
	processBase->Draw();
	if (type == 0)
	{
		breakCore->Draw();
	}
	else if (type == 1)
	{
		breakShield->Draw();
	}
	else if (type == 2)
	{
		breakMainCore->Draw();
	}
}
