#pragma once
#include "ObjObject.h"
#include "ParticleManager.h"
#include "Lerp.h"

class BossCore :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static BossCore* Create(ObjModel* model = nullptr);

private: // �ÓI�����o�ϐ�

public: // �����o�֐�
// ������
	bool Initialize() override;

	// ���t���[������
	void Update() override;

	// �q�b�g���̃J���[�ύX
	void HitChangeColor();

	// �������̃J���[�ύX
	void AliveChangeColor();

	//�^�C�}�[���Z�b�g
	void TimerReset(int Timer, int ResetValue);

	// �l�����Z�b�g
	void VarReset();

	//�Q�b�^�[
	const int& GetLife() { return life; }
	const int& GetDestroyPartTime() { return DestroyPartTime; }
	const bool& GetAliveFlag() { return isAlive; }

	//�J���[�Q�b�^�[
	const XMFLOAT4& GetColorRed() { return Red; }
	const XMFLOAT4& GetColorYellow() { return Yellow; }
	const XMFLOAT4& GetColorOrange() { return Orange; }

private: // �����o�ϐ�
	ObjModel* modelBossPartsSphere = nullptr;

	// �ő�̗̑�
	float lifeMax = 10.0f;

	// ��{�̃J���[
	XMFLOAT4 baseColor = { 0.9f, 0.2f, 0.2f, 1.0f };
	// �q�b�g���̃J���[
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// �j���̃J���[
	XMFLOAT4 breakColor = { 0.4f, 0.2f, 0.2f, 1.0f };
	// �������̃J���[
	XMFLOAT4 aliveColor = {};

	//�{�X�p�[�c�j��p�[�e�B�N���̃J���[
	XMFLOAT4 Red = { 1.0f, 0.1f, 0.1f, 1.0f };
	XMFLOAT4 Yellow = { 0.905f, 0.870f, 0.396f, 1.0f };
	XMFLOAT4 Orange = { 0.964f, 0.470f, 0.117, 1.0f };

	//�j��p�[�e�B�N���p�^�C�}�[
	int DestroyPartTime = 20;
	//�f�t�H���g�l
	int DestroyDefault = 20;

public:
	// �����t���O
	bool isAlive = true;

	// ���݂̗̑�
	float life = lifeMax;

	// �J���[�ύX�^�C�����[�g
	float colorTimeRate = 1.0f;

	// �J���[�ύX�^�C�����[�g
	float colorTimeRate2 = 1.0f;
};

