#include "Texture.h"

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

	DirectX::ScratchImage image = LoadTextureFromFile(strFileName);

	HRESULT hResult = DirectX::CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_pTextureView);

	if (FAILED(hResult))
		return E_FAIL;

	return S_OK;
}

DirectX::ScratchImage CTexture::LoadTextureFromFile(std::basic_string<TCHAR> strFileName)
{
	DirectX::ScratchImage image;

	TCHAR szExt[256];
	_tsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 256);
	
	if (_tcsicmp(szExt, _T(".dds")) == 0)
	{
		DirectX::LoadFromDDSFile(strFileName.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_tcsicmp(szExt, _T(".tga")) == 0)
	{
		DirectX::LoadFromTGAFile(strFileName.c_str(), nullptr, image);
	}
	else if (_tcsicmp(szExt, _T(".wic")) == 0)
	{
		DirectX::LoadFromWICFile(strFileName.c_str(), DirectX::WIC_FLAGS_NONE , nullptr, image);
	}

	return image;
}
