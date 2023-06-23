#include "Lerp.h"

using namespace DirectX;

float Lerp::LerpFloat(const float start, const float end, const float time)
{
	return start * (1.0f - time) + end * time;
}

XMFLOAT2 Lerp::LerpFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - time), start.y * (1.0f - time));
	B = XMFLOAT2(end.x * time, end.y * time);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT3 Lerp::LerpFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - time), start.y * (1.0f - time), start.z * (1.0f - time));
	B = XMFLOAT3(end.x * time, end.y * time, end.z * time);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT4 Lerp::LerpFloat4(const XMFLOAT4& start, const XMFLOAT4& end, const float time)
{
	XMFLOAT4 A, B;
	A = XMFLOAT4(start.x * (1.0f - time), start.y * (1.0f - time), start.z * (1.0f - time), start.w * (1.0f - time));
	B = XMFLOAT4(end.x * time, end.y * time, end.z * time, end.w * time);

	XMFLOAT4 position;
	position = XMFLOAT4(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w);
	return position;
}