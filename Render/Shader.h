#pragma once

#include "RenderDefine.h"

class CMaterial;

class CShader
{
protected:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightBuffer;

	ID3D11SamplerState* m_pSampleState;
public:
	CShader();
	~CShader();

	virtual HRESULT Initialize(ID3D11Device* pDevice ) = 0;

	void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, const WCHAR* wszFileName);

	HRESULT Render(ID3D11DeviceContext* pDeviceContext, int nIndexCount, MatrixBufferType& matrixBuffer, CMaterial* pMaterial = nullptr);
protected:
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* pDeviceContext, MatrixBufferType& matrixBuffer, CMaterial* pMaterial = nullptr) = 0;
private:
	void RenderShader(ID3D11DeviceContext* pDeviceContext, int nIndexCount);
};