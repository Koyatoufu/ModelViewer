#include "LightModel.h"
#include "ImportUtil.h"
#include "Material.h"

CLightModel::CLightModel()
{
}

CLightModel::~CLightModel()
{
}

HRESULT CLightModel::Initialize(ID3D11Device * pDevice, ModelData * pModelData)
{
	pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\cube.txt"));

	if (pModelData == nullptr)
		return E_FAIL;

	m_pModelData = pModelData;

	//pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\"));

	if(FAILED(InitMaterial(pDevice, pModelData)))
		return E_FAIL;

	if(FAILED(InitBuffers(pDevice, pModelData)))
		return E_FAIL;

	return S_OK;
}

void CLightModel::Render(ID3D11DeviceContext * pDeviceContext)
{
	RenderBuffers(pDeviceContext);
}

void CLightModel::Update()
{
}

HRESULT CLightModel::InitBuffers(ID3D11Device * pDevice, ModelData * pModelData)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	if ( pModelData == nullptr || pModelData->vtMeshes.size() < 1 )
		return E_FAIL;

	m_nVertexCount = pModelData->vtMeshes[0]->vtPositions.size();
	m_nIndexCount = pModelData->vtMeshes[0]->vtIndexDatas.size();

	if (pModelData->vtMeshes[0]->vtPositions.size() != m_nVertexCount ||
		pModelData->vtMeshes[0]->vtUVs.size() != m_nVertexCount ||
		pModelData->vtMeshes[0]->vtNormals.size() != m_nVertexCount)
		return E_FAIL;

	if (pModelData->vtMeshes[0]->vtIndexDatas.size() < 1)
		return E_FAIL;

	// Create the vertex array.
	vertices = new VertexType[m_nVertexCount];
	// Create the index array.
	indices = new unsigned long[m_nIndexCount];

	// Load the vertex array and index array with data.
	for (i = 0; i < m_nVertexCount; i++)
	{
		vertices[i].position = pModelData->vtMeshes[0]->vtPositions[i];
		vertices[i].UV = pModelData->vtMeshes[0]->vtUVs[i];
		vertices[i].normal = pModelData->vtMeshes[0]->vtNormals[i];
	}

	for (i = 0; i < m_nIndexCount; i++)
	{
		indices[i] = pModelData->vtMeshes[0]->vtIndexDatas[i];
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_nVertexCount;
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
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	return S_OK;
}

HRESULT CLightModel::InitMaterial(ID3D11Device * pDevice, ModelData * pModelData)
{
	bool bModelLoad = false;

	if (pModelData->pMaterialData)
	{
		for (int i = 0; i < pModelData->pMaterialData->vtMaterialInfo.size(); i++)
		{
			MaterialData::MaterialInfo* pInfo = pModelData->pMaterialData->vtMaterialInfo[i];

			if (pInfo == nullptr)
				continue;

			CMaterial* pMaterial = new CMaterial;

			DirectX::XMFLOAT4 ambient = pInfo->ambient;
			DirectX::XMFLOAT4 diffuse = pInfo->ambient;
			DirectX::XMFLOAT4 specular = pInfo->ambient;

			pMaterial->Initialize(diffuse, ambient, specular);

			if (pInfo->strDiffuseMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, pInfo->strDiffuseMap);
			}
			if (pInfo->strNormalMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, pInfo->strNormalMap, ETEXTURE_NORMAL);
			}
			if (pInfo->strSpecularMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, pInfo->strSpecularMap, ETEXTURE_SPECULAR);
			}

			m_vecMaterial.push_back(pMaterial);
		}

		if (m_vecMaterial.size() > 0)
			bModelLoad = true;for (int i = 0; i < pModelData->pMaterialData->vtMaterialInfo.size(); i++)
		{
			MaterialData::MaterialInfo* pInfo = pModelData->pMaterialData->vtMaterialInfo[i];

			if (pInfo == nullptr)
				continue;

			CMaterial* pMaterial = new CMaterial;

			DirectX::XMFLOAT4 ambient = pInfo->ambient;
			DirectX::XMFLOAT4 diffuse = pInfo->ambient;
			DirectX::XMFLOAT4 specular = pInfo->ambient;

			pMaterial->Initialize(diffuse, ambient, specular);

			if (pInfo->strDiffuseMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, pInfo->strDiffuseMap);
			}
			if (pInfo->strNormalMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, pInfo->strNormalMap, ETEXTURE_NORMAL);
			}
			if (pInfo->strSpecularMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, pInfo->strSpecularMap, ETEXTURE_SPECULAR);
			}

			m_vecMaterial.push_back(pMaterial);
		}

		if (m_vecMaterial.size() > 0)
			bModelLoad = true;
	}

	if (bModelLoad == false)
	{
		CMaterial* pMaterial = new CMaterial();

		DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f);
		DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

		pMaterial->Initialize(diffuse, ambient, specular);

		pMaterial->AddTexture(pDevice, _T(".\\res\\texture\\seafloor.dds"));

		m_vecMaterial.push_back(pMaterial);
	}

	return S_OK;
}

void CLightModel::RenderBuffers(ID3D11DeviceContext * pDeviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
