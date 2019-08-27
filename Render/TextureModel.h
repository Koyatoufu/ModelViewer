#pragma once
#include "ColorModel.h"

class CTextureModel :public CColorModel
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
private:
	ID3D11Buffer* m_pInstanceBuffer;
	int m_nInstanceCount;
	bool m_bInstnceUse;
public:
	CTextureModel();
	virtual ~CTextureModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData) override;
	virtual void Render(ID3D11DeviceContext* pDeviceContext, MatrixBufferType* pMatrixBuffer = nullptr, LightBufferType* pLightBuffer = nullptr, CameraBufferType* pCameraBuffer = nullptr) override;
	virtual void Update() override;

	bool IsInstceUse() { return m_bInstnceUse; }
private:
	CTextureModel(const CTextureModel&) {};
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, ModelData* pModelData) override;
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData) override;

	virtual void RenderBuffers(ID3D11DeviceContext* pDeviceContext) override;
};