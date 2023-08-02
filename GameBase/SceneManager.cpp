#include "SceneManager.h"

#include <cassert>

extern bool victory = false;

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�������Ȃ�
	if (nextScene)
	{
		//���V�[���̏I��
		if (scene)
		{
			scene->Finalize();
			delete scene;
		}

		//�V�[���؂�ւ�
		scene = nextScene;
		nextScene = nullptr;

		//�V�V�[���̏�����
		scene->Initialize(/*dxCommon*/);

	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//���V�[���𐶐�
	nextScene = sceneFactory->CreateScene(sceneName);
}

void SceneManager::Finalize()
{
	//�Ō�̃V�[���̏I���Ɖ��
	scene->Finalize();
	delete scene;
}
