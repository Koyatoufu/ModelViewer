#pragma once
#include "Texture.h"

class CMaterial
{
private:
	CTexture* m_pArrTexture[ETextureType::ETEXTURE_MAX]; // ETextureType ¹è¿­

	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT4 m_specularColor;

public:
	CMaterial();
	~CMaterial();

	HRESULT Initialize(DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 specularColor );

	HRESULT AddTexture(ID3D11Device* pDevice, std::basic_string<TCHAR> strTexFileName, ETextureType eTextureType = ETEXTURE_DIFFUSE);

	CTexture* GetTexture(ETextureType eTextureType = ETEXTURE_DIFFUSE) { return m_pArrTexture[eTextureType]; }

	DirectX::XMFLOAT4 GetDiffuseColor() { return m_diffuseColor; }
	void SetDiffuseColor(float r, float g, float b, float a = 1.f);

	DirectX::XMFLOAT4 GetAmbientColor() { return m_ambientColor; }
	void SetAmbientColor(float r, float g, float b, float a = 1.f);

	DirectX::XMFLOAT4 GetSpecularColor() { return m_specularColor; }
	void SetSpecularColor(float r, float g, float b, float a = 1.f);

};