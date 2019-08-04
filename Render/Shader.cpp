#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>

enum EPolygonLayout
{
	EPOLYGONLAYOUT_POSITION = 0,
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

CShader::CShader():
	m_pVertexShader(nullptr),
	m_pPixelShader(nullptr),
	m_pLayout(nullptr),
	m_pMatrixBuffer(nullptr),
	m_pSampleState(nullptr),
	m_eShaderType(ESHADER_NORMAL)

	//m_pVertexShaderBuffer(nullptr),
	//m_pPixelShaderBuffer(nullptr),
{
}

CShader::~CShader()
{
}

HRESULT CShader::Initialize(std::wstring wstrFileName, EShaderType eShaderType)
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

	if (FAILED(D3DCompileFromFile(wstrFileName.c_str(), NULL, NULL, "vs_main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pVertexShaderBuffer, &pErrorMessage)))
	{
		if (pErrorMessage)
		{
			OutputShaderErrorMessage(pErrorMessage, wstrFileName.c_str());
		}

		return E_FAIL;
	}	

	if (FAILED(D3DCompileFromFile(wstrFileName.c_str(), NULL, NULL, "ps_main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pPixelShaderBuffer, &pErrorMessage)))
	{
		if (pErrorMessage)
		{
			OutputShaderErrorMessage(pErrorMessage, wstrFileName.c_str());
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

	if (pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::SetShaderParameter( DirectX::XMMATRIX matWorld, DirectX::XMMATRIX matView, DirectX::XMMATRIX matProjection, ID3D11ShaderResourceView * pTextureView	)
{
	ID3D11DeviceContext* pDeviceContext = CRenderer::Get()->GetDeviceContext();

	if (pDeviceContext == nullptr)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SMatrixBufferType* pDataPtr;
	unsigned int uBufferNumber = 0;

	matWorld = DirectX::XMMatrixTranspose(matWorld);
	matView = DirectX::XMMatrixTranspose(matView);
	matProjection = DirectX::XMMatrixTranspose(matProjection);

	if(FAILED(pDeviceContext->Map(m_pMatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return E_FAIL;

	pDataPtr = static_cast<SMatrixBufferType*>(mappedResource.pData);

	pDataPtr->matWorld = matWorld;
	pDataPtr->matView = matView;
	pDataPtr->matProjection = matProjection;

	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(uBufferNumber, 1, &m_pMatrixBuffer);

	//------------------------------------------------------------------------

	pDeviceContext->PSSetShaderResources( 0, 1 , &pTextureView);

	//if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	//	return E_FAIL;

	//pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	return S_OK;
}

void CShader::OutputShaderErrorMessage(ID3D10Blob * pErrorMessage, const WCHAR * wszFileName)
{
	char* szErrorMessage = NULL;
	unsigned long uBufferSize = 0;
	std::ofstream fout;


}
