#pragma once
#include "DirectXCommon.h"

class SceneManager;

class BaseScene
{
public: // メンバ関数
	virtual ~BaseScene() = default;

	//初期化
	virtual void Initialize() = 0;

	//終了処理
	virtual void Finalize() = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;
};