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

HRESULT CMaterial::Initialize(DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 specularColor)
{
	m_diffuseColor = diffuseColor;
	m_ambientColor = ambientColor;
	m_specularColor = specularColor;

	return S_OK;
}

HRESULT CMaterial::AddTexture(ID3D11Device* pDevice, HWND hWnd, std::basic_string<TCHAR> strTexFileName, ETextureType eTextureType)
{
	if ( pDevice == nullptr || hWnd == nullptr )
		return E_FAIL;

	if (strTexFileName.empty())
		return E_FAIL;

	if (eTextureType == ETEXTURE_MAX)
		return E_FAIL;

	if (m_pArrTexture[eTextureType] != nullptr)
	{
		SAFE_DELETE(m_pArrTexture[eTextureType]);
	}

	CTexture* pTexture = new CTexture();
	pTexture->Initiazlie(pDevice, strTexFileName);

	m_pArrTexture[eTextureType] = pTexture;

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
