#pragma once

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include <sstream>
#include <iomanip>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Lerp.h"
#include "Boss.h"
#include "BossTurret.h"
#include "BossCore.h"
#include "BossMainCore.h"
#include "BossParts.h"
#include "BossShield.h"

class Boss;
class BossCore;
class BossMainCore;
class BossParts;
class BossTurret;
class BossShield;

class StageSelect : public BaseScene
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
		result_bg,
		black,
		triangle_up,
		triangle_down,
		stage_select,
		stage01,
		stage02,
		key_w,
		key_s,
		key_space,
	};

public:
	// コンストラクタ
	StageSelect();

	// デストラクタ
	~StageSelect();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	//デバッグテキスト用関数
	void DrawDebugText();

	//イージング関数
	XMFLOAT3 Easing(XMFLOAT3 Start, XMFLOAT3 End, float TimeRate);

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	DebugText debugText;

	Camera* camera = nullptr;

	Sprite* stageSelectBG = nullptr;

	//3Dオブジェクト
	ObjObject* BossObj_1 = nullptr;

	Boss* boss = nullptr;

	BossParts* bossPartsCoreStand = nullptr;
	BossParts* bossPartsRing = nullptr;
	BossParts* bossPartsSphere = nullptr;
	BossParts* bossPartsBody = nullptr;

	BossCore* bossCore_1 = nullptr;
	BossCore* bossCore_2 = nullptr;
	BossCore* bossCore_3 = nullptr;
	BossCore* bossCore_4 = nullptr;

	BossMainCore* bossMainCore = nullptr;

	BossParts* bossCoreBox_1 = nullptr;
	BossParts* bossCoreBox_2 = nullptr;
	BossParts* bossCoreBox_3 = nullptr;
	BossParts* bossCoreBox_4 = nullptr;

	BossParts* bossTurretStand_1 = nullptr;

	BossTurret* bossTurret_1 = nullptr;

	ObjObject* BossObj_2 = nullptr;

	Boss* boss2 = nullptr;

	BossParts* boss2PartsCoreStand = nullptr;
	BossParts* boss2PartsRing = nullptr;
	BossParts* boss2PartsSphere = nullptr;
	BossParts* boss2PartsBody = nullptr;

	BossMainCore* boss2MainCore = nullptr;

	BossParts* boss2TurretStand_1 = nullptr;

	BossTurret* boss2Turret_1 = nullptr;

	BossShield* boss2Shield = nullptr;

	ObjObject* BossObj_3 = nullptr;

	//天球
	ObjObject* Skydome = nullptr;

	//モデル
	ObjModel* BossModel_1 = nullptr;
	ObjModel* BossModel_2 = nullptr;
	ObjModel* BossModel_3 = nullptr;

	ObjModel* modelBossPartsRing = nullptr;
	ObjModel* modelBossPartsCoreBox = nullptr;
	ObjModel* modelBossPartsSphere = nullptr;
	ObjModel* modelBossPartsTurretStand = nullptr;
	ObjModel* modelBossPartsBody = nullptr;
	ObjModel* modelBossPartsCoreStand = nullptr;

	ObjModel* model_Skydome = nullptr;

	//スプライト
	Sprite* Black = nullptr;
	Sprite* SelectUI_UP = nullptr;
	Sprite* SelectUI_DOWN = nullptr;
	Sprite* Stage_Select = nullptr;
	Sprite* Key_W = nullptr;
	Sprite* Key_S = nullptr;
	Sprite* Key_SPACE = nullptr;
	Sprite* Stage01 = nullptr;
	Sprite* Stage02 = nullptr;

	XMFLOAT3 cameraPos = {};
	XMFLOAT3 cameraRot = {};

	//タイムレート
	float timeRate = 0.0f;

	//スケールレート
	float scaleRate = 0.0f;

	//カメラ移動フラグ
	bool CameraMoveFlag_U = false;
	bool CameraMoveFlag_D = false;

	//カメラシーン
	int CameraScene = 1;	//MAX3

	//カメラ目的座標
	XMFLOAT3 CamTargetPos_U = {};
	XMFLOAT3 CamTargetPos_D = {};

	//一時座標格納用変数
	XMFLOAT3 StoragePos = {};

	//ステージ選択時カメラ移動用フラグ
	bool CamForwardFlag = false;

	//黒背景のアルファ値
	float BlackAlpha = 1.0f;

	//フェードインフラグ
	bool FeedInFlag = false;

	//スプライトのDrawタイマー
	int DrawTimer = 0;

	//ボスモデル配置座標
	XMFLOAT3 BossSelectPos_1 = { 0, -100, 100 };//下端
	XMFLOAT3 BossSelectPos_2 = { 0, -50, 100 };
	XMFLOAT3 BossSelectPos_3 = { 0,  0, 100 };
	XMFLOAT3 BossSelectPos_4 = { 0,  50, 100 };
	XMFLOAT3 BossSelectPos_5 = { 0, 100, 100 };

	//ボスのスケール確保用変数
	XMFLOAT3 BossScale_1 = {};
	XMFLOAT3 BossScale_2 = {};
	XMFLOAT3 BossScale_3 = {};

	//ボスのローテーション買う補用変数
	XMFLOAT3 BossRot_1 = {};
	XMFLOAT3 BossRot_2 = {};
	XMFLOAT3 BossRot_3 = {};

	//天球のローテーション
	XMFLOAT3 RotSkydome = {};
};

