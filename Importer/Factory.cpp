#include "Factory.h"

CModelFactory::CModelFactory()
{
}

CModelFactory::~CModelFactory()
{
}

HRESULT CModelFactory::Initialize()
{
	return S_OK;
}

CBaseModel * CModelFactory::LoadModel(std::string & strFileName)
{
	return nullptr;
}
