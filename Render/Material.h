#pragma once
#include "Texture.h"

class CMaterial
{
private:
	CTexture* m_pArrTexture[ETextureType::ETEXTURE_MAX]; // ETextureType ¹è¿­
public:
	CMaterial();
	~CMaterial();

	HRESULT InitOnlyDiffuseTexture(ID3D11Device* pDevice, HWND hWnd , std::string strTexFileName );

	HRESULT AddTexture(std::string strTexFileName, ETextureType eTextureType);

	CTexture* GetTexture(ETextureType eTextureType) { return m_pArrTexture[eTextureType]; }
};