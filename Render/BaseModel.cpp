#include "BaseModel.h"
#include "Shader.h"
#include "Material.h"

CBaseModel::CBaseModel():
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_nVertexCount(0),
	m_nIndexCount(0),
	m_pInstanceBuffer(nullptr),
	m_nInstanceCount(0),
	m_bInstnceUse(false)
{
}

CBaseModel::~CBaseModel()
{
	SAFE_RELEASE_D3DCONTENTS(m_pVertexBuffer);
	SAFE_RELEASE_D3DCONTENTS(m_pIndexBuffer);
	SAFE_RELEASE_D3DCONTENTS(m_pInstanceBuffer);

	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
		SAFE_DELETE(m_vecMaterial[i]);
}

CMaterial * CBaseModel::GetMaterial(int nIndex)
{
	if( m_vecMaterial.size() < 1 )
		return nullptr;

	if (nIndex >= m_vecMaterial.size())
		return m_vecMaterial[m_vecMaterial.size() - 1];

	return m_vecMaterial[nIndex];
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
