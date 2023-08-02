#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "ObjectParticle.h"
#include "Sound.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Test.h"

class Title : public BaseScene
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
		title_bg,
		titlefont,
		pressspace,
		black
	};

public:
	// �R���X�g�N���^
	Title();

	// �f�X�g���N�^
	~Title();

	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;
	
	//�p�[�e�B�N������
	void CreateParticle(XMFLOAT3 pos);

	int CountTimer(int Time);


private: // �����o�ϐ�
	//DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//����
	Input* input = Input::GetInstance();

	//����
	Sound* sound = Sound::GetInstance();

	//�J����
	Camera* camera = nullptr;

	//�X�v���C�g
	Sprite* titleBG = nullptr;
	Sprite* titleBG_LD = nullptr;
	Sprite* titleBG_RU = nullptr;
	Sprite* titleBG_RD = nullptr;
	Sprite* TitleFont = nullptr;
	Sprite* PressSpace = nullptr;
	Sprite* Black = nullptr;


	//3D�I�u�W�F�N�g
	ObjObject* objTitleFont = nullptr;

	std::unique_ptr<ObjectParticle> Object;

	std::list<std::unique_ptr<ObjectParticle>> ObjPart;

	std::unique_ptr<ObjectParticle> TestPart;

	//���f��
	ObjModel* modelTitleFont = nullptr;

	ObjModel* modelObject = nullptr;

	//�p�[�e�B�N��
	ParticleManager* Particle = nullptr;

	//�I�u�W�F�N�g�p�[�e�B�N��
	std::list<std::unique_ptr<ObjectParticle>> particle;


	//�e�X�g
	Test* circleParticle = nullptr;

	//�^�C�g���X�v���C�g���W
	XMFLOAT2 TitlePos_LU = {0.0f, 0.0f};
	XMFLOAT2 TitlePos_LD = { 0.0f, 720.0f };
	XMFLOAT2 TitlePos_RU = { 1280.0f, 0.0f };
	XMFLOAT2 TitlePos_RD = {1280.0f, 720.0f};

	float Speed_y = 1.8f;

	//���w�i�̃A���t�@�l
	float BlackAlpha = 0.0f;

	//�J�ڃt���O
	bool StartFlag = false;
	
	//�`��^�C�}�[
	int DrawTimer = 0;

	XMFLOAT3 Vector = {};

	//�f�t�H���g�|�C���g
	XMFLOAT3 DefaultPos = { 0.0f, 0.0f, 0.0f };

	//�J�E���g�^�C�}�[
	int CountTime = 0;

	//�p�[�e�B�N�������t���O
	bool PartFlag = false;

	//�^�C�g���\���t���O
	bool TitleFlag = true;

	//�f�B���C�^�C�}�[�ƃt���O
	bool DelayFlag = false;
	int DelayTimer = 0;

};