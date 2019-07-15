#pragma once

#include "RenderDefine.h"

class CRenderer
{
private:
	HWND m_hWnd;

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_pTexDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterState;

	DirectX::XMMATRIX m_matWorld;
	DirectX::XMMATRIX m_matOrtho;
	DirectX::XMMATRIX m_matProjection;

	bool m_bVsyncEnabled;
	int m_nVideoCardMemory;
	char m_szVideoCardDescription[128];
private:
	CRenderer(CRenderer&) {};
	void BeginScene();
	void EndScene();
public:
	CRenderer();
	~CRenderer();

	HRESULT Initialize(HWND hWnd);
	void Render();

	ID3D11Device* GetDevice() { return m_pDevice; };
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; };
};