#pragma once
#include "RenderDefine.h"

class CLight
{
private:
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT4 m_specularColor;

	DirectX::XMFLOAT3 m_direction;
	float m_fSpecularPower;
public:
	CLight();
	~CLight();

	void SetAmibentColor( float r, float g, float b, float a );
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float fPower);
	void SetDirection(float x, float y, float z);

	DirectX::XMFLOAT4 GetAmbientColor() { return m_ambientColor; }
	DirectX::XMFLOAT4 GetDiffuseColor() { return m_diffuseColor; }
	DirectX::XMFLOAT4 GetSpecularColor() { return m_specularColor; }

	DirectX::XMFLOAT3 GetDirection() { return m_direction; }
	float GetSpecularPower() { return m_fSpecularPower; }
};