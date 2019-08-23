#include "LightShader.h"
#include "Material.h"

CLightShader::CLightShader()
{
}

CLightShader::~CLightShader()
{
}

HRESULT CLightShader::Initialize(ID3D11Device * pDevice)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	std::wstring strFileName = L".\\shader\\tutorial\\light.vs";

	// Compile the vertex shader code.
	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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

	strFileName = L".\\shader\\tutorial\\light.ps";

	// Compile the pixel shader code.
	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
	result = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// Create the pixel shader from the buffer.
	result = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		return E_FAIL;
	}

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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_pLayout);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

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
	result = pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&cameraBufferDesc, NULL, &m_pCameraBuffer);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLightShader::SetShaderParameters(ID3D11DeviceContext * pDeviceContext, CMaterial * pMaterial,
	MatrixBufferType * pMatrixBuffer, LightBufferType * pLightBuffer, CameraBufferType* pCameraBuffer)
{
	if (!pMatrixBuffer || !pLightBuffer || !pCameraBuffer)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr = nullptr;
	LightBufferType* dataPtr2 = nullptr;
	CameraBufferType* dataPtr3 = nullptr;

	// Transpose the matrices to prepare them for the shader.
	pMatrixBuffer->world = XMMatrixTranspose(pMatrixBuffer->world);
	pMatrixBuffer->view = XMMatrixTranspose(pMatrixBuffer->view);
	pMatrixBuffer->projection = XMMatrixTranspose(pMatrixBuffer->projection);

	// Lock the constant buffer so it can be written to.
	if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return E_FAIL;
	}

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

	// Now set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pMatrixBuffer);

	// Lock the camera constant buffer so it can be written to.
	if (FAILED(pDeviceContext->Map(m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return E_FAIL;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (CameraBufferType*)mappedResource.pData;

	// Copy the camera position into the constant buffer.
	dataPtr3->cameraPosition = pCameraBuffer->cameraPosition;
	dataPtr3->padding = 0.0f;

	// Unlock the camera constant buffer.
	pDeviceContext->Unmap(m_pCameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pCameraBuffer);

	if (pMaterial && pMaterial->GetTexture() && pMaterial->GetTexture()->GetTextureView())
	{
		ID3D11ShaderResourceView* pTextureView = pMaterial->GetTexture()->GetTextureView();

		// Set shader texture resource in the pixel shader.
		pDeviceContext->PSSetShaderResources(0, 1, &pTextureView);
	}

	// Lock the light constant buffer so it can be written to.
	if (FAILED(pDeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return E_FAIL;
	}

	// Get a pointer to the data in the light constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the light constant buffer.
	dataPtr2->ambientColor = pLightBuffer->ambientColor;
	dataPtr2->diffuseColor = pLightBuffer->diffuseColor;
	dataPtr2->lightDirection = pLightBuffer->lightDirection;
	dataPtr2->specularColor = pLightBuffer->specularColor;
	dataPtr2->specularPower = pLightBuffer->specularPower;

	// Unlock the light constant buffer.
	pDeviceContext->Unmap(m_pLightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pLightBuffer);

	return S_OK;
}
