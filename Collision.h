#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Collision
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	static bool BasicCollisionDetection(XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize);

	static bool CCDCollisionDetection(XMFLOAT3 prevBulletPos, XMFLOAT3 bulletPos, float bulletSize, XMFLOAT3 bossPos, float bossSize);
};

