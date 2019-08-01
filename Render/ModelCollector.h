#pragma once
#include "BaseModel.h"
#include "SingleTon.h"

class CModelCollector : public CSingleTonT<CModelCollector>
{
	friend CSingleTonT;
private:
	std::list<CBaseModel*> m_lstModels;
protected:
	CModelCollector();
	virtual ~CModelCollector();
public:
	void Update();
	void Render(ID3D11DeviceContext* pDeviceContext);
private:
	void ReleaseAllModel();
};
