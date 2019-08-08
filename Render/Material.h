#pragma once
#include "Texture.h"

class CMaterial
{
private:
	CTexture* m_pArrTexture[ETextureType::ETEXTURE_MAX]; // ETextureType ¹è¿­

	DirectX::XMFLOAT3 m_vecDiffuseColor;
public:
	CMaterial();
	~CMaterial();

	HRESULT InitOnlyDiffuseTexture(ID3D11Device* pDevice, HWND hWnd , std::string strTexFileName );

	HRESULT AddTexture(std::string strTexFileName, ETextureType eTextureType);

	CTexture* GetTexture(ETextureType eTextureType) { return m_pArrTexture[eTextureType]; }

	DirectX::XMFLOAT3 GetDiffuseColor() { return m_vecDiffuseColor; }
	void SetDiffuseColor(float r, float g, float b) { m_vecDiffuseColor.x = r; m_vecDiffuseColor.y = g; m_vecDiffuseColor.z = b; }
};