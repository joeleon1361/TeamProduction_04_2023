#include "MyGame.h"
#include "SceneFactory.h"

void MyGame::Initialize()
{
	//���N���X�̏���������
	FrameWork::Initialize();

	//�V�[���̏���������
	//�V�[���t�@�N�g���𐶐����A�}�l�[�W���[�ɃZ�b�g
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	//�V�[���}�l�[�W���ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	//���N���X�̏I������
	FrameWork::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	FrameWork::Update();
}

void MyGame::Draw()
{
	//���N���X�̕`�揈��
	FrameWork::Draw();
}
