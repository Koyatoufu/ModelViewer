#include "LightModel.h"
#include "ImportUtil.h"
#include "Material.h"
#include "Shader.h"

CLightModel::CLightModel():
	m_fTestRotation(0.0f)
{
}

CLightModel::~CLightModel()
{
}

HRESULT CLightModel::Initialize(ID3D11Device * pDevice, ModelData * pModelData)
{
	//pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\cube.txt"));
	//pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\cube.obj"));
	pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\sphere.obj"));

	if (pModelData == nullptr)
		return E_FAIL;

	m_pModelData = pModelData;
		
	if(FAILED(InitMaterial(pDevice, m_pModelData)))
		return E_FAIL;

	if(FAILED(InitBuffers(pDevice, m_pModelData)))
		return E_FAIL;

	return S_OK;
}

void CLightModel::Render(ID3D11DeviceContext * pDeviceContext, MatrixBufferType * pMatrixBuffer, LightBufferType * pLightBuffer, CameraBufferType * pCameraBuffer)
{
	DirectX::XMMATRIX matRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_fTestRotation));
	pMatrixBuffer->world = pMatrixBuffer->world * matRot;

	for (size_t i = 0; i < m_vtSubsets.size(); ++i)
	{
		ModelSubsets* pSubSet = m_vtSubsets[i];

		if (pSubSet == nullptr)
			continue;

		RenderBuffers(pDeviceContext, pSubSet);

		if (m_pShader)
		{
			CMaterial* pMaterial = GetMaterial(pSubSet->strMtlName);

			if (pMaterial == nullptr)
				continue;

			m_pShader->Render(pDeviceContext, (int)(pSubSet->vtIndices.size()), pMaterial, pMatrixBuffer, pLightBuffer, pCameraBuffer);
		}
	}
}

void CLightModel::Update()
{
	m_fTestRotation += DirectX::XM_PI * 0.05f;

	if (m_fTestRotation > 360.0f)
	{
		m_fTestRotation -= 360.f;
	}
}

HRESULT CLightModel::InitBuffers(ID3D11Device * pDevice, ModelData * pModelData)
{
	int i;

	if ( pModelData == nullptr || pModelData->vtMeshes.size() < 1 )
		return E_FAIL;
	
	for (i = 0; i < pModelData->vtMeshes.size(); ++i)
	{
		VertexGroup* pGroup = pModelData->vtMeshes[i];

		if ( pGroup == nullptr || 
			pGroup->vtIndexDatas.size() < 1 ||
			pGroup->vtIndexDatas.size() != pGroup->vtNormalIndexDatas.size() ||
			pGroup->vtIndexDatas.size() != pGroup->vtUVIndexDatas.size() )
			continue;

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
		ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));

		ModelSubsets* pSubSet = new ModelSubsets();

		//VertexType* parVeticies = new VertexType[pGroup->vtIndexDatas.size()];

		for (int j = 0; j < pGroup->vtIndexDatas.size(); ++j)
		{
			VertexType vertex;

			vertex.position = pGroup->vtPositions[pGroup->vtIndexDatas[j]];
			vertex.normal = pGroup->vtNormals[pGroup->vtNormalIndexDatas[j]];
			vertex.UV = pGroup->vtUVs[pGroup->vtUVIndexDatas[j]];

			pSubSet->vtVertices.push_back(vertex);

			//parVeticies[j] = vertex;
		}

		if (pSubSet->vtVertices.size() < 1)
		{
			SAFE_DELETE(pSubSet);
			return E_FAIL;
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * (UINT)(pSubSet->vtVertices.size());
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = &pSubSet->vtVertices[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pSubSet->pVertexBuffer)))
		{
			SAFE_DELETE(pSubSet);
			return E_FAIL;
		}

		//DWORD* parIndices = new DWORD[pGroup->vtIndexDatas.size()];

		for (int j = 0; j < pGroup->vtTotalIndicies.size(); ++j)
		{
			DWORD dwIndices = pGroup->vtTotalIndicies[j];

			pSubSet->vtIndices.push_back(dwIndices);
		}

		if (pSubSet->vtIndices.size() < 1)
		{
			SAFE_DELETE(pSubSet);
			return E_FAIL;
		}

		//Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * (UINT)(pSubSet->vtIndices.size());
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = &pSubSet->vtIndices[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &pSubSet->pIndexBuffer)))
		{
			SAFE_DELETE(pSubSet);
			return E_FAIL;
		}

		pSubSet->strMtlName = pGroup->strMtlName;

		if (pSubSet->strMtlName.empty())
		{
			pSubSet->strMtlName = _T("Basic");
		}

		//SAFE_DELETE_ARRAY(parVeticies);
		//SAFE_DELETE_ARRAY(parIndices);

		m_vtSubsets.push_back(pSubSet);
	}

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

			pMaterial->SetName(pInfo->strName);

			m_mapMaterial.insert(std::pair<std::basic_string<TCHAR>,CMaterial*>(pInfo->strName,pMaterial));
		}

		if (m_mapMaterial.size() > 0)
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

		pMaterial->SetName(_T("Basic"));

		m_mapMaterial.insert(std::pair<std::basic_string<TCHAR>, CMaterial*>(_T("Basic"), pMaterial));
	}

	return S_OK;
}

void CLightModel::RenderBuffers(ID3D11DeviceContext * pDeviceContext, ModelSubsets* pSubset)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetVertexBuffers(0, 1, &pSubset->pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	pDeviceContext->IASetIndexBuffer(pSubset->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
