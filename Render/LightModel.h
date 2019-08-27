#pragma once
#include "BaseModel.h"

class CLightModel :public CBaseModel 
{
private:
	float m_fTestRotation;
public:
	CLightModel();
	virtual ~CLightModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData) override;
	virtual void Render(ID3D11DeviceContext* pDeviceContext, MatrixBufferType* pMatrixBuffer = nullptr, LightBufferType* pLightBuffer = nullptr, CameraBufferType* pCameraBuffer = nullptr);

	virtual void Update() override;
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, ModelData* pModelData) override;
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData) override;

	virtual void RenderBuffers(ID3D11DeviceContext* pDeviceContext) override;
private:
	CLightModel(CLightModel&) {};
};