#pragma once
#include "ObjObject.h"
#include <math.h>
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
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static std::unique_ptr<TargetBullet>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed);

private: // �ÓI�����o�ϐ�
// ������܂ł̎���
	static const int32_t LifeTime = 30;

public: // �����o�֐�
// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed);

	// ���t���[������
	void Update();

	const bool GetDeathFlag() { return deathFlag; }

private: // �����o�ϐ�
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// �f�X�^�C�}�[ 
	int32_t deathTimer = LifeTime;

public:
	// �f�X�t���O
	bool deathFlag = false;
};

