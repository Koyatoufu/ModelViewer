#include "BaseModel.h"
#include "Shader.h"
#include "Material.h"
#include "ImportDefine.h"

CBaseModel::CBaseModel():
	m_pShader(nullptr),
	m_pModelData(nullptr)
{
}

CBaseModel::~CBaseModel()
{
	SAFE_DELETE(m_pModelData);
	SAFE_DELETE(m_pShader);

	for (size_t i = 0; i < m_vtSubsets.size(); ++i)
		SAFE_DELETE(m_vtSubsets[i]);

	for (std::map<std::basic_string<TCHAR>, CMaterial*>::iterator iter = m_mapMaterial.begin(); iter != m_mapMaterial.end(); ++iter)
	{
		SAFE_DELETE((*iter).second);
	}

	m_mapMaterial.clear();
}

CMaterial * CBaseModel::GetMaterial(std::basic_string<TCHAR> strName)
{
	if( m_mapMaterial.size() < 1 )
		return nullptr;

	std::map<std::basic_string<TCHAR>, CMaterial*>::iterator iter = m_mapMaterial.begin();

	if (strName.empty() || iter == m_mapMaterial.end())
		return nullptr;

	std::map<std::basic_string<TCHAR>, CMaterial*>::iterator iterFind = m_mapMaterial.find(strName);

	if (iterFind == m_mapMaterial.end())
		return iter->second;

	return iterFind->second;
}

ModelSubsets * CBaseModel::GetSubSets(int nIndex)
{
	if (m_vtSubsets.size() < 1)
		return nullptr;

	if (nIndex >= m_vtSubsets.size() || nIndex < 0)
		return nullptr;

	return m_vtSubsets[nIndex];
}

void CBaseModel::SetPosition(float x, float y, float z)
{
	m_vecPosition.x = x;
	m_vecPosition.y = y;
	m_vecPosition.z = z;
}

//HRESULT CBaseModel::InitBuffers(ID3D11Device * pDevice, void* pData)
//{
//	if (pDevice == nullptr || pData == nullptr)
//		return E_FAIL;
//
//	//SVertexType* parVerticies = NULL;
//	//unsigned long* parIndices = 0;
//	//D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
//	//D3D11_SUBRESOURCE_DATA vertexData, indexData;
//
//	//m_nVertexCount = nVertexCount;
//	//m_nIndexCount = nIndexCount;
//
//	//parVerticies = new SVertexType[m_nVertexCount];
//	//parIndices = new unsigned long[m_nIndexCount];
//
//	return S_OK;
//}
