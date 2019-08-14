#pragma once
#include "RenderDefine.h"

class CMaterial;
class CShader;

class CBaseModel
{
protected:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_nVertexCount;
	int m_nIndexCount;

	SVertexType* m_ArrVertex;
	std::vector<CMaterial*> m_vecMaterial;

	std::string m_strFilePath;
	std::string m_strName;

	struct SModelData* m_pModelData;

	CShader* m_pShader;

	DirectX::XMFLOAT3 m_vecPosition;
public:
	CBaseModel();
	virtual ~CBaseModel();

	virtual HRESULT Initialize( ID3D11Device* pDevice, void * pModelData ) = 0;

	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) = 0;

	CMaterial* GetMaterial(int nIndex = 0);
	std::string GetName() const;
	std::string GetFilePath() const;

	CShader* GetShader() const { return m_pShader; }
	void SetShader(CShader* pShader) { m_pShader = pShader; }

	DirectX::XMFLOAT3 GetPosition() { return m_vecPosition; }
	void SetPosition(float x, float y, float z);

protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice , void* pModelData);
	virtual HRESULT InitMaterial(ID3D11Device * pDevice) { return S_OK; };
};