#include "Camera.h"


CCamera::CCamera()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_fPadding = 0.0f;
}


CCamera::CCamera(const CCamera& other)
{
}


CCamera::~CCamera()
{
}


void CCamera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	return;
}


void CCamera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
	return;
}

void CCamera::SetPadding(float fPadding)
{
	m_fPadding = fPadding;
}


DirectX::XMFLOAT3 CCamera::GetPosition()
{
	return m_position;
}


DirectX::XMFLOAT3 CCamera::GetRotation()
{
	return m_rotation;
}

float CCamera::GetPadding()
{
	return m_fPadding;
}


void CCamera::Update()
{
	// Setup the vector that points upwards.
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f,1.0f,0.0f);
	
	// Load it into a XMVECTOR structure.
	DirectX::XMVECTOR upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	DirectX::XMFLOAT3 position = m_position;

	// Load it into a XMVECTOR structure.
	DirectX::XMVECTOR positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	DirectX::XMFLOAT3 lookAt = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);

	// Load it into a XMVECTOR structure.
	DirectX::XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = m_rotation.x * 0.0174532925f;
	float yaw = m_rotation.y * 0.0174532925f;
	float roll = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	//lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}


void CCamera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}