#include "Camera.h"

using namespace DirectX;

Camera::Camera( int window_width, int window_height )
{
	aspectRatio = (float)window_width / window_height;

	phi = -3.14159265f / 2;
	theta = 0;
	nowTheta = theta;

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();

	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}

void Camera::Update()
{
	//Input* input = Input::GetInstance();

	dirty = false;

	//Input::MouseMove mouseMove = input->GetMouseMove();

	//if (input->PushMouseRight())
	//{
	//	phi += XM_PI / 180.0f * mouseMove.lX / 7;
	//	theta += XM_PI / 180.0f * mouseMove.lY / 7;
	//	if (theta > 40 * XM_PI / 180.0f)
	//		theta = 40 * XM_PI / 180.0f;
	//	else if (theta < -40 * XM_PI / 180.0f)
	//		theta = -40 * XM_PI / 180.0f;

	//	if (phi > 360 * XM_PI / 180.0f)
	//		phi -= 360 * XM_PI / 180.0f;
	//	else if (phi < 0)
	//		phi += 360 * XM_PI / 180.0f;

	//	/*if (theta < 0)
	//	{
	//		distance = 48 * (1 + theta * 1.1f);
	//	}*/
	//	//printf("phi : %f , theta : %f\n", phi, theta);
	//	dirty = true;
	//}

	// �z�C�[�����͂ŋ�����ύX
	/*if (mouseMove.lZ != 0) {
		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}*/

	{
		// define the camera eye, target, and up vectors
		XMFLOAT3 cameraEye = eye;
		XMFLOAT3 cameraTarget = target;
		XMFLOAT3 cameraUp = up;

		// limit the pitch angle to prevent camera flipping
		/*if (theta > 89.0f) {
			theta = 89.0f;
		}
		if (theta < -89.0f) {
			theta = -89.0f;
		}*/

		// calculate the new camera front vector
		XMFLOAT3 front;
		front.x = cos(XMConvertToRadians(phi)) * cos(XMConvertToRadians(theta));
		front.y = sin(XMConvertToRadians(theta));
		front.z = sin(XMConvertToRadians(phi)) * cos(XMConvertToRadians(theta));
		XMVECTOR newCameraFront = XMVector3Normalize(XMLoadFloat3(&front));

		// update the camera position to rotate around the boss
		XMFLOAT3 cameraDirection = XMFLOAT3(cameraEye.x - bossPos.x, cameraEye.y - bossPos.y, cameraEye.z - bossPos.z);
		XMVECTOR newCameraDirection = XMVector3Normalize(XMLoadFloat3(&cameraDirection));
		XMFLOAT3 cameraRight;
		XMStoreFloat3(&cameraRight, XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&cameraUp), newCameraDirection)));
		XMFLOAT3 cameraUpRotated;
		XMStoreFloat3(&cameraUpRotated, XMVector3Normalize(XMVector3Cross(newCameraDirection, XMLoadFloat3(&cameraRight))));
		XMFLOAT3 newCameraPos = XMFLOAT3(bossPos.x + XMVectorGetX(newCameraDirection) * distance, bossPos.y + XMVectorGetY(newCameraDirection) * distance, bossPos.z + XMVectorGetZ(newCameraDirection) * distance);

		// adjust the camera position to keep the player in the center of the view
		XMFLOAT3 playerDirection = XMFLOAT3(playerPos.x - newCameraPos.x, playerPos.y - newCameraPos.y, playerPos.z - newCameraPos.z);
		XMVECTOR newPlayerDirection = XMVector3Normalize(XMLoadFloat3(&playerDirection));
		float dotProduct = XMVectorGetX(XMVector3Dot(newCameraFront, newPlayerDirection));
		if (dotProduct < 0.9f) {
			newCameraPos = XMFLOAT3(playerPos.x - XMVectorGetX(newCameraFront) * distance, playerPos.y - XMVectorGetY(newCameraFront) * distance, playerPos.z - XMVectorGetZ(newCameraFront) * distance);
		}

		// update the camera eye, target, and up vectors
		eye = newCameraPos;
		target = bossPos;
		up = cameraUpRotated;

		viewDirty = true;
	}

	//if (dirty || viewDirty) {
	//	nowTheta = theta;
	//	/*if (theta < 0)
	//		nowTheta = 0;*/
	//	eyeTransfer = Vector3(sin(nowTheta), cos(phi) * cos(nowTheta), sin(phi) * cos(nowTheta)) * distance;
	//	eye = Vector3(cos(phi) * cos(nowTheta), sin(nowTheta), sin(phi) * cos(nowTheta)) * distance + target;
	//	viewDirty = true;
	//}

	if ( viewDirty || projectionDirty ) {
		// �Čv�Z�K�v�Ȃ�
		if ( viewDirty ) {
			// �r���[�s��X�V
			UpdateViewMatrix();
			viewDirty = false;
		}

		// �Čv�Z�K�v�Ȃ�
		if ( projectionDirty ) {
			// �r���[�s��X�V
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		// �r���[�v���W�F�N�V�����̍���
		matViewProjection = matView * matProjection;
	}
}

void Camera::UpdateViewMatrix()
{
	// �r���[�s��̍X�V
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3( &eye );
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3( &target );
	// �i���́j�����
	XMVECTOR upVector = XMLoadFloat3( &up );

	// �J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract( targetPosition, eyePosition );
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert( !XMVector3Equal( cameraAxisZ, XMVectorZero() ) );
	assert( !XMVector3IsInfinite( cameraAxisZ ) );
	assert( !XMVector3Equal( upVector, XMVectorZero() ) );
	assert( !XMVector3IsInfinite( upVector ) );
	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize( cameraAxisZ );

	// �J������X���i�E�����j
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross( upVector, cameraAxisZ );
	// �x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize( cameraAxisX );

	// �J������Y���i������j
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross( cameraAxisZ, cameraAxisX );

	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet( 0, 0, 0, 1 );
	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose( matCameraRot );

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate( eyePosition );
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot( cameraAxisX, reverseEyePosition );	// X����
	XMVECTOR tY = XMVector3Dot( cameraAxisY, reverseEyePosition );	// Y����
	XMVECTOR tZ = XMVector3Dot( cameraAxisZ, reverseEyePosition );	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet( tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f );
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet( 0, 0, 0, 1 );
#pragma endregion

#pragma region Y�����r���{�[�h�s��̌v�Z
	// �J����X���AY���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X���͋���
	ybillCameraAxisX = cameraAxisX;
	// Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize( upVector );
	// Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross( ybillCameraAxisX, ybillCameraAxisY );

	// Y�����r���{�[�h�s��
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet( 0, 0, 0, 1 );
#pragma endregion
}

void Camera::UpdateProjectionMatrix()
{
	// ���s���e�ɂ��ˉe�s��̐���
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// �������e�ɂ��ˉe�s��̐���
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

