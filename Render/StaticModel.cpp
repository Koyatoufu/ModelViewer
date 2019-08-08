#include "StaticModel.h"

CStaticModel::CStaticModel()
{
}

CStaticModel::~CStaticModel()
{
}

HRESULT CStaticModel::Initialize(ID3D11Device * pDevice, void* pData)
{
	return S_OK;
}

void CStaticModel::Update()
{

}

void CStaticModel::Render(ID3D11DeviceContext * pDeviceContext)
{
	if (pDeviceContext == nullptr)
		return;
}
