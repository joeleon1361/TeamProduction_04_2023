#pragma once

#include "Sprite.h"
#include "Lerp.h"
#include "Easing.h"

class ProcessUI
{
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
// �X�v���C�g�̃e�N�X�`���ԍ�
	static enum TextureNumber
	{
		debug_txt,
		game_bg,
		reticle,
		black,
		rule,
		speed,
		meter,
		process,
		breakshield,
		breakmaincore,
		breakcore,

		// �{�X
		game_boss_frame_1,
		game_boss_frame_2,
		game_boss_gauge
	};

public: // �ÓI�����o�֐�
// �C���X�^���X����
	static ProcessUI* Create(XMFLOAT2 position, int type);

	bool Initialize(XMFLOAT2 position, int type);

	void Update(XMFLOAT2 position);

	void Draw();

private:
	Sprite* processBase = nullptr; 
	Sprite* breakCore = nullptr;
	Sprite* breakMainCore = nullptr;
	Sprite* breakShield = nullptr;

	int type = 1;
};

