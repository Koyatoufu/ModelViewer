#pragma once
#include "RenderDefine.h"

class CCamera
{
private:
	DirectX::XMFLOAT3 m_vecPosition;
	DirectX::XMFLOAT3 m_vecRotation;
	DirectX::XMFLOAT3 m_vecLook;

	DirectX::XMMATRIX m_matView;
public:
	CCamera();
	~CCamera();

	void SetPosition(DirectX::XMFLOAT3 vecPosition) { m_vecPosition = vecPosition; }
	void SetPosition(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 vecRotation) { m_vecRotation = vecRotation; }
	void SetRotation(float x, float y, float z);
	void SetLookPosition(DirectX::XMFLOAT3 vecLook) { m_vecLook = vecLook; }
	void SetLookPosition(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition() { return m_vecPosition; }
	DirectX::XMFLOAT3 GetRotation() { return m_vecRotation; }

	DirectX::XMMATRIX& GetViewMatrix() { return m_matView; }

	void Update();
};