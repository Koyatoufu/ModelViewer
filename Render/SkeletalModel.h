#pragma once
#include "BaseModel.h"

class CSkeletalModel :public CBaseModel
{
protected:

public:
	CSkeletalModel();
	virtual ~CSkeletalModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, void * pModelData);

	virtual void Update();
	virtual void Render(ID3D11DeviceContext* pDeviceContext);

protected:
};