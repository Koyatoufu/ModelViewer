#pragma once
#include "RenderDefine.h"

class CMaterial;
struct ModelData;
class CShader;

class CBaseModel
{
protected:
	std::vector<ModelSubsets*> m_vtSubsets;

	std::vector<CMaterial*> m_vtMaterial;
	ModelData* m_pModelData;

	CShader* m_pShader;

	DirectX::XMFLOAT3 m_vecPosition;
public:
	CBaseModel();
	virtual ~CBaseModel();

	virtual HRESULT Initialize( ID3D11Device* pDevice, ModelData * pModelData ) = 0;

	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* pDeviceContext, MatrixBufferType* pMatrixBuffer = nullptr, LightBufferType* pLightBuffer = nullptr, CameraBufferType* pCameraBuffer = nullptr) = 0;

	CMaterial* GetMaterial(int nIndex = 0);

	ModelSubsets* GetSubSets(int nIndex = 0);

	DirectX::XMFLOAT3 GetPosition() { return m_vecPosition; }
	void SetPosition(float x, float y, float z);

	void setShader(CShader* pShader) { m_pShader = pShader; }

protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice , ModelData* pModelData) = 0;
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData) = 0;

	virtual void RenderBuffers(ID3D11DeviceContext* pDeviceContext) = 0;
};