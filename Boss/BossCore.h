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

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static BossCore* Create(ObjModel* model = nullptr);

private: // 静的メンバ変数

public: // メンバ関数
// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// ヒット時のカラー変更
	void HitChangeColor();

private: // メンバ変数
	ObjModel* modelBossPartsSphere = nullptr;

	// 最大の体力
	float lifeMax = 10.0f;

	// 基本のカラー
	XMFLOAT4 baseColor = { 0.9f, 0.2f, 0.2f, 1.0f };
	// ヒット時のカラー
	XMFLOAT4 hitColor = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	// 生存フラグ
	bool isAlive = true;

	// 現在の体力
	float life = lifeMax;

	// カラー変更タイムレート
	float colorTimeRate = 1.0f;
};

