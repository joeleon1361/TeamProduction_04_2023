#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sound.h"
#include "GamePlay.h"
#include "fbxsdk.h"
#include "PostEffect.h"
#include "FbxLoader.h"
#include "ObjObject.h"
#include "ObjectParticle.h"

#include "SceneManager.h"
#include "AbstractSceneFactory.h"

class FrameWork
{
public: // �����o�֐�

	// ���s
	void Run();

	// ������
	virtual void Initialize();

	// �I��
	virtual void Finalize();

	// �X�V
	virtual void Update();

	// �`��
	virtual void Draw();

	// �I���`�F�b�N
	virtual bool IsEndRequest() { return isEndRequest; }
	
	virtual ~FrameWork() = default;

protected: //�����o�ϐ�
//�Q�[�����[�v�I�����N�G�X�g
	bool isEndRequest = false;

	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;
	PostEffect* postEffect = nullptr;
	
	AbstractSceneFactory* sceneFactory = nullptr;
};

