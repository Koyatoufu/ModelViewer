#include "TestModel.h"

CTestModel::CTestModel()
{
}

CTestModel::~CTestModel()
{
}

HRESULT CTestModel::Initialize(ID3D11Device * pDevice, TCHAR * szFileName)
{
	SVertexType* parVerticies = NULL;
	unsigned long uIndices = 0;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;



	return S_OK;
}

void CTestModel::Update()
{
}

void CTestModel::Render(ID3D11DeviceContext * pDeviceContext)
{
	
}
