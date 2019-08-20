#pragma once
#include "BaseModel.h"

class CStaticModel :public CBaseModel
{
protected:
	CStaticModel();
	virtual ~CStaticModel();
public:

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData);

	virtual void Update();
	virtual void Render(ID3D11DeviceContext* pDeviceContext);
};