#include "Camera.h"

CCamera::CCamera()
{
	m_matView = DirectX::XMMatrixIdentity();
}

CCamera::~CCamera()
{
}

void CCamera::SetPosition(float x, float y, float z)
{
	m_vecPosition.x = x;
	m_vecPosition.y = y;
	m_vecPosition.z = z;
}

void CCamera::SetRotation(float x, float y, float z)
{
	m_vecRotation.x = x;
	m_vecRotation.y = y;
	m_vecRotation.z = z;
}

void CCamera::SetLookPosition(float x, float y, float z)
{
	m_vecLook.x = x;
	m_vecLook.y = y;
	m_vecLook.z = z;
}

void CCamera::Update()
{
	float fYaw = 0.0f, fPitch = 0.0f, fRoll = 0.0f;

	DirectX::XMFLOAT3 f3Up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR vecUp = DirectX::XMLoadFloat3(&f3Up);
	DirectX::XMVECTOR vecLook = DirectX::XMLoadFloat3(&m_vecLook);
	DirectX::XMVECTOR vecPos = DirectX::XMLoadFloat3(&m_vecPosition);

	DirectX::XMMATRIX matRotation;

	matRotation = DirectX::XMMatrixRotationRollPitchYaw(fPitch,fYaw,fRoll);

	vecLook = DirectX::XMVector3TransformCoord(vecLook,matRotation);
	vecUp = DirectX::XMVector3TransformCoord(vecUp,matRotation);

	m_matView = DirectX::XMMatrixLookAtLH(vecPos, vecLook, vecUp);
}
