#include "LightShader.h"

CLightShader::CLightShader()
{
}

CLightShader::~CLightShader()
{
}

HRESULT CLightShader::Initialize(ID3D11Device * pDevice)
{
	return S_OK;
}

HRESULT CLightShader::SetShaderParameters(ID3D11DeviceContext * pDeviceContext, CMaterial * pMaterial, MatrixBufferType * pMatrixBuffer, LightBufferType * pLightBuffer)
{
	return S_OK;
}
