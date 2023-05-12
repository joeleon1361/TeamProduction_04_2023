﻿#pragma once

#include "BaseScene.h"

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Sound.h"
#include "FbxObject.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject.h"
#include "WinApp.h"

#include "Player.h"
#include "Bullet.h"
#include "Boss.h"
#include "BossPartsRing.h"
#include "BossPartsSphere.h"
#include "BossCore.h"
#include "BossPartsCoreBox.h"
#include "TargetBullet.h"

#include "Spline.h"
#include "Lerp.h"
#include "Easing.h"
#include "Transform.h"

#include "SceneManager.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>
#include <array>

class CollisionManager;
class Player;
class Bullet;
class PlayerSpecialBullet;
class Boss;
class BossBullet;
class TargetBullet;
class StageObject;
class BossPartsRing;
class BossPartsSphere;
class BossCore;
class BossPartsCoreBox;

// ゲームシーン
class GamePlay : public BaseScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	// スプライトのテクスチャ番号
	static enum TextureNumber
	{
		debug_txt,
		game_bg,
		reticle
	};

public: // メンバ関数

	// コンストクラタ
	GamePlay();

	// デストラクタ
	~GamePlay();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

	//---パーティクル関数---//
	//基本パーティクル
	void DefaultParticle(XMFLOAT3 position);

	void CreateBossHitParticles(XMFLOAT3 position);

	//マウス情報取得
	void GetMouse();

	//デバッグテキスト用関数
	void DrawDebugText();

	void Shoot();

	bool BasicCollisionDetection(XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize);

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	DebugText debugText;
	
	Camera* camera = nullptr;

	ParticleManager* bossHitParticle = nullptr;

	Sprite* gameBG = nullptr;
	Sprite* Reticle = nullptr;
	Sprite* test = nullptr;
	
	ObjModel* modelSkydome = nullptr;
	ObjModel* modelBullet = nullptr;

	Player* player = nullptr;
	Boss* boss = nullptr;
	BossPartsRing* bossPartsRing = nullptr;
	BossPartsSphere* bossPartsSphere = nullptr;
	BossCore* bossCore_1 = nullptr;
	BossCore* bossCore_2 = nullptr;
	BossCore* bossCore_3 = nullptr;
	BossCore* bossCore_4 = nullptr;
	BossPartsCoreBox* bossCoreBox_1 = nullptr;
	BossPartsCoreBox* bossCoreBox_2 = nullptr;
	BossPartsCoreBox* bossCoreBox_3 = nullptr;
	BossPartsCoreBox* bossCoreBox_4 = nullptr;
	ObjObject* objSkydome = nullptr;

	std::list<std::unique_ptr<TargetBullet>> playerBullets;

	float shotRate = 0.0f;
	bool shotFlag = true;

	//レティクル座標
	XMFLOAT2 ReticlePos = { 0.0f, 0.0f };

	//マウス座標
	POINT mousePosition;

	XMFLOAT3 worldPos;
	XMFLOAT3 worldPos2;
	XMFLOAT3 worldPos3;
	XMFLOAT3 worldPos4;
};