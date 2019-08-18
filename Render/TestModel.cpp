#include "TestModel.h"
#include "Material.h"
#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"

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

	m_pShader = new CShader();
	if (FAILED(m_pShader->Initialize(_T("Test"))))
	{
		;
	}

	SetPosition(0.0f, 0.0f, 0.0f);

	return S_OK;
}

void CTestModel::Update()
{
	CRenderer* pRenderer = CRenderer::Get();

	if (pRenderer)
	{
		CCamera* pCamera = pRenderer->GetRendererCamera();

		//pCamera->SetLookPosition(GetPosition());

		if (m_pShader)
		{
			DirectX::XMMATRIX matWorld = pRenderer->GetWorld();
			DirectX::XMMATRIX matProj = pRenderer->GetProjection();

			DirectX::XMMATRIX matView = pCamera->GetViewMatrix();

			m_pShader->SetShaderParameter(matWorld, matView, matProj,m_vecMaterial[0]);
		}

	}
}

void CTestModel::Render(ID3D11DeviceContext * pDeviceContext)
{
	unsigned int unStride;
	unsigned int unOffset;

	// Set vertex buffer stride and offset.
	unStride = sizeof(SVertexType);
	unOffset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &unStride, &unOffset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_pShader)
		m_pShader->Render(pDeviceContext, m_nIndexCount);

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
	parVerticies[0].vColor = DirectX::XMFLOAT4(0.0f,1.0f,0.0f,1.0f); 
	//parVerticies[0].vUV = DirectX::XMFLOAT2(0.0f, 1.0f);
	//parVerticies[0].vNormal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	parVerticies[1].vPosition = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // Top middle. 
	parVerticies[1].vColor = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//parVerticies[1].vUV = DirectX::XMFLOAT2(0.5f, 0.0f);
	//parVerticies[1].vNormal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	parVerticies[2].vPosition = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f); // Bottom right.
	parVerticies[2].vColor = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//parVerticies[2].vUV = DirectX::XMFLOAT2(1.0f, 1.0f);
	//parVerticies[2].vNormal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

	// Set up the description of the static vertex buffer. 
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT; 
	vertexBufferDesc.ByteWidth = sizeof(SVertexType) * m_nVertexCount; 
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufferDesc.CPUAccessFlags = 0; 
	vertexBufferDesc.MiscFlags = 0; 
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

	DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
	DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	pMaterial->Initialize(diffuse, ambient, specular);

	m_vecMaterial.push_back(pMaterial);

	return S_OK;
}
