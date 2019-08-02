#include "Factory.h"
#include "ModelCollector.h"

#include "StaticModel.h"
#include "SkeletalModel.h"

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
	if (CModelCollector::Get() == nullptr)
		return nullptr;

	return nullptr;
}
