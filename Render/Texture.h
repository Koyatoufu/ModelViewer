#pragma once

#include "RenderDefine.h"

class CTexture
{
private:
	ID3D11ShaderResourceView* m_pTextureView;
public:
	CTexture();
	~CTexture();

	HRESULT Initiazlie(ID3D11Device* pDevice, std::wstring wStrFileName);

	ID3D11ShaderResourceView* GetTextureView();
};