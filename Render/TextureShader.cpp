#include "TextureShader.h"
#include "Material.h"

CTextureShader::CTextureShader()
{
}

CTextureShader::~CTextureShader()
{
}

HRESULT CTextureShader::Initialize(ID3D11Device * pDevice)
{
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	std::wstring strFileName = L".\\shader\\tutorial\\texture.vs";

	// Compile the vertex shader code.
	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage)))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, strFileName.c_str());
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(NULL, strFileName.c_str(), L"Missing Shader File", MB_OK);
		}

		return E_FAIL;
	}

	strFileName = L".\\shader\\tutorial\\texture.ps";

	// Compile the pixel shader code.
	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage)))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, strFileName.c_str());
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(NULL, strFileName.c_str(), L"Missing Shader File", MB_OK);
		}

		return E_FAIL;
	}

	// Create the vertex shader from the buffer.
	if (FAILED(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		return E_FAIL;
	}

	// Create the pixel shader from the buffer.
	if (FAILED(pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 1;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 1;
	polygonLayout[2].AlignedByteOffset = 0;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	polygonLayout[2].InstanceDataStepRate = 1;

	polygonLayout[3].SemanticName = "COLOR";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	if (FAILED(pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_pLayout)))
	{
		return E_FAIL;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	SAFE_RELEASE_D3DCONTENTS(vertexShaderBuffer);
	SAFE_RELEASE_D3DCONTENTS(pixelShaderBuffer);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer)))
	{
		return E_FAIL;
	}

	// Create a texture sampler state description.
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

	// Create the texture sampler state.
	if (FAILED(pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTextureShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, CMaterial* pMaterial,
	MatrixBufferType* pMatrixBuffer, LightBufferType* pLightBuffer, CameraBufferType* pCameraBuffer)
{
	if (pMatrixBuffer == nullptr)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	pMatrixBuffer->world = XMMatrixTranspose(pMatrixBuffer->world);
	pMatrixBuffer->view = XMMatrixTranspose(pMatrixBuffer->view);
	pMatrixBuffer->projection = XMMatrixTranspose(pMatrixBuffer->projection);

	// Lock the constant buffer so it can be written to.
	if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return E_FAIL;

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = pMatrixBuffer->world;
	dataPtr->view = pMatrixBuffer->view;
	dataPtr->projection = pMatrixBuffer->projection;

	// Unlock the constant buffer.
	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	if (pMaterial && pMaterial->GetTexture())
	{
		CTexture* pTexture = pMaterial->GetTexture();
		ID3D11ShaderResourceView* pTextureView = pTexture->GetTextureView();

		if (pTextureView)
		{
			// Set shader texture resource in the pixel shader.
			pDeviceContext->PSSetShaderResources(0, 1, &pTextureView);
		}
	}

	return S_OK;
}
