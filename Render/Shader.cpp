#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include "Material.h"

enum EPolygonLayout
{
	EPOLYGONLAYOUT_POSITION = 0,
	//EPOLYGONLAYOUT_COLOR,
	EPOLYGONLAYOUT_TEXCOORD,
	EPOLYGONLAYOUT_NORMAL,
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
	m_pSampleState(nullptr),
	m_eShaderType(ESHADER_NORMAL)

	//m_pVertexShaderBuffer(nullptr),
	//m_pPixelShaderBuffer(nullptr),
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

HRESULT CShader::Initialize(std::basic_string<TCHAR> strName, EShaderType eShaderType)
{
	ID3D11Device* pDevice = CRenderer::Get()->GetDevice();

	if (pDevice == nullptr )
		return E_FAIL;

	ID3D10Blob* pVertexShaderBuffer;
	ID3D10Blob* pPixelShaderBuffer;

	ID3D10Blob* pErrorMessage = nullptr;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[EPOLYGONLAYOUT_MAX];
	unsigned int uNumElements = 0;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_BUFFER_DESC lightBufferDesc;

	std::basic_string<TCHAR> strFileName;

	strFileName = _T(".\\Shader\\") + strName + _T(".vs");

	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "vs_main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pVertexShaderBuffer, &pErrorMessage)))
	{
		if (pErrorMessage)
		{
			OutputShaderErrorMessage(pErrorMessage, strFileName.c_str());
		}
		else
		{
			MessageBox(NULL, strFileName.c_str(), _T("VertexShader NotFound"), MB_OK | MB_ICONERROR);
		}

		return E_FAIL;
	}

	strFileName = _T(".\\Shader\\") + strName + _T(".ps");

	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "ps_main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pPixelShaderBuffer, &pErrorMessage)))
	{
		if (pErrorMessage)
		{
			OutputShaderErrorMessage(pErrorMessage, strFileName.c_str());
		}
		else
		{
			MessageBox(NULL, strFileName.c_str(), _T("PixelShader NotFound"), MB_OK | MB_ICONERROR);
		}

		return E_FAIL;
	}

	if( FAILED(pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, & m_pVertexShader)))
		return E_FAIL;

	if (FAILED(pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader)))
		return E_FAIL;

	polygonLayout[EPOLYGONLAYOUT_POSITION].SemanticName = "POSITION";
	polygonLayout[EPOLYGONLAYOUT_POSITION].SemanticIndex = 0;;
	polygonLayout[EPOLYGONLAYOUT_POSITION].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[EPOLYGONLAYOUT_POSITION].InputSlot = 0;
	polygonLayout[EPOLYGONLAYOUT_POSITION].AlignedByteOffset = 0;
	polygonLayout[EPOLYGONLAYOUT_POSITION].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[EPOLYGONLAYOUT_POSITION].InstanceDataStepRate = 0;

	//polygonLayout[EPOLYGONLAYOUT_COLOR].SemanticName = "COLOR";
	//polygonLayout[EPOLYGONLAYOUT_COLOR].SemanticIndex = 0;;
	//polygonLayout[EPOLYGONLAYOUT_COLOR].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//polygonLayout[EPOLYGONLAYOUT_COLOR].InputSlot = 0;
	//polygonLayout[EPOLYGONLAYOUT_COLOR].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//polygonLayout[EPOLYGONLAYOUT_COLOR].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//polygonLayout[EPOLYGONLAYOUT_COLOR].InstanceDataStepRate = 0;
	
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].SemanticName = "TEXCOORD";
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].SemanticIndex = 0;;
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].InputSlot = 0;
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[EPOLYGONLAYOUT_TEXCOORD].InstanceDataStepRate = 0;

	polygonLayout[EPOLYGONLAYOUT_NORMAL].SemanticName = "NORMAL";
	polygonLayout[EPOLYGONLAYOUT_NORMAL].SemanticIndex = 0;;
	polygonLayout[EPOLYGONLAYOUT_NORMAL].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[EPOLYGONLAYOUT_NORMAL].InputSlot = 0;
	polygonLayout[EPOLYGONLAYOUT_NORMAL].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[EPOLYGONLAYOUT_NORMAL].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[EPOLYGONLAYOUT_NORMAL].InstanceDataStepRate = 0;

	uNumElements = ( sizeof(D3D11_INPUT_ELEMENT_DESC) * EPOLYGONLAYOUT_MAX )/ sizeof(D3D11_INPUT_ELEMENT_DESC);

	SAFE_RELEASE_D3DCONTENTS(pVertexShaderBuffer);
	SAFE_RELEASE_D3DCONTENTS(pPixelShaderBuffer);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; 
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1; 
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; 
	samplerDesc.BorderColor[0] = 0; 
	samplerDesc.BorderColor[1] = 0; 
	samplerDesc.BorderColor[2] = 0; 
	samplerDesc.BorderColor[3] = 0; 
	samplerDesc.MinLOD = 0; 
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if(FAILED(pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState)))
		return E_FAIL;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(SMatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0; matrixBufferDesc.StructureByteStride = 0;

	if (FAILED(pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer)))
		return E_FAIL;

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(SLightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	if (FAILED(pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::SetShaderParameter( DirectX::XMMATRIX matWorld, DirectX::XMMATRIX matView, DirectX::XMMATRIX matProjection, 
	CMaterial* pMaterial, DirectX::XMFLOAT3 vecLightDirection, DirectX::XMFLOAT4 diffuseColor )
{
	ID3D11DeviceContext* pDeviceContext = CRenderer::Get()->GetDeviceContext();

	if (pDeviceContext == nullptr)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SMatrixBufferType* pMatrixDataPtr;
	SLightBufferType* pLightDataPtr;
	unsigned int uBufferNumber = 0;

	matWorld = DirectX::XMMatrixTranspose(matWorld);
	matView = DirectX::XMMatrixTranspose(matView);
	matProjection = DirectX::XMMatrixTranspose(matProjection);

	if(FAILED(pDeviceContext->Map(m_pMatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return E_FAIL;

	pMatrixDataPtr = static_cast<SMatrixBufferType*>(mappedResource.pData);

	pMatrixDataPtr->matWorld = matWorld;
	pMatrixDataPtr->matView = matView;
	pMatrixDataPtr->matProjection = matProjection;

	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(uBufferNumber, 1, &m_pMatrixBuffer);

	//------------------------------------------------------------------------

	if (pMaterial == nullptr)
		return S_OK;

	CTexture* pTexture = pMaterial->GetTexture();

	if (pTexture == nullptr)
		return S_OK;

	ID3D11ShaderResourceView * pTextureView = pTexture->GetTextureView();

	pDeviceContext->PSSetShaderResources( 0, 1 , &pTextureView);

	if (FAILED(pDeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return E_FAIL;

	pLightDataPtr = static_cast<SLightBufferType*>(mappedResource.pData);

	//pLightDataPtr->diffuseColor = diffuseColor;
	//pLightDataPtr->vecLightDirection = vecLightDirection;
	pLightDataPtr->fPadding = 0.0f;

	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	uBufferNumber = 0;

	pDeviceContext->PSSetConstantBuffers(uBufferNumber, 1, &m_pLightBuffer);

	return S_OK;
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

void CShader::Render(ID3D11DeviceContext * pDeviceContext, int nIndexCount)
{
	pDeviceContext->IASetInputLayout(m_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	// Render the triangle.
	pDeviceContext->DrawIndexed(nIndexCount, 0, 0);
}
