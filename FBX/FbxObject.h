#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "FbxModel.h"
#include "Camera.h"

class FbxObject3d
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �萔
// �{�[���̍ő吔
	static const int MAX_BONES = 32;

public: // �T�u�N���X
// �萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	// �萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};


public: // �ÓI�����o�֐�
// setter
	static void SetDevice( ID3D12Device *device ) { FbxObject3d::device = device; }
	static void SetCamera( Camera *camera ) { FbxObject3d::camera = camera; }

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

private: // �ÓI�����o�ϐ�
	static ID3D12Device *device;

	// �J����
	static Camera *camera;

	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;

	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

public: // �����o�֐�
	// ������
	void Initialize();

	// ���t���[������
	void Update();
	
	// ���f���̃Z�b�g
	void SetModel( FbxModel *fbxmodel ) { this->fbxmodel = fbxmodel; }
	
	// �`��
	void Draw( ID3D12GraphicsCommandList *cmdList );
	
	// ���W�̎擾
	const XMFLOAT3 &GetPosition() { return position; }
	
	// ���W�̐ݒ�
	void SetPosition( XMFLOAT3 position ) { this->position = position; }

	void SetRotation( XMFLOAT3 rotation ) { this->rotation = rotation; }
	
	// �X�P�[���̐ݒ�
	void SetScale( XMFLOAT3 scale ) { this->scale = scale; }
	
	// �A�j���[�V�����J�n
	void PlayAnimation();

protected: // �����o�ϐ�
// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// ���f��
	FbxModel *fbxmodel = nullptr;

	// 1�t���[��
	FbxTime frameTime;
	// �A�j���[�V�����J�n����
	FbxTime startTime;
	// �A�j���[�V�����I������
	FbxTime endTime;
	// ���ݎ���
	FbxTime currentTime;
	// �A�j���[�V�����Đ���
	bool isPlay = false;
};

