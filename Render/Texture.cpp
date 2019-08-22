#include "Texture.h"
#include "ImportUtil.h"

CTexture::CTexture()
	:m_pTextureView(nullptr)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE_D3DCONTENTS(m_pTextureView);
}

HRESULT CTexture::Initiazlie(ID3D11Device * pDevice, std::basic_string<TCHAR> strFileName)
{
	if ( pDevice == nullptr )
		return E_FAIL;

	if (strFileName.empty() )
		return E_FAIL;

	DirectX::ScratchImage image = CImportUtil::GetInstance()->LoadTextureFromFile(strFileName);

	if (FAILED(DirectX::CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_pTextureView)))
		return E_FAIL;

	return S_OK;
}

