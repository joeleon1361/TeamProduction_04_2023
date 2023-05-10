#pragma once
#include "ObjObject.h"
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

private: // �����o�ϐ�
	ObjModel* modelBossPartsSphere = nullptr;

	// �ő�̗̑�
	float lifeMax = 10.0f;

	// ��{�̃J���[
	XMFLOAT4 baseColor = { 0.9f, 0.2f, 0.2f, 1.0f };
	// �q�b�g���̃J���[
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	// �����t���O
	bool isAlive = true;

	// ���݂̗̑�
	float life = lifeMax;

	// �J���[�ύX�^�C�����[�g
	float colorTimeRate = 1.0f;
};

