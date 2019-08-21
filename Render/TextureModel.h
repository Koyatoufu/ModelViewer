#pragma once
#include "BaseModel.h"

class CTextureModel :public CBaseModel
{
private:
	struct  TextureVertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT4 color;
	};

	struct InstanceType
	{
		DirectX::XMFLOAT3 position;
	};
public:
	CTextureModel();
	virtual ~CTextureModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData) override;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) override;

	virtual void Update() override;
private:
	CTextureModel(const CTextureModel&) {};
	void RenderBuffers(ID3D11DeviceContext* pDeviceContext);
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, ModelData* pModelData) override;
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData) override;
};