#pragma once
#include "BaseModel.h"

class CTextureModel :public CBaseModel
{
private:
	struct  ColorVertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	CTextureModel();
	virtual ~CTextureModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData);
	virtual void Render(ID3D11DeviceContext* pDeviceContext);

	virtual void Update();

private:
	CTextureModel(const CTextureModel&) {};
	void RenderBuffers(ID3D11DeviceContext*);
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, void* pModelData);
	virtual HRESULT InitMaterial(ID3D11Device * pDevice);
};