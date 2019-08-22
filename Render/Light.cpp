#include "Light.h"

CLight::CLight():
	m_ambientColor(0.0f,0.0f,0.0f,1.0f),
	m_diffuseColor(0.0f, 0.0f, 0.0f, 1.0f),
	m_specularColor(0.0f, 0.0f, 0.0f, 1.0f),
	m_direction(0.0f, 0.0f, 0.0f),
	m_fSpecularPower(0.0f)
{
}

CLight::~CLight()
{
}

void CLight::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor.x = r;
	m_ambientColor.y = g;
	m_ambientColor.z = b;
	m_ambientColor.w = a;
}

void CLight::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor.x = r;
	m_diffuseColor.y = g;
	m_diffuseColor.z = b;
	m_diffuseColor.w = a;
}

void CLight::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor.x = r;
	m_specularColor.y = g;
	m_specularColor.z = b;
	m_specularColor.w = a;
}

void CLight::SetSpecularPower(float fPower)
{
	m_fSpecularPower = fPower;
}

void CLight::SetDirection(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = y;
	m_direction.z = z;
}
