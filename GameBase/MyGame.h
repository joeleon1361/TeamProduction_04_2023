#pragma once
#include "FrameWork.h"

// �Q�[���ŗL�̃N���X
class MyGame : public FrameWork
{
public: //�����o�֐�
	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;
};

