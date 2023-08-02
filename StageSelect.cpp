#include "StageSelect.h"

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
// DirectX::を省略
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

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

	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::triangle_up, L"Resources/Sprite/TitleUI/Triangle_UP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::triangle_down, L"Resources/Sprite/TitleUI/Triangle_DOWN.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::stage_select, L"Resources/Sprite/TitleUI/StageSelect.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::stage01, L"Resources/Sprite/TitleUI/STAGE_01.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::stage02, L"Resources/Sprite/TitleUI/STAGE_02.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::key_s, L"Resources/Sprite/TitleUI/Key_S.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::key_w, L"Resources/Sprite/TitleUI/Key_W.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::key_space, L"Resources/Sprite/TitleUI/Key_SPACE.png")) {
		assert(0);
		return;
	}

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");

	Black = Sprite::Create(TextureNumber::black, { 0,0.0f });
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	SelectUI_UP = Sprite::Create(TextureNumber::triangle_up, { 0.0f, 0.0f });
	SelectUI_DOWN = Sprite::Create(TextureNumber::triangle_down, { 0.0f, 0.0f });

	SelectUI_UP->SetPosition({ 950, 250});
	SelectUI_DOWN->SetPosition({ 950, 400 });

	Stage_Select = Sprite::Create(TextureNumber::stage_select, { 0.0f, 0.0f });
	Stage_Select->SetPosition({ 20, 20 });

	Stage01 = Sprite::Create(TextureNumber::stage01, { 0.0f, 0.0f });
	Stage02 = Sprite::Create(TextureNumber::stage02, { 0.0f, 0.0f });

	Stage01->SetPosition({ 550, 500 });
	Stage02->SetPosition({ 550, 500 });

	Key_W = Sprite::Create(TextureNumber::key_w, { 0.0f, 0.0f });
	Key_S = Sprite::Create(TextureNumber::key_s, { 0.0f, 0.0f });
	Key_SPACE = Sprite::Create(TextureNumber::key_space, { 0.0f, 0.0f });

	Key_W->SetPosition({ 1100, 300 });
	Key_S->SetPosition({ 1100, 400 });
	Key_SPACE->SetPosition({ 500, 600 });




	// デバッグテキスト初期化
	debugText.Initialize(0);

	stageSelectBG = Sprite::Create(TextureNumber::result_bg, { 0.0f,0.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	BossObj_1 = ObjObject::Create();
	BossObj_2 = ObjObject::Create();
	BossObj_3 = ObjObject::Create();

	BossModel_1 = ObjModel::CreateFromOBJ("TitleFont");
	BossModel_2 = ObjModel::CreateFromOBJ("bullet");
	BossModel_3 = ObjModel::CreateFromOBJ("player2");

	BossObj_1->SetModel(BossModel_1);
	BossObj_2->SetModel(BossModel_2);
	BossObj_3->SetModel(BossModel_3);

	BossObj_1->SetPosition(BossSelectPos_3);
	BossObj_2->SetPosition(BossSelectPos_4);
	BossObj_3->SetPosition(BossSelectPos_5);

	//ボスのスケールを取得
	BossScale_1 = BossObj_1->GetScale();
	BossScale_2 = BossObj_2->GetScale();
	BossScale_3 = BossObj_3->GetScale();

	//ボスのローテーションを取得
	BossRot_1 = BossObj_1->GetRotation();
	BossRot_2 = BossObj_2->GetRotation();
	BossRot_3 = BossObj_3->GetRotation();


	//ボスの初期化
	BossScale_1 = { 1.0f, 1.0f, 1.0f };
	BossScale_2 = { 1.0f, 1.0f, 1.0f };
	BossScale_3 = { 1.0f, 1.0f, 1.0f };

	BossRot_1 = { 0.0f, 0.0f, 0.0f };
	BossRot_2 = { 0.0f, 0.0f, 0.0f };
	BossRot_3 = { 0.0f, 0.0f, 0.0f };


	//天球生成
	Skydome = ObjObject::Create();

	model_Skydome = ObjModel::CreateFromOBJ("TitleSkydome");

	Skydome->SetModel(model_Skydome);
	Skydome->SetScale({ 200.0f, 200.0f, 200.0f });

	//天球のローテーションを取得
	RotSkydome = Skydome->GetRotation();

	//初期カメラシーン
	CameraScene = 1;
	cameraPos = { 0.0f, 0.0f, 100.0f };
	CamForwardFlag = false;

	
}

void StageSelect::Finalize()
{
}

void StageSelect::Update()
{
	DrawTimer++;

	if (DrawTimer >= 60)
	{
		DrawTimer = 0;
	}

	//現在のカメラのシーンに合わせて目的座標の更新
	if (CameraScene == 1)
	{
		//BossObj_2->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		//ボスモデル拡大
		scaleRate += 0.04f;
		BossScale_1 = Lerp::LerpFloat3({ 1.0f, 1.0f, 1.0 }, { 5.0f, 5.0f, 5.0f }, scaleRate);
		BossObj_1->SetScale(BossScale_1);

		//ボスモデルの拡大が終わったらモデル回転
		if (scaleRate >= 1.0f)
		{
			BossRot_1.y += 0.5f;
			BossObj_1->SetRotation(BossRot_1);
		}

		CamTargetPos_U = BossObj_2->GetPosition();
	}

	if (CameraScene == 2)
	{
		//ボスモデル拡大
		scaleRate += 0.04f;
		BossScale_2 = Lerp::LerpFloat3({ 1.0f, 1.0f, 1.0 }, { 5.0f, 5.0f, 5.0f }, scaleRate);
		BossObj_2->SetScale(BossScale_2);

		//ボスモデルの拡大が終わったらモデル回転
		if (scaleRate >= 1.0f)
		{
			BossRot_2.y += 0.5f;
			BossObj_2->SetRotation(BossRot_2);
		}

		CamTargetPos_U = BossObj_3->GetPosition();
		CamTargetPos_D = BossObj_1->GetPosition();
	}

	if (CameraScene == 3)
	{
		//ボスモデル拡大
		scaleRate += 0.04f;
		BossScale_3 = Lerp::LerpFloat3({ 1.0f, 1.0f, 1.0 }, { 5.0f, 5.0f, 5.0f }, scaleRate);
		BossObj_3->SetScale(BossScale_3);

		//ボスモデルの拡大が終わったらモデル回転
		if (scaleRate >= 1.0f)
		{
			BossRot_3.y += 0.5f;
			BossObj_3->SetRotation(BossRot_3);
		}

		CamTargetPos_D = BossObj_2->GetPosition();
	}

	//各シーンイージング処理
	if (CameraScene == 1)
	{
		if (input->TriggerKey(DIK_W) && CameraMoveFlag_U == false && CameraMoveFlag_D == false)
		{
			StoragePos = BossObj_1->GetPosition();
			CameraMoveFlag_U = true;
		}

		if(CameraMoveFlag_U == true )
		{
			if (timeRate >= 1.0)
			{
				CameraMoveFlag_U = false;
				BossScale_1 = { 1.0f, 1.0f, 1.0f };
				scaleRate = 0.0f;
				BossObj_1->SetScale(BossScale_1);
				timeRate = 0.0f;
				CameraScene = 2;
			}
		}

		//決定キーを入力したら、シーン遷移
		if (input->TriggerKey(DIK_SPACE))
		{
			CamForwardFlag = true;
		}

		if (BlackAlpha >= 1.0f)
		{
			//シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}

	if (CameraScene == 2)
	{
		//上回転
		if (input->TriggerKey(DIK_W) && CameraMoveFlag_U == false && CameraMoveFlag_D == false)
		{
			CameraMoveFlag_U = true;
		}

		if (CameraMoveFlag_U == true)
		{
			if (timeRate >= 1.0)
			{
				CameraMoveFlag_U = false;
				timeRate = 0.0f;
				BossScale_2 = { 1.0f, 1.0f, 1.0f };
				BossObj_2->SetScale(BossScale_2);
				scaleRate = 0.0f;
				CameraScene = 3;
			}
		}

		//下回転
		if (input->TriggerKey(DIK_S) && CameraMoveFlag_D == false && CameraMoveFlag_U == false)
		{
			CameraMoveFlag_D = true;
		}

		if (CameraMoveFlag_D == true)
		{
			if (timeRate >= 1.0)
			{
				CameraMoveFlag_D = false;
				timeRate = 0.0f;
				BossScale_2 = { 1.0f, 1.0f, 1.0f };
				BossObj_2->SetScale(BossScale_2);
				scaleRate = 0.0f;
				CameraScene = 1;
			}
		}

		//決定キーを入力したら、シーン遷移
		if (input->TriggerKey(DIK_SPACE))
		{
			CamForwardFlag = true;
		}

		if (BlackAlpha >= 1.0f)
		{
			//シーン切り替え
			SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");
		}
	}

	if (CameraScene == 3)
	{
		//下回転
		if (input->TriggerKey(DIK_S) && CameraMoveFlag_D == false && CameraMoveFlag_U == false)
		{
			CameraMoveFlag_D = true;
		}

		if (CameraMoveFlag_D == true)
		{
			if (timeRate >= 1.0)
			{
				CameraMoveFlag_D = false;
				BossScale_3 = { 1.0f, 1.0f, 1.0f };
				BossObj_3->SetScale(BossScale_3);
				scaleRate = 0.0f;
				timeRate = 0.0f;
				CameraScene = 2;
			}
		}

		//決定キーを入力したら、シーン遷移
		if (input->TriggerKey(DIK_SPACE))
		{
			CamForwardFlag = true;
		}

		if (BlackAlpha >= 1.0f)
		{
			//シーン切り替え
			SceneManager::GetInstance()->ChangeScene("THIRDSTAGE");
		}
	}

	//タイムレート操作
	if (CameraMoveFlag_U == true)
	{
		if (CameraScene == 1)
		{
			BossObj_1->SetPosition(Easing(StoragePos, BossSelectPos_2, timeRate));
			BossObj_2->SetPosition(Easing(BossSelectPos_4, BossSelectPos_3, timeRate));
			BossObj_3->SetPosition(Easing(BossSelectPos_5, BossSelectPos_4, timeRate));
		}

		if (CameraScene == 2)
		{
			BossObj_1->SetPosition(Easing(BossSelectPos_2, BossSelectPos_1, timeRate));
			BossObj_2->SetPosition(Easing(BossSelectPos_3, BossSelectPos_2, timeRate));
			BossObj_3->SetPosition(Easing(BossSelectPos_4, BossSelectPos_3, timeRate));
		}


		timeRate += 0.02f;
	}

	if (CameraMoveFlag_D == true)
	{
		if (CameraScene == 3)
		{
			BossObj_1->SetPosition(Easing(BossSelectPos_1, BossSelectPos_2, timeRate));
			BossObj_2->SetPosition(Easing(BossSelectPos_2, BossSelectPos_3, timeRate));
			BossObj_3->SetPosition(Easing(BossSelectPos_3, BossSelectPos_4, timeRate));
		}

		if (CameraScene == 2)
		{
			BossObj_1->SetPosition(Easing(BossSelectPos_2, BossSelectPos_3, timeRate));
			BossObj_2->SetPosition(Easing(BossSelectPos_3, BossSelectPos_4, timeRate));
			BossObj_3->SetPosition(Easing(BossSelectPos_4, BossSelectPos_5, timeRate));
		}

		timeRate += 0.02f;
	}

	//カメラ前進
	if (CamForwardFlag == true)
	{
		BlackAlpha += 0.02f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
		cameraPos.y -= 0.07f;
		cameraPos.z -= 1;
	}

	//カメラシーン切り替え
	if (input->PushKey(DIK_SPACE))
	{
		timeRate += 0.01f;
	}


	//タイムレートが一定の値を超えないようにする
	if (timeRate >= 1.0f)
	{
		timeRate = 1.0f;
	}

	if (timeRate < 0.0f)
	{
		timeRate = 0.0f;
	}

	//スケールレートの値が一定を超えないようにする
	if (scaleRate >= 1.0f)
	{
		scaleRate = 1.0f;
	}

	if (scaleRate < 0.0f)
	{
		scaleRate = 0.0f;
	}

	//ボスオブジェクト更新
	BossObj_1->Update();
	BossObj_2->Update();
	BossObj_3->Update();

	//カメラ更新
	camera->Update(cameraPos,  cameraPos);


	//天球回転
	RotSkydome.y += 0.5f;
	Skydome->SetRotation(RotSkydome);
	//天球更新
	Skydome->Update();
	

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
	BossObj_1->Draw();
	BossObj_2->Draw();
	BossObj_3->Draw();

	//天球の描画
	Skydome->Draw();


	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 前景スプライト描画
	debugText.DrawAll(cmdList);

	Black->Draw();

	if (CameraScene == 1 || CameraScene == 2)
	{

		if (timeRate == 0.0f)
		{
			if (CameraScene == 1)
			{
				Stage01->Draw();
			}

			if (CameraScene == 2)
			{
				Stage02->Draw();
			}

			SelectUI_UP->Draw();
			Key_W->Draw();
		}
	}

	if (CameraScene == 2 || CameraScene == 3)
	{
		if (timeRate == 0.0f)
		{
			SelectUI_DOWN->Draw();
			Key_S->Draw();
		}
	}

	if (CamForwardFlag == false)
	{
		Stage_Select->Draw();

		if (DrawTimer <= 40)
		{
			Key_SPACE->Draw();
		}
	}

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void StageSelect::DrawDebugText()
{
	//std::ostringstream StageSelect;
	//StageSelect << "StageSelect 1:FirstStage 2:SecondStage 3:ThirdStage";
	//debugText.Print(StageSelect.str(), 10, 10, 2.0f);

	//std::ostringstream TimeRate;
	//TimeRate << "TimeRate:("
	//	<< std::fixed << std::setprecision(2)
	//	<< timeRate << ")"; // z
	//debugText.Print(TimeRate.str(), 10, 560, 1.0f);

	//std::ostringstream cameraScene;
	//cameraScene << "cameraScene:("
	//	<< std::fixed << std::setprecision(2)
	//	<< CameraScene << ")"; // z
	//debugText.Print(cameraScene.str(), 10, 580, 1.0f);
}

XMFLOAT3 StageSelect::Easing(XMFLOAT3 Start, XMFLOAT3 End, float TimeRate)
{
	return Lerp::LerpFloat3(Start, End, timeRate);;
}


