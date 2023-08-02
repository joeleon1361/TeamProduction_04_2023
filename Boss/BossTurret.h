#pragma once
#include "ObjObject.h"
#include "Lerp.h"

class BossTurret :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static BossTurret* Create(ObjModel* model = nullptr);

private: // �ÓI�����o�ϐ�

public: // �����o�֐�
// ������
	bool Initialize();

	// ���t���[������
	void Update(XMFLOAT3 target_position);

	// �q�b�g���̃J���[�ύX
	void HitChangeColor();

	const float& GetExternRotationY() { return externRotationY; }

private: // �����o�ϐ�
	Input* input = Input::GetInstance();

	ObjModel* modelBossTurret = nullptr;

	float limitRot = 30.0f;

	// �ő�̗̑�
	float lifeMax = 10.0f;

	// ��{�̃J���[
	XMFLOAT4 baseColor = { 0.3f, 0.3f, 0.3f, 1.0f };
	// �q�b�g���̃J���[
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// ���j���̊p�x
	XMFLOAT3 breakRotation = { 30.0f, 0.0f, 0.0f };

	float externRotationY = 0.0f;

public:
	// �����t���O
	bool isAlive = true;

	// ���݂̗̑�
	float life = lifeMax;

	bool isShot = false;

	// �J���[�ύX�^�C�����[�g
	float colorTimeRate = 1.0f;

	// �p�x�ύX�^�C�����[�g
	float rotationTimeRate = 0.0f;

	// �e�̔��ˊԊu
	static const int32_t ShotInterval = 60;

	// ���˃^�C�}�[
	int32_t shotTimer = 240;
};

