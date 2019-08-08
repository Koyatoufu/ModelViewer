#pragma once
#include "BaseModel.h"

class CTestModel :public CBaseModel
{
protected:
public:
	CTestModel();
	virtual ~CTestModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, void * pModelData);

	virtual void Update();
	virtual void Render(ID3D11DeviceContext* pDeviceContext);
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice , void* pModelData);
	virtual HRESULT InitMaterial(ID3D11Device * pDevice);
};
