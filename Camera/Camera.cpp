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

