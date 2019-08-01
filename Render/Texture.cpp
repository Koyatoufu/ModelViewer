#include "Texture.h"

CTexture::CTexture()
	:m_pTextureView(nullptr)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE_D3DCONTENTS(m_pTextureView);
}

HRESULT CTexture::Initiazlie(ID3D11Device * pDevice, std::wstring wStrFileName)
{
	if ( pDevice == nullptr )
		return E_FAIL;

	if ( wStrFileName.empty() )
		return E_FAIL;

	return S_OK;
}
