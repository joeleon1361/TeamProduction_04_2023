#pragma once
#include "ObjObject.h"
#include "Lerp.h"

class ReflectBullet :
	public ObjObject
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<ReflectBullet>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed);

private: // �ÓI�����o�ϐ�
	// ������܂ł̎���
	static const int32_t LifeTime = 360;

public: // �����o�֐�
	// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed);

	// ���t���[������
	void Update(const XMFLOAT3 reflect_position);

	// �q�b�g���̃J���[�ύX
	void HitChangeColor();

	const bool GetDeathFlag() { return deathFlag; }

	void RallyReset(const XMFLOAT3 position, const XMFLOAT3 target, const float speed, const float life);

private: // �����o�ϐ�
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// �ő�̗̑�
	float lifeMax = 7.0f;

	// ��{�̃J���[
	XMFLOAT4 baseColor = { 0.9f, 0.5f, 0.2f, 1.0f };
	// �q�b�g���̃J���[
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// �f�X�^�C�}�[ 
	int32_t deathTimer = LifeTime;

public:
	// �f�X�t���O
	bool deathFlag = false;

	bool isReflect = false;

	int rallyCount = 1;

	// ���݂̗̑�
	float life = lifeMax;

	// �J���[�ύX�^�C�����[�g
	float colorTimeRate = 1.0f;

	XMFLOAT3 prevPosition = { 0.0f, 0.0f, 0.0f };
};

