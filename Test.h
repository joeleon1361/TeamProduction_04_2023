#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "ObjModel.h"
#include "Camera.h"

// �p�[�e�B�N���}�l�[�W��
class Test
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale; // �X�P�[��
		XMFLOAT4 color;
		float rotation;
	};

	// �p�C�v���C���Z�b�g
	struct PipelineSet
	{
		// ���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	// �p�[�e�B�N��1��
	class Particle
	{
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::���ȗ�
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
	public:
		// ���W
		XMFLOAT3 position = {};
		// ���x
		XMFLOAT3 velocity = {};
		// �����x
		XMFLOAT3 accel = {};
		// �F
		XMFLOAT4 color = {};
		// �X�P�[��
		float scale = 1.0f;
		// ��]
		float rotation = 0.0f;
		// �����l
		XMFLOAT4 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// �ŏI�l
		XMFLOAT4 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// ���݃t���[��
		int frame = 0;
		// �I���t���[��
		int num_frame = 0;
	};

private: // �萔
	static const int vertexCount = 65536;		// ���_��

public: // �ÓI�����o�֐�
	// �C���X�^���X����
	static Test* Create(ID3D12Device* device, Camera* camera, int blensType, const wchar_t* filename);

public: // �����o�֐�	
	// �R���X�g���N�^
	Test(ID3D12Device* device, Camera* camera);

	// ������
	void Initialize(int blendType, const wchar_t* filename);

	// ���t���[������
	void Update();

	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	// �p�[�e�B�N���̒ǉ�
	void Add(int life, const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel, const XMFLOAT4& start_color, const XMFLOAT4& end_color, float start_scale, float end_scale, float start_rotation, float end_rotation);

	// �f�X�N���v�^�q�[�v�̏�����
	void InitializeDescriptorHeap();

	// �O���t�B�b�N�p�C�v���C������
	void InitializeGraphicsPipeline(int blendType);

	// �e�N�X�`���ǂݍ���
	void LoadTexture(const wchar_t* filename);

	// ���f���쐬
	void CreateModel();

	//---�p�[�e�B�N���֐�---//
	//��{�p�[�e�B�N��
	void DefaultParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//�{�X���ʔj��p�[�e�B�N��
	void BlastPart_1(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void BlastPart_2(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void BlastPart_3(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);
	void BlastPart_4(int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//�v���C���[�W�F�b�g�p�[�e�B�N��
	void JettParticle(int PartNum, int Life, XMFLOAT3 position, XMFLOAT3 velosity, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//�u�[�X�g�p�[�e�B�N��
	void BoostParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	//�Ήԃp�[�e�B�N��
	void SparkParticle(int PartNum, int Life, XMFLOAT3 position, int StartScale, int EndScale, XMFLOAT4 StartColor, XMFLOAT4 EndColor);

	// �e�p�[�e�B�N������
	void BulletParticle(int PartNum, int Life, XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale);

	// ���f���̃Z�b�g
	void SetModel(ObjModel* model) { this->model = model; }

private: // �����o�ϐ�

	// �f�o�C�X
	ID3D12Device* device = nullptr;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize = 0u;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �p�[�e�B�N���z��
	std::forward_list<Particle> particles;
	// �J����
	Camera* camera = nullptr;
	// ���f��
	ObjModel* model = nullptr;
	// �e�N�X�`������p�p�C�v���C��
	static PipelineSet pipelineSet;

	enum blendType
	{
		translucent,
		add,
		sub
	};

	//�d�͉����x
	float Gravity = 9.8f;

	//Y���̈ړ���
	float VY = 0.0f;

	//�o�ߎ���
	int elapsedTime = 0;

};



