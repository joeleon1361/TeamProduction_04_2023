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
#include "ObjModel.h"
#include "FbxLoader.h"
#include "FbxObject.h"

#include "Player.h"
#include "Bullet.h"
#include "Boss.h"
#include "TargetBullet.h"

#include "Spline.h"
#include "Lerp.h"
#include "Easing.h"

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
		game_bg
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

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	DebugText debugText;
	
	Camera* camera = nullptr;

	Sprite* gameBG = nullptr;
	
	ObjModel* modelPlayer = nullptr;
	ObjModel* modelSkydome = nullptr;

	Player* player = nullptr;
	ObjObject* objSkydome = nullptr;

	std::list<std::unique_ptr<Bullet>> playerBullets;
};