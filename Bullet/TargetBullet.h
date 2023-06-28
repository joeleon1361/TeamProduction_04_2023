#pragma once
#include "ObjObject.h"
#include "WinApp.h"
#include <DirectXMath.h>

class TargetBullet :
	public ObjObject
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static std::unique_ptr<TargetBullet>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed, XMFLOAT3 eye, XMFLOAT3 aimTarget, XMFLOAT3 up);

private: // 静的メンバ変数
// 消えるまでの時間
	static const int32_t LifeTime = 150;

public: // メンバ関数
// 初期化
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed,
		XMFLOAT3 eye, XMFLOAT3 aimTarget, XMFLOAT3 up);

	// 毎フレーム処理
	void Update();

	const bool GetDeathFlag() { return deathFlag; }

private: // メンバ変数
	XMFLOAT3 velocity = { 0,0,0 };

	XMFLOAT3 target = { 0,0,0 };

	float speed = 0.0f;

	// デスタイマー 
	int32_t deathTimer = LifeTime;

	int width = 0;
	int height = 0;

	POINT cursorPos;

public:
	// デスフラグ
	bool deathFlag = false;
	XMFLOAT3 eyePosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 targetPosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 upVector = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 bulletDirection = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 prevPosition = { 0.0f, 0.0f, 0.0f };
};

