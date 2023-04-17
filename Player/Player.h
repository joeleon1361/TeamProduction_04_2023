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

	static Camera* camera;

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

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	DebugText debugText;

	Vector3 direction = { 0, 0, 1 };
	Vector3 moveDirection = {};
	float rotSpeed = 22.5f;
	float speed = 1.0f;

	XMFLOAT3 playerSpeed = {};

	XMFLOAT3 rollRotation = {};
};

