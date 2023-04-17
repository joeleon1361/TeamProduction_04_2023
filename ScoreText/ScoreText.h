#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

class ScoreText
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 256;	// �ő啶����
	static const int fontWidth = 32;			// �t�H���g�摜��1�������̉���
	static const int fontHeight = 36;		// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 11;	// �t�H���g�摜��1�s���̕�����

	ScoreText();
	~ScoreText();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, XMFLOAT2 position, XMFLOAT4 color, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
};

