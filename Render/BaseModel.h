#pragma once
#include "RenderDefine.h"
#include "Material.h"

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
public:
	CBaseModel();
	virtual ~CBaseModel();

	virtual HRESULT Initialize( ID3D11Device* pDevice, void * pModelData ) = 0;

	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* pDeviceContext) = 0;

	CMaterial* GetMaterial(int nIndex = 0);
	std::string GetName() const;
	std::string GetFilePath() const;
protected:
	virtual HRESULT InitBuffers(ID3D11Device* pDevice , void* pModelData);
	virtual HRESULT InitMaterial(ID3D11Device * pDevice) { return S_OK; };
};