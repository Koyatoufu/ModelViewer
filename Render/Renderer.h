#pragma once

#include "RenderDefine.h"
#include "SingleTon.h"

class CCamera;
class CBaseModel;

class CRenderer:public CSingleTonT<CRenderer>
{
	friend CSingleTonT;
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

	CCamera* m_pCamera;
	CBaseModel* m_pTestModel;

private:
	CRenderer(CRenderer&) {};
	void BeginScene();
	void EndScene();
protected:
	CRenderer();
	~CRenderer();
public:
	HRESULT Initialize(HWND hWnd);

	void Update();
	void Render();

	ID3D11Device* GetDevice() { return m_pDevice; };
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; };

	CCamera* GetRendererCamera() { return m_pCamera; }

	DirectX::XMMATRIX GetWorld() { return m_matWorld; }
	DirectX::XMMATRIX GetOrtho() { return m_matOrtho; }
	DirectX::XMMATRIX GetProjection() { return m_matProjection; }

};