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
protected:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_nVertexCount;
	int m_nIndexCount;
public:
	CColorModel();
	virtual ~CColorModel();

	virtual HRESULT Initialize(ID3D11Device* pDevice, ModelData * pModelData);
	virtual void Render(ID3D11DeviceContext* pDeviceContext, MatrixBufferType* pMatrixBuffer = nullptr, LightBufferType* pLightBuffer = nullptr, CameraBufferType* pCameraBuffer = nullptr) override;

	virtual void Update();

private:
	CColorModel(const CColorModel&) {};
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice, ModelData* pModelData);
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData);

	virtual void RenderBuffers(ID3D11DeviceContext* pDeviceContext, ModelSubsets* pSubset = nullptr) override;
};