#include "Material.h"

#include "RenderDefine.h"

CMaterial::CMaterial()
{
	for (int i = 0; i < ETEXTURE_MAX; i++)
	{
		m_pArrTexture[i] = nullptr;
	}
}

CMaterial::~CMaterial()
{
	for (int i = 0 ; i < ETEXTURE_MAX; i++)
	{
		SAFE_DELETE(m_pArrTexture[i]);
	}
}

HRESULT CMaterial::InitOnlyDiffuseTexture(ID3D11Device * pDevice, HWND hWnd, std::string strTexFileName)
{
	return S_OK;
}

HRESULT CMaterial::AddTexture(std::string strTexFileName, ETextureType eTextureType)
{
	return S_OK;
}
