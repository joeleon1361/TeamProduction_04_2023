#include "FrameWork.h"

extern HWND hwnd = nullptr;

void FrameWork::Run()
{
	// �Q�[���̏�����
	Initialize();

	while (true) // �Q�[�����[�v
	{
		// �X�V
		Update();

		// �I�����N�G�X�g���󂯎�����甲����
		if (IsEndRequest())
		{
			break;
		}

		// �`��
		Draw();
	}

	// �Q�[���̏I��
	Finalize();
}

void FrameWork::Initialize()
{
	// �Q�[���E�B���h�E�̍쐬
	win = new WinApp();
	win->CreateGameWindow();
	hwnd = win->GetHwnd();

	//DirectX����������
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region �ėp�@�\������
	// ���͂̏�����
	input = Input::GetInstance();
	input->Initialize(win->GetInstance(), win->GetHwnd());
	// �I�[�f�B�I�̏�����
	sound = Sound::GetInstance();
	sound->Initialize();

	// �X�v���C�g�ÓI������
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	// �|�X�g�G�t�F�N�g�p�e�N�X�`���̓ǂݍ���
	// Sprite::LoadTexture(100, L"Resources/white1x1.png");

	// �|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();

	// 3D�I�u�W�F�N�g�ÓI������
	ObjObject::StaticInitialize(dxCommon->GetDevice());

	ObjectParticle::StaticInitialize(dxCommon->GetDevice());

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
#pragma endregion
}

void FrameWork::Finalize()
{
	// �e����
	//�V�[���}�l�[�W���[�̉��
	SceneManager::GetInstance()->Finalize();
	// //FBXLoader���
	FbxLoader::GetInstance()->Finalize();
	Sound::GetInstance()->Finalize();
	// safe_delete(input);
	//safe_delete(dxCommon);
	delete postEffect;

	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
	safe_delete(win);
}

void FrameWork::Update()
{
	// ���b�Z�[�W����
	if (win->ProcessMessage())
	{
		// �Q�[�����[�v�𔲂���
		isEndRequest = true;
		return;
	}

	hwnd = win->GetHwnd();

	// ���͊֘A�̖��t���[������
	input->Update();
	
	//�V�[���X�V
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	// �����_�[�e�N�X�`���ւ̕`��
		// postEffect->PreDrawScene(dxCommon->GetCommandList());
		// gameScene->Draw();
		// postEffect->PostDrawScene(dxCommon->GetCommandList());

	// �`��J�n
	dxCommon->PreDraw();

	// �|�X�g�G�t�F�N�g�̕`��
	// postEffect->Draw(dxCommon->GetCommandList());

	// �Q�[���V�[���̕`��
	SceneManager::GetInstance()->Draw();
	
	// �`��I��
	dxCommon->PostDraw();
}
