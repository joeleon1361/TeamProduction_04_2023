#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "ObjectParticle.h"
#include "Sound.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Test.h"

class Title : public BaseScene
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
		title_bg,
		titlefont,
		pressspace,
		black
	};

public:
	// コンストクラタ
	Title();

	// デストラクタ
	~Title();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;
	
	//パーティクル生成
	void CreateParticle(XMFLOAT3 pos);

	int CountTimer(int Time);


private: // メンバ変数
	//DirectXCommon
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//入力
	Input* input = Input::GetInstance();

	//音声
	Sound* sound = Sound::GetInstance();

	//カメラ
	Camera* camera = nullptr;

	//スプライト
	Sprite* titleBG = nullptr;
	Sprite* titleBG_LD = nullptr;
	Sprite* titleBG_RU = nullptr;
	Sprite* titleBG_RD = nullptr;
	Sprite* TitleFont = nullptr;
	Sprite* PressSpace = nullptr;
	Sprite* Black = nullptr;


	//3Dオブジェクト
	ObjObject* objTitleFont = nullptr;

	std::unique_ptr<ObjectParticle> Object;

	std::list<std::unique_ptr<ObjectParticle>> ObjPart;

	std::unique_ptr<ObjectParticle> TestPart;

	//モデル
	ObjModel* modelTitleFont = nullptr;

	ObjModel* modelObject = nullptr;

	//パーティクル
	ParticleManager* Particle = nullptr;

	//オブジェクトパーティクル
	std::list<std::unique_ptr<ObjectParticle>> particle;


	//テスト
	Test* circleParticle = nullptr;

	//タイトルスプライト座標
	XMFLOAT2 TitlePos_LU = {0.0f, 0.0f};
	XMFLOAT2 TitlePos_LD = { 0.0f, 720.0f };
	XMFLOAT2 TitlePos_RU = { 1280.0f, 0.0f };
	XMFLOAT2 TitlePos_RD = {1280.0f, 720.0f};

	float Speed_y = 1.8f;

	//黒背景のアルファ値
	float BlackAlpha = 0.0f;

	//遷移フラグ
	bool StartFlag = false;
	
	//描画タイマー
	int DrawTimer = 0;

	XMFLOAT3 Vector = {};

	//デフォルトポイント
	XMFLOAT3 DefaultPos = { 0.0f, 0.0f, 0.0f };

	//カウントタイマー
	int CountTime = 0;

	//パーティクル発生フラグ
	bool PartFlag = false;

	//タイトル表示フラグ
	bool TitleFlag = true;

	//ディレイタイマーとフラグ
	bool DelayFlag = false;
	int DelayTimer = 0;

};