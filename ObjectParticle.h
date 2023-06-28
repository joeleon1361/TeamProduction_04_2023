#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "ObjModel.h"
#include "Camera.h"



// 3D�I�u�W�F�N�g
class ObjectParticle
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �T�u�N���X	

	// �p�C�v���C���Z�b�g
	struct PipelineSet
	{
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;		//�F
		XMMATRIX mat;	// �R�c�ϊ��s��
	};

private: // �萔


public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);

	// �O���t�B�b�N�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

	// �J�����̃Z�b�g
	static void SetCamera(Camera* camera)
	{
		ObjectParticle::camera = camera;
	}

	// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	// �`��㏈��
	static void PostDraw();

	// 3D�I�u�W�F�N�g����
	static std::unique_ptr <ObjectParticle> Create(ObjModel* model , XMFLOAT3 pos);


private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �e�N�X�`������p�p�C�v���C��
	static PipelineSet pipelineSet;
public:
	// �J����
	static Camera* camera;

public: // �����o�֐�
	// �R���X�g���N�^
	ObjectParticle() = default;

	// �f�X�g���N�^
	virtual ~ObjectParticle();

	// ������
	virtual bool Initialize(XMFLOAT3 pos);

	// ���t���[������
	virtual void Update();

	// �`��
	virtual void Draw();

	// ���[���h�s��̎擾
	const XMMATRIX& GetMatWorld() { return matWorld; }

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }

	// ���W�̎擾
	const XMFLOAT3& GetRotation() { return rotation; }

	// �F�̎擾
	const XMFLOAT4& GetColor() { return color; }

	//�f�X�t���O�̎擾
	const bool GetDeathFlag() { return DeathFlag; }

	// ���[���h���W�̏K��
	const XMFLOAT3 GetWorldPosition()
	{
		XMVECTOR positionV;
		XMFLOAT3 worldPosition;
		positionV = DirectX::XMLoadFloat3(&position);
		positionV.m128_f32[3] = 1.0f;
		positionV = DirectX::XMVector3Transform(positionV, parent->matWorld);
		DirectX::XMStoreFloat3(&worldPosition, positionV);
		return worldPosition;
	}

	// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	// �p�x�̐ݒ�
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	// �X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	// �F�̐ݒ�
	void SetColor(const XMFLOAT4& color) { this->color = color; }

	// ���f���̃Z�b�g
	void SetModel(ObjModel* model) { this->model = model; }

	void SetParent(ObjectParticle* parent) { this->parent = parent; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

protected: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[���h�ϊ��s��
	XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	ObjectParticle* parent = nullptr;
	// 
	XMMATRIX updateMatWorld;
	// ���f��
	ObjModel* model = nullptr;

	// �r���{�[�h
	bool isBillboard = false;

	// �N���X��
	const char* name = nullptr;

	//�f�X�t���O
	bool DeathFlag = false;
};

