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
	pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\two.obj"));
	//pModelData = CImportUtil::GetInstance()->LoadModelData(_T(".\\res\\boy.md5mesh"));

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
	for (size_t i = 0; i < m_vtSubsets.size(); ++i)
	{
		//ModelSubsets* pSubSet = m_vtSubsets[0];
		ModelSubsets* pSubSet = m_vtSubsets[i];

		if (pSubSet == nullptr)
			continue;

		MatrixBufferType matrixBuffer = *pMatrixBuffer;

		//DirectX::XMFLOAT3 vecposition = m_vecPosition;

		//DirectX::XMVECTOR vecTrnas = DirectX::XMLoadFloat3(&vecposition);
		DirectX::XMVECTOR vecTrnas = DirectX::XMLoadFloat3(&m_vecPosition);
		DirectX::XMMATRIX matTrans = DirectX::XMMatrixTranslationFromVector(vecTrnas);

		DirectX::XMMATRIX matRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_fTestRotation));
		matrixBuffer.world = pMatrixBuffer->world * matTrans * matRot;
		//matrixBuffer.world = pMatrixBuffer->world * matTrans;

		RenderBuffers(pDeviceContext, pSubSet);

		if (m_pShader)
		{
			CMaterial* pMaterial = GetMaterial(pSubSet->strMtlName);

			if (pMaterial == nullptr)
				continue;

			m_pShader->Render(pDeviceContext, (int)(pSubSet->vtIndices.size()), pMaterial, &matrixBuffer, pLightBuffer, pCameraBuffer);
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

	//static bool bSwitch = false;

	//if (bSwitch)
	//{
	//	m_vecPosition.y += 0.005f;

	//	if (m_vecPosition.y >= 10.0f)
	//		bSwitch = false;
	//}
	//else
	//{
	//	m_vecPosition.y -= 0.005f;

	//	if (m_vecPosition.y < 0.0f)
	//		bSwitch = true;
	//}
}

HRESULT CLightModel::InitBuffers(ID3D11Device * pDevice, ModelData * pModelData)
{
	int i;

	if ( pModelData == nullptr || pModelData->vtMeshSubsets.size() < 1 )
		return E_FAIL;
	
	for (i = 0; i < pModelData->vtMeshSubsets.size(); ++i)
	{
		MeshSubsets* pGroup = pModelData->vtMeshSubsets[i];

		//if ( pGroup == nullptr || 
		//	pGroup->vtIndicies.size() < 1 ||
		//	pGroup->vtIndexDatas.size() != pGroup->vtNormalIndexDatas.size() ||
		//	pGroup->vtIndexDatas.size() != pGroup->vtUVIndexDatas.size() )
		//	continue;

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
		ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));

		ModelSubsets* pSubSet = new ModelSubsets();

		//VertexType* parVeticies = new VertexType[pGroup->vtIndexDatas.size()];

		for (int j = 0; j < pGroup->vtVertices.size(); ++j)
		{
			VertexType tempVertex;

			tempVertex.position = pGroup->vtVertices[j].position;
			tempVertex.normal = pGroup->vtVertices[j].normal;
			tempVertex.UV = pGroup->vtVertices[j].UV;

			//tempVertex.nStartWeight = pGroup->vtVertices[j].nStartWeight;
			//tempVertex.nWeightCount = pGroup->vtVertices[j].nWeightCount;

			tempVertex.tangent = pGroup->vtVertices[j].tangent;
			tempVertex.biTangent = pGroup->vtVertices[j].biTangent;

			pSubSet->vtVertices.push_back(tempVertex);
		}

		if (pSubSet->vtVertices.size() < 1)
		{
			SAFE_DELETE(pSubSet);
			return E_FAIL;
		}

		for (int j = 0; j < pGroup->vtIndicies.size(); ++j)
		{
			DWORD dwIndices = pGroup->vtIndicies[j];

			pSubSet->vtIndices.push_back(dwIndices);
		}

		if (pSubSet->vtIndices.size() < 1)
		{
			SAFE_DELETE(pSubSet);
			return E_FAIL;
		}

		for (int j = 0; j < pGroup->vtWeights.size(); ++j)
		{
			Weight groupWeight = pGroup->vtWeights[j];
			WeightType tempWeight;

			tempWeight.fBias = groupWeight.fBias;
			tempWeight.nJointID = groupWeight.nJointID;
			tempWeight.position = groupWeight.position;

			pSubSet->vtWeights.push_back(tempWeight);
		}

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * (UINT)(pSubSet->vtVertices.size());
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		//vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = &pSubSet->vtVertices[0];

		if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pSubSet->pVertexBuffer)))
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

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = &pSubSet->vtIndices[0];

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
			MaterialData::MaterialInfo info = pModelData->pMaterialData->vtMaterialInfo[i];

			CMaterial* pMaterial = new CMaterial;

			DirectX::XMFLOAT4 ambient = info.ambient;
			DirectX::XMFLOAT4 diffuse = info.diffuse;
			DirectX::XMFLOAT4 specular = info.specular;

			pMaterial->Initialize(diffuse, ambient, specular);

			if (info.strDiffuseMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, info.strDiffuseMap);
			}
			if (info.strNormalMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, info.strNormalMap, ETEXTURE_NORMAL);
			}
			if (info.strSpecularMap.empty() == false)
			{
				pMaterial->AddTexture(pDevice, info.strSpecularMap, ETEXTURE_SPECULAR);
			}

			pMaterial->SetName(info.strName);

			m_mapMaterial.insert(std::pair<std::basic_string<TCHAR>,CMaterial*>(info.strName,pMaterial));
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
