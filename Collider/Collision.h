#pragma once

#include "CollisionPrimitive.h"

// �����蔻��w���p�[�N���X
class Collision
{
public:
	// ���Ƌ��̓����蔻��
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	
	// ���ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);

	// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	// ���Ɩ@���t���O�p�`�̓�����`�F�b�N
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	// ���C�ƕ��ʂ̓����蔻��
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	
	// ���C�Ɩ@���t���O�p�`�̓����蔻��
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	// ���C�Ƌ��̓����蔻��
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
};