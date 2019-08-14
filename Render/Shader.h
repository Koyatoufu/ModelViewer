#pragma once

#include "RenderDefine.h"

class CMaterial;

class CShader
{
private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;
	ID3D11Buffer* m_pLightBuffer;

	ID3D11SamplerState* m_pSampleState;

	//ID3D10Blob* m_pVertexShaderBuffer;
	//ID3D10Blob* m_pPixelShaderBuffer;

	EShaderType m_eShaderType;
public:
	CShader();
	~CShader();

	HRESULT Initialize(std::basic_string<TCHAR> strName, EShaderType eShaderType = ESHADER_NORMAL );

	HRESULT SetShaderParameter(DirectX::XMMATRIX matWorld, DirectX::XMMATRIX matView, DirectX::XMMATRIX matProjection,
		CMaterial* pMaterial = nullptr,
		DirectX::XMFLOAT3 vecLightDirection = DirectX::XMFLOAT3(0.0f, -1.0f, 1.0f),
		DirectX::XMFLOAT4 diffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) );
	void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, const WCHAR* wszFileName);
	EShaderType GetShaderType() { return m_eShaderType; }

	void Render(ID3D11DeviceContext* pDeviceContext, int nIndexCount);
};