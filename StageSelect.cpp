#include "StageSelect.h"

StageSelect::StageSelect()
{
}

StageSelect::~StageSelect()
{
}

void StageSelect::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::result_bg, L"Resources/Sprite/ResultUI/result_bg.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	stageSelectBG = Sprite::Create(TextureNumber::result_bg, { 0.0f,0.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });
}

void StageSelect::Finalize()
{
}

void StageSelect::Update()
{
	if (input->TriggerKey(DIK_1))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	else if (input->TriggerKey(DIK_2))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");
	}
	else if (input->TriggerKey(DIK_3))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("THIRDSTAGE");
	}

	camera->Update({ 0, 0, 10 }, { 0, 0, 0 });

	DrawDebugText();
}

void StageSelect::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 前景スプライト描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void StageSelect::DrawDebugText()
{
	std::ostringstream StageSelect;
	StageSelect << "StageSelect 1:FirstStage 2:SecondStage 3:ThirdStage";
	debugText.Print(StageSelect.str(), 10, 10, 2.0f);
}
