#pragma once

#include "CollisionPrimitive.h"
#include "Vector.h"
#include <d3dx12.h>

// 当たり判定ヘルパークラス
class Collision
{
public:
	// 球と球の当たり判定
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	
	// 球と平面の当たり判定
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);

	// 点と三角形の最近接点を求める
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	// 球と法線付き三角形の当たりチェック
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	// レイと平面の当たり判定
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	
	// レイと法線付き三角形の当たり判定
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	// レイと球の当たり判定
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	
	// Ray to OBB
	static bool CheckRay2OBB(const Ray& ray, const Box& obb, float* distance, DirectX::XMVECTOR* inter);
	
	// OBB - OBB
	static bool CheckOBBOBB(const Box& box1, const Box& box2, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject);

	static float LenSegOnSeparateAxis(const Vector3& Sep, const Vector3& e1, const Vector3& e2, const Vector3& e3 = Vector3{ 0,0,0 });
};