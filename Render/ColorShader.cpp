////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ColorShader.h"
#include <fstream>
#include <tchar.h>

CColorShader::CColorShader()
{
}

CColorShader::~CColorShader()
{
}


HRESULT CColorShader::Initialize(ID3D11Device* device)
{
	// Initialize the vertex and pixel shaders.
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	std::wstring strFileName = L".\\shader\\tutorial\\color.vs";

	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &vertexShaderBuffer, &errorMessage)))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, strFileName.c_str());
		}
		else
		{
			MessageBox(NULL, strFileName.c_str(), L"Missing Shader File", MB_OK);
		}

		return E_FAIL;
	}

	strFileName = L".\\shader\\tutorial\\color.ps";

	// Compile the pixel shader code.
	if (FAILED(D3DCompileFromFile(strFileName.c_str(), NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &pixelShaderBuffer, &errorMessage)))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, strFileName.c_str());
		}
		else
		{
			MessageBox(NULL, strFileName.c_str(), L"Missing Shader File", MB_OK);
		}

		return E_FAIL;
	}

	// Create the vertex shader from the buffer.
	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		return E_FAIL;
	}

	// Create the pixel shader from the buffer.
	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader)))
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

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_pLayout)))
	{
		return E_FAIL;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CColorShader::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, CMaterial* pMaterial,
	MatrixBufferType* pMatrixBuffer, LightBufferType* pLightBuffer, CameraBufferType* pCameraBuffer)
{
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

	return S_OK;
}
