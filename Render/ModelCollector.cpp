#include "ModelCollector.h"

CModelCollector::CModelCollector()
{
}

CModelCollector::~CModelCollector()
{
	ReleaseAllModel();
}

void CModelCollector::Update()
{
	std::list<CBaseModel*>::iterator iter;

	for (iter = m_lstModels.begin(); iter != m_lstModels.end(); ++iter)
	{
		CBaseModel* pModel = *iter;

		if (pModel)
		{
			
		}
	}
}

void CModelCollector::Render(ID3D11DeviceContext* pDeviceContext)
{
	std::list<CBaseModel*>::iterator iter;

	for (iter = m_lstModels.begin(); iter != m_lstModels.end(); ++iter)
	{
		CBaseModel* pModel = *iter;

		if (pModel)
		{
			pModel->Render(pDeviceContext);
		}
	}
}

void CModelCollector::ReleaseAllModel()
{

}