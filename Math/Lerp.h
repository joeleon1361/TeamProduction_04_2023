#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
class Lerp
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
	static float LerpFloat(const float start, const float end, const float time);

	static XMFLOAT2 LerpFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);

	static XMFLOAT3 LerpFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);

	static XMFLOAT4 LerpFloat4(const XMFLOAT4& start, const XMFLOAT4& end, const float time);
};

