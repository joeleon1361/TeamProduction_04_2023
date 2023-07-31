#pragma once
#include "ObjObject.h"
#include "WinApp.h"
#include <DirectXMath.h>

class TargetBullet :
	public ObjObject
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static std::unique_ptr<TargetBullet>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed, XMFLOAT3 eye, XMFLOAT3 aimTarget, XMFLOAT3 up, XMFLOAT3 near_point);

private: // �ÓI�����o�ϐ�
// ������܂ł̎���
	static const int32_t LifeTime = 150;

public: // �����o�֐�
// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed,
		XMFLOAT3 eye, XMFLOAT3 aimTarget, XMFLOAT3 up, XMFLOAT3 near_point);

	// ���t���[������
	void Update(XMFLOAT3 prevPos, XMFLOAT3 currentPos, XMMATRIX viewMat, XMFLOAT3 playerRot);

	const bool GetDeathFlag() { return deathFlag; }

private: // �����o�ϐ�
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// �f�X�^�C�}�[ 
	int32_t deathTimer = LifeTime;

	int width = 0;
	int height = 0;

	float degrees = 0.0f;
	float diff = 0.0f;
	bool diffSet = false;

	int stage = 0;

	XMVECTOR permFarPoint;
	XMVECTOR permNearPoint;

	POINT cursorPos;

public:
	// �f�X�t���O
	bool deathFlag = false;
	XMFLOAT3 eyePosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 targetPosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 upVector = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 bulletDirection = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 prevPosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 futurePosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 nullVel = { 0.0f, 0.0f, 0.0f };

	int testing;
};

