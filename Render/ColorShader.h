#pragma once
#include "Shader.h"

class CColorShader:public CShader
{
public:
	virtual HRESULT Initialize(ID3D11Device* pDevice);
protected:
	HRESULT SetShaderParameters(ID3D11DeviceContext* pDeviceContext, MatrixBufferType& matrixBuffer, CMaterial* pMaterial = nullptr);
public:
	CColorShader();
	virtual ~CColorShader();
private:
	CColorShader(const CColorShader&) {};
};