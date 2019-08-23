#pragma once
#include "BaseModel.h"

class CLightModel :public CBaseModel 
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT3 normal;
	};
public:
	CLightModel();
	virtual ~CLightModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData) override;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) override;

	virtual void Update() override;
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, ModelData* pModelData) override;
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData) override;
private:
	CLightModel(CLightModel&) {};
	void RenderBuffers(ID3D11DeviceContext* pDeviceContext);
};