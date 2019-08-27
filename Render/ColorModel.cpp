#include "ColorModel.h"
#include "Material.h"
#include "Shader.h"

CColorModel::CColorModel():
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_nVertexCount(0),
	m_nIndexCount(0)
{
}


CColorModel::~CColorModel()
{
	SAFE_RELEASE_D3DCONTENTS(m_pVertexBuffer);
	SAFE_RELEASE_D3DCONTENTS(m_pIndexBuffer);
}


HRESULT CColorModel::Initialize(ID3D11Device* pDevice, ModelData * pModelData)
{
	if (FAILED(InitBuffers(pDevice,pModelData)))
		return E_FAIL;

	return S_OK;
}

void CColorModel::Render(ID3D11DeviceContext * pDeviceContext, MatrixBufferType * pMatrixBuffer, LightBufferType * pLightBuffer, CameraBufferType * pCameraBuffer)
{
	RenderBuffers(pDeviceContext);

	if (m_pShader)
	{
		m_pShader->Render(pDeviceContext, m_nIndexCount, m_vtMaterial[0], pMatrixBuffer, pLightBuffer, pCameraBuffer);
	}
}


void CColorModel::Update()
{
}


HRESULT CColorModel::InitBuffers(ID3D11Device* pDevice, ModelData* pModelData)
{
	ColorVertexType* pVertices;
	unsigned long* pIndices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set the number of vertices in the vertex array.
	m_nVertexCount = 4;

	// Set the number of indices in the index array.
	m_nIndexCount = 6;

	// Create the vertex array.
	pVertices = new ColorVertexType[m_nVertexCount];
	// Create the index array.
	pIndices = new unsigned long[m_nIndexCount];

	// Load the vertex array with data.
	pVertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	pVertices[0].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	pVertices[1].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left.
	pVertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	pVertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	pVertices[2].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	pVertices[3].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top Right.
	pVertices[3].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	pIndices[0] = 0;  // Bottom left.
	pIndices[1] = 1;  // Top left.
	pIndices[2] = 2;  // Bottom right.
	pIndices[3] = 2; 
	pIndices[4] = 1; 
	pIndices[5] = 3; 

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorVertexType) * m_nVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
		return E_FAIL;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
		return E_FAIL;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	SAFE_DELETE_ARRAY(pVertices);
	SAFE_DELETE_ARRAY(pIndices);

	return S_OK;
}

HRESULT CColorModel::InitMaterial(ID3D11Device * pDevice, ModelData* pModelData)
{
	return S_OK;
}


void CColorModel::RenderBuffers(ID3D11DeviceContext* pDeviceContext)
{
	unsigned int unStride;
	unsigned int unOffset;


	// Set vertex buffer stride and offset.
	unStride = sizeof(ColorVertexType);
	unOffset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &unStride, &unOffset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}