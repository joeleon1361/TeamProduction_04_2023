#pragma once
#include "ObjObject.h"
class Boss :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static Boss* Create(ObjModel* model = nullptr);

private: // �ÓI�����o�ϐ�

public: // �����o�֐�
// ������
	bool Initialize() override;

	// ���t���[������
	void Update() override;

private: // �����o�ϐ�
	Input* input = Input::GetInstance();

	ObjModel* modelBoss = nullptr;

public:
	// ���˃^�C�}�[
	int32_t shotTimer = 0;
};

