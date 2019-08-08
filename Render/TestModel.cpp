#include "TestModel.h"
#include "Material.h"

CTestModel::CTestModel()
{
}

CTestModel::~CTestModel()
{
}

HRESULT CTestModel::Initialize(ID3D11Device * pDevice, void * pModelData)
{
	InitBuffers(pDevice, nullptr);

	InitMaterial(pDevice);

	return S_OK;
}

void CTestModel::Update()
{
}

void CTestModel::Render(ID3D11DeviceContext * pDeviceContext)
{
	
}

HRESULT CTestModel::InitBuffers(ID3D11Device * pDevice, void* pModelData)
{	
	SVertexType* parVerticies = NULL;
	unsigned long* parIndices = 0;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	m_nVertexCount = 3;
	m_nIndexCount = 3;

	parVerticies = new SVertexType[m_nVertexCount];
	parIndices = new unsigned long[m_nIndexCount];

	// Load the vertex array with data. 
	parVerticies[0].vPosition = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f); // Bottom left. 
	parVerticies[0].vUV = DirectX::XMFLOAT2(0.0f, 1.0f);
	parVerticies[0].vNormal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	parVerticies[1].vPosition = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // Top middle. 
	parVerticies[1].vUV = DirectX::XMFLOAT2(0.5f, 0.0f);
	parVerticies[1].vNormal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	parVerticies[2].vPosition = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f); // Bottom right.
	parVerticies[2].vUV = DirectX::XMFLOAT2(1.0f, 1.0f);
	parVerticies[2].vNormal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

	// Set up the description of the static vertex buffer. 
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	vertexBufferDesc.ByteWidth = sizeof(SVertexType) * m_nVertexCount; 
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufferDesc.CPUAccessFlags = 0; vertexBufferDesc.MiscFlags = 0; 
	vertexBufferDesc.StructureByteStride = 0; 
	
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = parVerticies; 
	vertexData.SysMemPitch = 0; 
	vertexData.SysMemSlicePitch = 0; 
	
	// Now create the vertex buffer.
	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = parIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
	{
		return E_FAIL;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] parVerticies;
	parVerticies = 0;

	delete[] parIndices;
	parIndices = 0;


	return S_OK;
}

HRESULT CTestModel::InitMaterial(ID3D11Device * pDevice)
{
	CMaterial* pMaterial = new CMaterial();
	//pMaterial->InitOnlyDiffuseTexture();

	m_vecMaterial.push_back(pMaterial);

	return S_OK;
}
