#include "Title.h"

Title::Title()
{
}

Title::~Title()
{
}

void Title::Initialize()
{
	// �T�E���h������
	sound->Initialize();

	//�J�ڃt���O���Z�b�g
	StartFlag = false;

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �J�����Z�b�g
	ObjObject::SetCamera(camera);
	ObjectParticle::SetCamera(camera);

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

	//�^�C�g���t�H���g����
	objTitleFont = ObjObject::Create();

	// ���f���Z�b�g
	modelTitleFont = ObjModel::CreateFromOBJ("TitleSkydome");
	objTitleFont->SetModel(modelTitleFont);

	objTitleFont->SetScale({ 20.0f,20.0f,20.0f });
	objTitleFont->SetPosition({ 0.0f,10.0f,45.0f});
	objTitleFont->SetRotation({ -20.0f,0.0f,0.0f});

	//�I�u�W�F�N�g�p�[�e�B�N��
	modelObject = ObjModel::CreateFromOBJ("TitleSkydome");
	//Object = ObjectParticle::Create(modelObject, DefaultPos);
	//Object->SetModel(modelObject);

	//Object->SetScale({ 10.0f,10.0f,10.0f });
	//Object->SetRotation({ 0.0f,0.0f,0.0f });
	

	// �p�[�e�B�N��
	//Particle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");


	//�J�����̒����_�ƍ��W��ݒ�
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
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	//�X�v���C�g���W�ύX
	TitlePos_LU.x-= Speed_y;
	TitlePos_LU.y--;

	TitlePos_LD.x -= Speed_y;
	TitlePos_LD.y--;

	TitlePos_RU.x -= Speed_y;
	TitlePos_RU.y--;

	TitlePos_RD.x -= Speed_y;
	TitlePos_RD.y--;

	//�X�v���C�g�̍��W�𑗐M
	titleBG->SetPosition(TitlePos_LU);
	titleBG_LD->SetPosition(TitlePos_LD);
	titleBG_RU->SetPosition(TitlePos_RU);
	titleBG_RD->SetPosition(TitlePos_RD);

	//�X�v���C�g���[�ɂȂ����烊�Z�b�g
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

	if (input->TriggerKey(DIK_M))
	{
		//Object = ObjectParticle::Create(modelObject, DefaultPos);
		CreateParticle();
	}

	if (StartFlag == true)
	{
		BlackAlpha += 0.02f;
		Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });
	}

	objTitleFont->Update();

	if (Object)
	{
		Object->Update();
	}
	// �p�[�e�B�N���X�V
	//Particle->Update();

	//�I�u�W�F�N�g�p�[�e�B�N�����X�V
	for (std::unique_ptr<ObjectParticle>& part : particle)
	{
		part->Update();
	}

	particle.remove_if([](std::unique_ptr<ObjectParticle>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	//�I�u�W�F�N�g�p�[�e�B�N������������ō폜
	ObjPart.remove_if([](std::unique_ptr<ObjectParticle>& objectParticle)
		{
			return objectParticle->GetDeathFlag();
		});

	camera->SetEye({ 0.0f, 0.0f, 0.0f });
	camera->SetTarget({ 0.0f, 0.0f, 100.0f });
	camera->Update();
}

void Title::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	/*titleBG->Draw();
	titleBG_LD->Draw();
	titleBG_RU->Draw();
	titleBG_RD->Draw();

	TitleFont->Draw();*/

	if (DrawTimer <= 30)
	{
		PressSpace->Draw();
	}

	//Black->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	ObjObject::PreDraw(cmdList);
	ObjectParticle::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	//objTitleFont->Draw();

	if (Object)
	{
		Object->Draw();
	}

	//�I�u�W�F�N�g�p�[�e�B�N���`��
	for (std::unique_ptr<ObjectParticle>& part : particle)
	{
		part->Draw();
	}

	for (std::unique_ptr<ObjectParticle>& objPart : ObjPart)
	{
		objPart->Draw();
	}

	//�p�[�e�B�N���X�V
	//Particle->Draw(cmdList);

	// 3D�I�u�W�F�N�g�`��㏈��
	ObjectParticle::PostDraw();
	ObjObject::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �O�i�X�v���C�g�`��

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion

}

void Title::CreateParticle()
{
	XMFLOAT3 Position = {};

	Position.x = 10.0f;
	Position.y = 10.0f;
	Position.z = 0.0f;

	//�p�[�e�B�N���𔭐�������
	std::unique_ptr<ObjectParticle> newPart = std::make_unique<ObjectParticle>();
	newPart = ObjectParticle::Create(modelObject, Position);
	ObjPart.push_back(std::move(newPart));
}
