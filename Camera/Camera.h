#pragma once

#include <DirectXMath.h>

// �J����
class Camera
{
protected: // �G�C���A�X
// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �����o�֐�
	
	// �R���X�g���N�^
	Camera( int window_width, int window_height );

	// �f�X�g���N�^
	~Camera() = default;

	// ���t���[���X�V
	void Update();

	// �r���[�s����X�V
	void UpdateViewMatrix();

	// �ˉe�s����X�V
	void UpdateProjectionMatrix();

	// �r���[�s��̎擾
	const XMMATRIX &GetViewMatrix() {
		return matView;
	}

	// �ˉe�s��̎擾
	const XMMATRIX &GetProjectionMatrix() {
		return matProjection;
	}
	
	// �r���[�ˉe�s��̎擾
	const XMMATRIX &GetViewProjectionMatrix() {
		return matViewProjection;
	}

	// �r���{�[�h�s��̎擾
	const XMMATRIX &GetBillboardMatrix() {
		return matBillboard;
	}

	
	// ���_���W�̎擾
	const XMFLOAT3 &GetEye() {
		return eye;
	}

	
	// ���_���W�̐ݒ�
	void SetEye( XMFLOAT3 eye ) {
		this->eye = eye; viewDirty = true;
	}

	// �����_���W�̎擾
	const XMFLOAT3 &GetTarget() {
		return target;
	}

	// �����_���W�̐ݒ�
	void SetTarget( XMFLOAT3 target ) {
		this->target = target; viewDirty = true;
	}
	
	// ������x�N�g���̎擾
	const XMFLOAT3 &GetUp() {
		return up;
	}

	// ������x�N�g���̐ݒ�
	void SetUp( XMFLOAT3 up ) {
		this->up = up; viewDirty = true;
	}

	// �x�N�g���ɂ�鎋�_�ړ�
	void MoveEyeVector( const XMFLOAT3 & move );
	
	// �x�N�g���ɂ��ړ�
	void MoveVector( const XMFLOAT3 & move );

protected: // �����o�ϐ�
	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �r���{�[�h�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	// ���_���W
	XMFLOAT3 eye = { 0, 10, 0 };
	// �����_���W
	XMFLOAT3 target = { 0, 0, 0 };
	// ������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;
};

