#pragma once

#include "RenderDefine.h"
#include "SingleTon.h"

class CBaseModel;

class CModelFactory : public CSingleTonT<CModelFactory>
{
	friend CSingleTonT;

protected:
	CModelFactory();
	virtual ~CModelFactory();
public:
	HRESULT Initialize();
	CBaseModel* LoadModel(std::string& strFileName);
};