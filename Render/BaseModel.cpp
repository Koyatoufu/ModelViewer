#include "stdafx.h"
#include "BaseModel.h"

CBaseModel::CBaseModel():
	m_pVertexBuffer(NULL),
	m_pIndexBuffer(NULL),
	m_nVertexCount(0),
	m_nIndexCount(0)
{
}

CBaseModel::~CBaseModel()
{
	SAFE_RELEASE_D3DCONTENTS(m_pVertexBuffer);
	SAFE_RELEASE_D3DCONTENTS(m_pIndexBuffer);
}

CMaterial * CBaseModel::GetMaterial(int nIndex)
{
	if( m_vecMaterial.size() < 1 )
		return nullptr;

	if (nIndex >= m_vecMaterial.size())
		return m_vecMaterial[m_vecMaterial.size() - 1];

	return m_vecMaterial[nIndex];
}

std::string CBaseModel::GetName() const
{
	return m_strName;
}

std::string CBaseModel::GetFilePath() const
{
	return m_strFilePath;
}
