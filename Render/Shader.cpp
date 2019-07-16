#include "Shader.h"

CShader::CShader():
	m_pVertexShader(nullptr),
	m_pPixelShader(nullptr),
	m_pLayout(nullptr),
	m_pMatrixBuffer(nullptr),
	m_pSampleState(nullptr),
	m_pVertexShaderBuffer(nullptr),
	m_pPixelShaderBuffer(nullptr),
	m_eShaderType(ESHADER_NORMAL)
{
}

CShader::~CShader()
{
}

HRESULT CShader::Initialize(std::wstring wstrFileName, EShaderType eShaderType)
{
	return S_OK;
}

HRESULT CShader::SetShaderParameter( DirectX::FXMMATRIX matWorld, DirectX::FXMMATRIX matView, DirectX::FXMMATRIX matProj, ID3D11ShaderResourceView * pTextureView	)
{
	return S_OK;
}

void CShader::OutputShaderErrorMessage(ID3D10Blob * pErrorMessage, WCHAR * wszFileName)
{
}
