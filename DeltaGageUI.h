#pragma once

#include "Sprite.h"
#include "Lerp.h"

class DeltaGageUI
{
private: // �G�C���A�X
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
	static DeltaGageUI* Create(XMFLOAT2 position, XMFLOAT2 size);

	bool Initialize(XMFLOAT2 position, XMFLOAT2 size);

	void Update(float nowVal, float maxVal, XMFLOAT2 position);

	void Draw();

private:
	Sprite* gageBase = nullptr;
	Sprite* gageMain = nullptr;
	Sprite* gageCover = nullptr;
	Sprite* gageDelta = nullptr;

	XMFLOAT2 nowSize = { 530.0f, 30.0f };
	XMFLOAT2 maxSize = {};
	XMFLOAT2 delatSize = {};

	float ratio = {};
};

