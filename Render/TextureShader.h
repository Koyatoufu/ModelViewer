#pragma once
#include "Shader.h"

class CTextureShader :public CShader
{
public:
	CTextureShader();
	virtual ~CTextureShader();

	virtual HRESULT Initialize(ID3D11Device* pDevice) override;
protected:
	HRESULT SetShaderParameters(ID3D11DeviceContext* pDeviceContext, CMaterial* pMaterial = nullptr, 
		MatrixBufferType* pMatrixBuffer = nullptr, LightBufferType* pLightBuffer = nullptr) override;
private:
	CTextureShader(CTextureShader&) {}
};