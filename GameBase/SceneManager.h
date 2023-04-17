#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

// �V�[���Ǘ�
class SceneManager
{
public:
	static SceneManager* GetInstance();

	
	// �X�V
	void Update();

	// �`��
	void Draw();

	// ���V�[���\��
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { this->sceneFactory = sceneFactory; }

	//���
	void Finalize();

private: //�����o�ϐ�
//���̃V�[��
	BaseScene* scene = nullptr;
	//���̃V�[��
	BaseScene* nextScene = nullptr;
	//�V�[���H��
	AbstractSceneFactory* sceneFactory = nullptr;
};

