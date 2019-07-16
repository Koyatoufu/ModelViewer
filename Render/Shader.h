#pragma once

#include "RenderDefine.h"

class CShader
{
private:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pMatrixBuffer;

	ID3D11SamplerState* m_pSampleState;

	ID3D10Blob* m_pVertexShaderBuffer;
	ID3D10Blob* m_pPixelShaderBuffer;

	EShaderType m_eShaderType;
public:
	CShader();
	~CShader();

	HRESULT Initialize(std::wstring wstrFileName, EShaderType eShaderType = ESHADER_NORMAL );

	HRESULT SetShaderParameter(DirectX::FXMMATRIX matWorld, DirectX::FXMMATRIX matView, DirectX::FXMMATRIX matProj, ID3D11ShaderResourceView* pTextureView);
	void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage, WCHAR* wszFileName);
	EShaderType GetShaderType() { return m_eShaderType; }
};