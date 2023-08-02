#pragma once

#include "DirectXCommon.h"
#include "ObjObject.h"
#include "GamePlay.h"
#include "Input.h"
#include "Camera.h"
#include "DebugText.h"
#include "Vector.h"
#include "Easing.h"
#include "Lerp.h"

#include <cmath>

class Player :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static Player* Create(ObjModel* model = nullptr);

private:
	static ID3D12Device* device;

	//static Camera* camera;

public: // �����o�֐�
	// ������
	bool Initialize() override;

	// ���t���[������
	void Update() override;

	// �`��
	void Draw() override;

	void DebugTextUpdate();

	void DebugTextDraw();

	void Move();

	void Rolling();

	void Boost();

	void BoostTest();

	bool CheckCollisionWithBoss(XMFLOAT3 bossPos, float collisionRadius);

	void MoveTowards(float& current, float target, float speed, float elapsedTime);

	//�Q�b�^�[
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetVel() { return Vel; }

	const bool& GetBoostFlag() { return BoostFlag; }

	const XMFLOAT4& GetBoostPartColor() { return BoostPartColor; }

	const float& GetBoostPowNow() { return BoostPowNow; }

	const float& GetBoostPowMax() { return BoostPowMax; }

	const float& GetTotalSpeed() { return totalSpeed; }

	const float& GetNormalShootSpeedMax() { return normalShootSpeedMax; }

	const float& GetChargeShootSpeedMax() { return chargeShootSpeedMax; }

	const float& GetShootSpeedMin() { return shootSpeedMin; }

	const float& GetTotalSpeedMax() { return baseSpeed + boostSpeedMax; }

private: // �����o�ϐ�
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	DebugText debugText;

	ObjModel* modelPlayer = nullptr;

	float rotationSpeed = 30.0f;
	float speed = 0.25f;

	XMFLOAT3 playerSpeed = {};

	XMFLOAT3 rollRotation = {};
	XMFLOAT3 axis = { 0.0f, 0.0f, 0.0f };
	float xAngle = 90.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float hypotenuse = 0.0f;
	float radians = 0.0f;
	float radians2 = 0.0f;
	float degrees = 90.0f;
	float degrees2 = 0.0f;
	float yChange = 0.0f;
	float collisionSphereRadius = 3.0f;

	//�v���C���[�X�s�[�h
	float totalSpeed = 2.0f;

	XMFLOAT3 Vel = {};
	float yVel = 0.0f;

	//�d�͉����x
	float Gravity = 9.8f;

	// �v�\�X�g�̗͂̍ő�l
	float BoostPowMax = 100.0f;

	//�u�[�X�g�̗�
	float BoostPowNow = BoostPowMax;

	//�u�[�X�g�t���O
	bool BoostFlag = false;

	//�u�[�X�g�p�[�e�B�N���J���[
	XMFLOAT4 BoostPartColor = {};

	// �u�[�X�g���̑��x
	float boostSpeedMax = 1.0f;
	float boostSpeedMin = 0.0f;
	float boostSpeed = boostSpeedMin;
	// �u�[�X�g���x�̐�����ԃ��[�g
	float boostTimeRate = 0.0f;

	// �ʏ펞�̑��x
	float baseSpeed = 2.0f;

	// �e���ˎ��̑��x
	float normalShootSpeedMax = 1.0f;
	float chargeShootSpeedMax = 2.0f;
	float shootSpeedMin = 0.0f;

public:
	Vector3 direction = { 0, 0, 1 };
	Vector3 moveDirection = {};

	XMFLOAT3 prevPos = { 0.0f, 0.0f, 0.0f };

	// �e���ˎ��̑��x
	float shootSpeed = 0.0f;
	float shootSpeedTimeRate = 0.0f;

	float HPMAX = 15.0f;
	float HP = HPMAX;

	bool boostCancel = false;
	//float boostCancelTimer = 
};