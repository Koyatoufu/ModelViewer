#pragma once
#include "RenderDefine.h"

class CCamera
{
public:
	CCamera();
	CCamera(const CCamera&);
	~CCamera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Update();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMMATRIX m_viewMatrix;
};
