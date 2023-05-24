#pragma once

// �����蔻��v���~�e�B�u
#include <DirectXMath.h>
#include "Vector.h"
#include <array>

// ��
struct Sphere
{
	// ���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	// ���a
	float radius = 1.0f;
};

// ����
struct Plane
{
	// �@���x�N�g��
	DirectX::XMVECTOR normal = { 0, 1, 0, 0 };
	// ���_(0,0,0)����̋���
	float distance = 0.0f;
};

// �@���t���O�p�`�i���v��肪�\�ʁj
class Triangle
{
public:
	// ���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// �@���x�N�g��
	DirectX::XMVECTOR normal;

	// �@���̌v�Z
	void ComputeNormal();
};

// ���C�i�������j
struct Ray
{
	// �n�_���W
	DirectX::XMVECTOR start = { 0,0,0,1 };
	// ����
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

// �l�p�`
struct Box
{
	//���S���W
	DirectX::XMVECTOR center = {};
	std::array<DirectX::XMVECTOR, 3> normalDirect;
	//float fLength[3] = {};
	//�傫��
	Vector3 scale = { 1,1,1 };
	Vector3 minPosition = {};
	Vector3 maxPosition = {};
	Vector3 rotation = {};
};