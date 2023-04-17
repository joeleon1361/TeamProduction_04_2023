#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
public: // �ÓI�����o�֐�
	 // �V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	// �V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private: // �ÓI�����o�ϐ�
	// ��ʃN���A�J���[
	static const float clearColor[4];

public: // �����o�֐�
	// �R���X�g���N�^
	PostEffect();

	// ������
	void Initialize();

	// �p�C�v���C������
	void CreateGraphicsPipelineState();

	// �`��R�}���h�̔��s
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // �����o�ϐ�
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelineState;

	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;

	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

