#include "Camera.h"

using namespace DirectX;

Camera::Camera( int window_width, int window_height )
{
	aspectRatio = (float)window_width / window_height;

	phi = -3.14159265f / 2;
	theta = 0;
	nowTheta = theta;

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}

void Camera::Update(XMFLOAT3 new_position, XMFLOAT3 direction_position)
{
	dirty = false;

	// Calculate the view matrix
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	// Extract the cameraPos position from the view matrix
	XMFLOAT3 cameraPos;
	XMStoreFloat3(&cameraPos, XMVectorNegate(XMVector3TransformCoord(XMLoadFloat3(&target), XMMatrixInverse(nullptr, view))));

	// Calculate the direction vector from the camera's position to the boss position
	XMFLOAT3 directionVector = XMFLOAT3(direction_position.x - cameraPos.x, direction_position.y - cameraPos.y, direction_position.z - cameraPos.z);

	// Normalize the direction vector
	XMVECTOR directionVectorNormalized = XMVector3Normalize(XMLoadFloat3(&directionVector));

	// Calculate the camera's new position based on the direction vector and camera speed
	XMFLOAT3 newPosition = XMFLOAT3(new_position.x + XMVectorGetX(directionVectorNormalized) * 48.0f, new_position.y + XMVectorGetY(directionVectorNormalized) * 48.0f + 10.0f, new_position.z + XMVectorGetZ(directionVectorNormalized) * 48.0f);

	// Convert cameraPos to cameraEye
	XMVECTOR posV = XMLoadFloat3(&newPosition);
	XMVECTOR targetV = XMLoadFloat3(&target);
	XMVECTOR upV = XMLoadFloat3(&up);

	// Calculate the view matrix
	view = XMMatrixLookAtLH(posV, targetV, upV);

	// Extract the cameraEye position from the view matrix
	XMVECTOR cameraEyeVec = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	cameraEyeVec = XMVector4Transform(cameraEyeVec, XMMatrixInverse(nullptr, view));

	// Convert cameraEyeVec back to a XMFLOAT3
	XMFLOAT3 cameraEye;
	XMStoreFloat3(&cameraEye, cameraEyeVec);

	// Update the camera's position, target, and up vectors
	eye = cameraEye;
	target = direction_position;
	up = up;

	viewDirty = true;

	if ( viewDirty || projectionDirty ) {
		// 再計算必要なら
		if ( viewDirty ) {
			// ビュー行列更新
			UpdateViewMatrix();
			viewDirty = false;
		}

		// 再計算必要なら
		if ( projectionDirty ) {
			// ビュー行列更新
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		// ビュープロジェクションの合成
		matViewProjection = matView * matProjection;
	}
}

void Camera::UpdateViewMatrix()
{
	// ビュー行列の更新
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3( &eye );
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3( &target );
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3( &up );

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract( targetPosition, eyePosition );
	// 0ベクトルだと向きが定まらないので除外
	assert( !XMVector3Equal( cameraAxisZ, XMVectorZero() ) );
	assert( !XMVector3IsInfinite( cameraAxisZ ) );
	assert( !XMVector3Equal( upVector, XMVectorZero() ) );
	assert( !XMVector3IsInfinite( upVector ) );
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize( cameraAxisZ );

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross( upVector, cameraAxisZ );
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize( cameraAxisX );

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross( cameraAxisZ, cameraAxisX );

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）	

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet( 0, 0, 0, 1 );
	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose( matCameraRot );

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate( eyePosition );
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot( cameraAxisX, reverseEyePosition );	// X成分
	XMVECTOR tY = XMVector3Dot( cameraAxisY, reverseEyePosition );	// Y成分
	XMVECTOR tZ = XMVector3Dot( cameraAxisZ, reverseEyePosition );	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet( tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f );
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet( 0, 0, 0, 1 );
#pragma endregion

#pragma region Y軸回りビルボード行列の計算
	// カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = cameraAxisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize( upVector );
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross( ybillCameraAxisX, ybillCameraAxisY );

	// Y軸回りビルボード行列
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet( 0, 0, 0, 1 );
#pragma endregion
}

void Camera::UpdateProjectionMatrix()
{
	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians( 60.0f ),
		aspectRatio,
		0.1f, 1000.0f
	);
}

void Camera::MoveEyeVector( const XMFLOAT3 &move )
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye( eye_moved );
}

void Camera::MoveVector( const XMFLOAT3 &move )
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye( eye_moved );
	SetTarget( target_moved );
}

