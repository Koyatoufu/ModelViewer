#pragma once
#include "RenderDefine.h"

class CMaterial;
struct ModelData;
//class CShader;

class CBaseModel
{
protected:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_nVertexCount;
	int m_nIndexCount;

	ID3D11Buffer* m_pInstanceBuffer;
	int m_nInstanceCount;

	std::vector<CMaterial*> m_vecMaterial;
	ModelData* m_pModelData;

	DirectX::XMFLOAT3 m_vecPosition;

	bool m_bInstnceUse;
public:
	CBaseModel();
	virtual ~CBaseModel();

	virtual HRESULT Initialize( ID3D11Device* pDevice, ModelData * pModelData ) = 0;

	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) = 0;

	CMaterial* GetMaterial(int nIndex = 0);

	int	GetVertexCount() const { return m_nVertexCount; };
	int	GetIndexCount() const { return m_nIndexCount; };
	int GetInstanceCount() { return m_nInstanceCount; }

	DirectX::XMFLOAT3 GetPosition() { return m_vecPosition; }
	void SetPosition(float x, float y, float z);

	bool IsInstceUse() { return m_bInstnceUse; }

protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice , ModelData* pModelData) = 0;
	virtual HRESULT InitMaterial(ID3D11Device * pDevice, ModelData* pModelData) = 0;
};