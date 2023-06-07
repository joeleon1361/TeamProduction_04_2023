#include "Title.h"

Title::Title()
{
}

Title::~Title()
{
}

void Title::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	//遷移フラグリセット
	StartFlag = false;

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::title_bg, L"Resources/Sprite/TitleUI/Title.jpg")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::titlefont, L"Resources/Sprite/TitleUI/TitleFont.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::pressspace, L"Resources/Sprite/TitleUI/PressSpace.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	titleBG = Sprite::Create(TextureNumber::title_bg, { 0.0f,0.0f });
	titleBG_LD = Sprite::Create(TextureNumber::title_bg, { 0.0f,720.0f });
	titleBG_RU = Sprite::Create(TextureNumber::title_bg, { 1280.0f,0.0f });
	titleBG_RD = Sprite::Create(TextureNumber::title_bg, { 1280.0f,720.0f });

	TitleFont = Sprite::Create(TextureNumber::titlefont, { 290,200.0f });
	PressSpace = Sprite::Create(TextureNumber::pressspace, { 380,500.0f });

	Black = Sprite::Create(TextureNumber::black, { 0,0.0f });
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	//タイトルフォント生成
	objTitleFont = ObjObject::Create();

	// モデルセット
	modelTitleFont = ObjModel::CreateFromOBJ("TitleSkydome");
	objTitleFont->SetModel(modelTitleFont);

	objTitleFont->SetScale({ 20.0f,20.0f,20.0f });
	objTitleFont->SetPosition({ 0.0f,10.0f,45.0f});
	objTitleFont->SetRotation({ -20.0f,0.0f,0.0f});

	// パーティクル
	//Particle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");


	//カメラの注視点と座標を設定
	camera->SetEye({ 0.0f, 0.0f, 0.0f });
	camera->SetTarget({ 0.0f, 0.0f, 100.0f });
}

void Title::Finalize()
{
	safe_delete(titleBG);
	safe_delete(titleBG_LD);
	safe_delete(titleBG_RU);
	safe_delete(titleBG_RD);
}

void Title::Update()
{
	DrawTimer++;

	if (DrawTimer >= 60)
	{
		DrawTimer = 0;
	}
	
	if (BlackAlpha >= 1.0f)
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	//スプライト座標変更
	TitlePos_LU.x-= Speed_y;
	TitlePos_LU.y--;

	TitlePos_LD.x -= Speed_y;
	TitlePos_LD.y--;

	TitlePos_RU.x -= Speed_y;
	TitlePos_RU.y--;

	TitlePos_RD.x -= Speed_y;
	TitlePos_RD.y--;

	//スプライトの座標を送信
	titleBG->SetPosition(TitlePos_LU);
	titleBG_LD->SetPosition(TitlePos_LD);
	titleBG_RU->SetPosition(TitlePos_RU);
	titleBG_RD->SetPosition(TitlePos_RD);

	//スプライトが端になったらリセット
	if (TitlePos_LU.x <= -1280)
	{
		TitlePos_LU.x = 1280;
		TitlePos_LU.y = 720;
	}

	if (TitlePos_LD.x <= -1280)
	{
		TitlePos_LD.x = 0;
		TitlePos_LD.y = 720;
	}

	if (TitlePos_RU.x <= 0)
	{
		TitlePos_RU.x = 1280;
		TitlePos_RU.y = 0;
	}

	if (TitlePos_RD.x <= -1280)
	{
		TitlePos_RD.x = 1280;
		TitlePos_RD.y = 720;
	}

	if (input->TriggerKey(DIK_SPACE))
	{
		StartFlag = true;
	}

	if (StartFlag == true)
	{
		BlackAlpha += 0.02f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
	}

	objTitleFont->Update();

	// パーティクル更新
	//Particle->Update();

	camera->SetEye({ 0.0f, 0.0f, 0.0f });
	camera->SetTarget({ 0.0f, 0.0f, 100.0f });
	camera->Update();
}

void Title::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	titleBG->Draw();
	titleBG_LD->Draw();
	titleBG_RU->Draw();
	titleBG_RD->Draw();

	TitleFont->Draw();

	if (DrawTimer <= 30)
	{
		PressSpace->Draw();
	}

	Black->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//objTitleFont->Draw();

	//パーティクル更新
	//Particle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 前景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion

}