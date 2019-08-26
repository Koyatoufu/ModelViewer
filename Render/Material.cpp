#include "Material.h"

#include "ImportUtil.h"

CMaterial::CMaterial()
{
	for (int i = 0; i < ETEXTURE_MAX; i++)
	{
		m_pArrTextureView[i] = nullptr;
	}
}

CMaterial::~CMaterial()
{
	for (int i = 0 ; i < ETEXTURE_MAX; i++)
	{
		SAFE_RELEASE_D3DCONTENTS(m_pArrTextureView[i]);
	}
}

HRESULT CMaterial::Initialize(DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 specularColor)
{
	m_diffuseColor = diffuseColor;
	m_ambientColor = ambientColor;
	m_specularColor = specularColor;

	return S_OK;
}

HRESULT CMaterial::AddTexture(ID3D11Device* pDevice, std::basic_string<TCHAR> strTexFileName, ETextureType eTextureType)
{
	if ( pDevice == nullptr )
		return E_FAIL;

	if (strTexFileName.empty())
		return E_FAIL;

	if (eTextureType == ETEXTURE_MAX)
		return E_FAIL;

	if (m_pArrTextureView[eTextureType] != nullptr)
	{
		SAFE_DELETE(m_pArrTextureView[eTextureType]);
	}

	ID3D11ShaderResourceView* pTextureView = LoadTextureView(pDevice, strTexFileName);

	m_pArrTextureView[eTextureType] = pTextureView;

	return S_OK;
}

void CMaterial::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor.x = r;
	m_diffuseColor.y = g;
	m_diffuseColor.z = b;
	m_diffuseColor.w = a;
}

void CMaterial::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor.x = r;
	m_ambientColor.y = g;
	m_ambientColor.z = b;
	m_ambientColor.w = a;
}

void CMaterial::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor.x = r;
	m_specularColor.y = g;
	m_specularColor.z = b;
	m_specularColor.w = a;
}

ID3D11ShaderResourceView * CMaterial::LoadTextureView(ID3D11Device * pDevice, std::basic_string<TCHAR> strTexFileName)
{
	if (pDevice == nullptr)
		return nullptr;

	if (strTexFileName.empty())
		return nullptr;

	ID3D11ShaderResourceView* pTextureVIew = nullptr;

	DirectX::ScratchImage image = CImportUtil::GetInstance()->LoadTextureFromFile(strTexFileName);

	if (FAILED(DirectX::CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &pTextureVIew)))
	{
		SAFE_RELEASE_D3DCONTENTS(pTextureVIew);

		return nullptr;
	}

	return pTextureVIew;
}
