#pragma once

#include "RenderDefine.h"

class CTexture
{
private:
	ID3D11ShaderResourceView* m_pTextureView;
public:
	CTexture();
	~CTexture();

	HRESULT Initiazlie(ID3D11Device* pDevice, std::basic_string<TCHAR> strFileName);

	ID3D11ShaderResourceView* GetTextureView() { return m_pTextureView; }

private:
	DirectX::ScratchImage LoadTextureFromFile(std::basic_string<TCHAR> strFileName);
};