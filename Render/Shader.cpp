#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include "Material.h"

enum EPolygonLayout
{
	EPOLYGONLAYOUT_POSITION = 0,
	EPOLYGONLAYOUT_COLOR,
	//EPOLYGONLAYOUT_TEXCOORD,
	//EPOLYGONLAYOUT_NORMAL,
	EPOLYGONLAYOUT_MAX,
};

struct SMatrixBufferType
{
	DirectX::XMMATRIX matWorld;
	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matProjection;
};

struct SLightBufferType
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 vecLightDirection;
	float fPadding;
};

CShader::CShader():
	m_pVertexShader(nullptr),
	m_pPixelShader(nullptr),
	m_pLayout(nullptr),
	m_pMatrixBuffer(nullptr),
	m_pLightBuffer(nullptr),
	m_pSampleState(nullptr)
{
}

CShader::~CShader()
{
	SAFE_RELEASE_D3DCONTENTS(m_pLightBuffer);
	SAFE_RELEASE_D3DCONTENTS(m_pMatrixBuffer);

	SAFE_RELEASE_D3DCONTENTS(m_pSampleState);
	SAFE_RELEASE_D3DCONTENTS(m_pLayout);

	SAFE_RELEASE_D3DCONTENTS(m_pPixelShader);
	SAFE_RELEASE_D3DCONTENTS(m_pVertexShader);
}

void CShader::OutputShaderErrorMessage(ID3D10Blob * pErrorMessage, const WCHAR * wszFileName)
{
	char* szErrorMessage = NULL;
	size_t unBufferSize = 0;
	std::ofstream fout;

	szErrorMessage = static_cast<char*>(pErrorMessage->GetBufferPointer());

	unBufferSize = pErrorMessage->GetBufferSize();

	fout.open("shader-error.txt");
	for (size_t i = 0; i < unBufferSize; ++i)
	{
		fout << szErrorMessage[i];
	}

	fout.close();

	SAFE_RELEASE_D3DCONTENTS(pErrorMessage);

	MessageBox(NULL, _T("Error Compiling Shader"), _T("Error Compiling Shader"), MB_OK | MB_ICONERROR);
}

HRESULT CShader::Render(ID3D11DeviceContext* pDeviceContext, int nIndexCount, CMaterial* pMaterial, MatrixBufferType* pMatrixBuffer , LightBufferType* pLightBuffer )
{
	if (FAILED(SetShaderParameters(pDeviceContext, pMaterial, pMatrixBuffer, pLightBuffer)))
		return E_FAIL;

	//Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, nIndexCount);

	return S_OK;
}

HRESULT CShader::Render(ID3D11DeviceContext * pDeviceContext, int nIndexCount, int nInstanceCount, CMaterial* pMaterial,
	MatrixBufferType* pMatrixBuffer, LightBufferType* pLightBuffer)
{
	if (FAILED(SetShaderParameters(pDeviceContext, pMaterial, pMatrixBuffer, pLightBuffer)))
		return E_FAIL;

	//Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, nIndexCount, nInstanceCount);

	return S_OK;
}

void CShader::RenderShader(ID3D11DeviceContext* pDeviceContext, int nIndexCount)
{
	pDeviceContext->IASetInputLayout(m_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	if(m_pSampleState)
		pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	// Render the triangle.
	pDeviceContext->DrawIndexed(nIndexCount, 0, 0);
}

void CShader::RenderShader(ID3D11DeviceContext * pDeviceContext, int nIndexCount, int nInstanceCount)
{
	pDeviceContext->IASetInputLayout(m_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	if (m_pSampleState)
		pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	// Render the triangle.
	pDeviceContext->DrawIndexedInstanced(nIndexCount, nInstanceCount, 0, 0, 0);
}
