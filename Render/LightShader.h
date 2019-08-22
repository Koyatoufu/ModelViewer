#pragma once
#include "Shader.h"

class CLightShader :public CShader
{
public:
	CLightShader();
	virtual ~CLightShader();

	virtual HRESULT Initialize(ID3D11Device* pDevice) override;
protected:
	HRESULT SetShaderParameters(ID3D11DeviceContext* pDeviceContext, CMaterial* pMaterial = nullptr,
		MatrixBufferType* pMatrixBuffer = nullptr, LightBufferType* pLightBuffer = nullptr, CameraBufferType* pCameraBuffer = nullptr) override;
private:
	CLightShader(CLightShader&) {}
};