#pragma once
#include "BaseModel.h"

class CColorModel:public CBaseModel
{
private:
	struct  ColorVertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	CColorModel();
	virtual ~CColorModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData);
	virtual void Render(ID3D11DeviceContext* pDeviceContext);

	virtual void Update();

private:
	CColorModel(const CColorModel&) {};
	void RenderBuffers(ID3D11DeviceContext* pDeviceContext);
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, ModelData* pModelData);
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData);
};