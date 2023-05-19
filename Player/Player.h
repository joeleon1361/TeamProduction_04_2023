#pragma once

#include "DirectXCommon.h"
#include "ObjObject.h"
#include "GamePlay.h"
#include "Input.h"
#include "Camera.h"
#include "DebugText.h"
#include "Vector.h"

class Player :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static Player* Create(ObjModel* model = nullptr);

private:
	static ID3D12Device* device;

	//static Camera* camera;

public: // メンバ関数
	// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

	void DebugTextUpdate();

	void DebugTextDraw();

	void Move();

	void Rolling();

	bool CheckCollisionWithBoss(XMFLOAT3 bossPos, float collisionRadius);

	void MoveTowards(float& current, float target, float speed, float elapsedTime);

	//ゲッター
	const XMFLOAT3& GetPosition() { return position; }

	const XMFLOAT3& GetVel() { return Vel; }

private: // メンバ変数
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

	XMFLOAT3 Vel = {};
	
public:
	Vector3 direction = { 0, 0, 1 };
	Vector3 moveDirection = {};
};