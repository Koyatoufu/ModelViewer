#pragma once
#include "BaseModel.h"

class CModelCollector
{
private:
	std::list<CBaseModel*> m_lstModels;
protected:
	CModelCollector();
	virtual ~CModelCollector();
public:
	void Render();
private:
	void ReleaseAllModel();
};
