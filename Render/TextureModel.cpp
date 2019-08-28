#include "TextureModel.h"
#include "Material.h"
#include "Shader.h"

CTextureModel::CTextureModel():
	m_bInstnceUse(false),
	m_pInstanceBuffer(nullptr),
	m_nInstanceCount(0)
{
}

CTextureModel::~CTextureModel()
{
	SAFE_RELEASE_D3DCONTENTS(m_pInstanceBuffer);
}

HRESULT CTextureModel::Initialize(ID3D11Device * pDevice, ModelData * pModelData)
{
	if (FAILED(InitMaterial(pDevice, pModelData)))
		return E_FAIL;

	if (FAILED(InitBuffers(pDevice, pModelData)))
		return E_FAIL;

	return S_OK;
}

void CTextureModel::Render(ID3D11DeviceContext * pDeviceContext, MatrixBufferType * pMatrixBuffer, LightBufferType * pLightBuffer, CameraBufferType * pCameraBuffer)
{
	RenderBuffers(pDeviceContext);

	if (m_pShader)
	{
		m_pShader->Render(pDeviceContext, m_nIndexCount, m_nInstanceCount, GetMaterial(), pMatrixBuffer, pLightBuffer, pCameraBuffer);
	}
}

void CTextureModel::Update()
{
}

void CTextureModel::RenderBuffers(ID3D11DeviceContext *pDeviceContext, ModelSubsets* pSubset)
{
	if (m_bInstnceUse)
	{
		unsigned int arStrides[2];
		unsigned int arOffsets[2];
		ID3D11Buffer* bufferPointers[2];


		// Set the buffer strides.
		arStrides[0] = sizeof(TextureVertexType);
		arStrides[1] = sizeof(InstanceType);

		// Set the buffer offsets.
		arOffsets[0] = 0;
		arOffsets[1] = 0;

		// Set the array of pointers to the vertex and instance buffers.
		bufferPointers[0] = m_pVertexBuffer;
		bufferPointers[1] = m_pInstanceBuffer;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		pDeviceContext->IASetVertexBuffers(0, 2, bufferPointers, arStrides, arOffsets);
	}
	else
	{
		unsigned int unStride;
		unsigned int unOffset;

		// Set vertex buffer stride and offset.
		unStride = sizeof(TextureVertexType);
		unOffset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &unStride, &unOffset);
	}

	// Set the index buffer to active in the input assembler so it can be rendered.
	if(m_pIndexBuffer)
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

HRESULT CTextureModel::InitBuffers(ID3D11Device * pDevice, ModelData * pModelData)
{
	TextureVertexType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	// Set the number of vertices in the vertex array.
	m_nVertexCount = 4;

	// Create the vertex array.
	vertices = new TextureVertexType[m_nVertexCount];
	if (!vertices)
		return E_FAIL;

	CMaterial* pMaterial = GetMaterial();

	// Load the vertex array with data.
	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = pMaterial ? pMaterial->GetDiffuseColor(): DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f);
	vertices[0].UV = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = pMaterial ? pMaterial->GetDiffuseColor() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].UV = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = pMaterial ? pMaterial->GetDiffuseColor() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].UV = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);  // Bottom right.
	vertices[3].color = pMaterial ? pMaterial->GetDiffuseColor() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	vertices[3].UV = DirectX::XMFLOAT2(1.0f, 0.0f);

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TextureVertexType) * m_nVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(vertices);

	m_nInstanceCount = 4;

	InstanceType* pInstances = nullptr;
	pInstances = new InstanceType[m_nInstanceCount];

	pInstances[0].position = DirectX::XMFLOAT3(-1.5f, -1.5f, 5.0f);
	pInstances[1].position = DirectX::XMFLOAT3(-1.5f, 1.5f, 5.0f);
	pInstances[2].position = DirectX::XMFLOAT3(1.5f, -1.5f, 5.0f);
	pInstances[3].position = DirectX::XMFLOAT3(1.5f, 1.5f, 5.0f);

	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_nInstanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = pInstances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&instanceBufferDesc, &instanceData, &m_pInstanceBuffer)))
	{
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(pInstances);

	unsigned long* indices;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set the number of indices in the index array.
	m_nIndexCount = 6;

	// Create the index array.
	indices = new unsigned long[m_nIndexCount];
	if (!indices)
	{
		return E_FAIL;
	}

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
	{
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(indices);

	m_bInstnceUse = true;

	return S_OK;
}

HRESULT CTextureModel::InitMaterial(ID3D11Device * pDevice, ModelData* pModelData)
{
	CMaterial* pMaterial = new CMaterial();

	DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f);
	DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	if (FAILED(pMaterial->Initialize(diffuse, ambient, specular)))
	{
		SAFE_DELETE(pMaterial);
		return E_FAIL;
	}

	if (FAILED(pMaterial->AddTexture(pDevice, L".\\res\\texture\\stone01.tga")))
	{
		SAFE_DELETE(pMaterial);
		return E_FAIL;
	}

	pMaterial->SetName(_T("Basic"));

	m_mapMaterial.insert(std::pair<std::basic_string<TCHAR>, CMaterial*>(_T("Basic"), pMaterial));

	return S_OK;
}
