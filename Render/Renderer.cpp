#include "Renderer.h"
#include <stdlib.h>
#include "ModelCollector.h"

CRenderer::CRenderer():
	m_hWnd(NULL),
	m_pSwapChain(NULL),
	m_pDevice(NULL),
	m_pDeviceContext(NULL),
	m_pRenderTargetView(NULL),
	m_pTexDepthStencilBuffer(NULL),
	m_pDepthStencilState(NULL),
	m_pDepthStencilView(NULL),
	m_pRasterState(NULL),
	m_bVsyncEnabled(false),
	m_nVideoCardMemory(0)
{
	if ( !ms_pInstance )
	{
		ms_pInstance = this;
	}
	else
	{
		this->~CRenderer();
	}
}

CRenderer::~CRenderer()
{
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(FALSE, nullptr);

	SAFE_RELEASE_D3DCONTENTS(m_pRasterState);
	SAFE_RELEASE_D3DCONTENTS(m_pDepthStencilView);
	SAFE_RELEASE_D3DCONTENTS(m_pDepthStencilState);
	SAFE_RELEASE_D3DCONTENTS(m_pTexDepthStencilBuffer);
	SAFE_RELEASE_D3DCONTENTS(m_pRenderTargetView);
	SAFE_RELEASE_D3DCONTENTS(m_pDeviceContext);
	SAFE_RELEASE_D3DCONTENTS(m_pDevice);
	SAFE_RELEASE_D3DCONTENTS(m_pSwapChain);
}

HRESULT CRenderer::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	IDXGIFactory* pFactory = NULL;
	IDXGIAdapter* pAdapter = NULL;
	IDXGIOutput* pAdapterOutput = NULL;
	unsigned int nNumModes, i, nNumerator, nDenominator;
	size_t nStringLength = 0;
	DXGI_MODE_DESC* pArrDisplayModeList = NULL;
	DXGI_ADAPTER_DESC adapterDesc;
	int nError;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;

	//세부 설정들은 차후 INI로
	int nScreenWidth = 0, nScreenHeight = 0;
	float fFieldOfView = 0.0f, fScreenAspect = 0.0f;
	float fScreenNear = 0.0f, fScreenDepth = 0.0f;
	bool bVsyncEnable = true;

	if (true)
	{
		nScreenWidth = 1600, nScreenHeight = 900;
		fScreenNear = 0.1f, fScreenDepth = 1000.f;
		m_bVsyncEnabled = bVsyncEnable;
	}

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
		return E_FAIL;

	if (FAILED(pFactory->EnumAdapters(0, &pAdapter)))
		return E_FAIL;

	if (FAILED(pAdapter->EnumOutputs(0, &pAdapterOutput)))
		return E_FAIL;

	if (FAILED(pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nNumModes, NULL)))
		return E_FAIL;

	if (nNumModes <= 0)
		return E_FAIL;

	pArrDisplayModeList = new DXGI_MODE_DESC[nNumModes];

	if (!pArrDisplayModeList)
		return E_FAIL;

	if (pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nNumModes, pArrDisplayModeList))
		return E_FAIL;

	for (i = 0; i < nNumModes; i++)
	{
		if (pArrDisplayModeList[i].Width == nScreenWidth && pArrDisplayModeList[i].Height == nScreenHeight)
		{
			nNumerator = pArrDisplayModeList[i].RefreshRate.Numerator;
			nDenominator = pArrDisplayModeList[i].RefreshRate.Denominator;
		}
	}

	if (FAILED(pAdapter->GetDesc(&adapterDesc)))
		return E_FAIL;

	m_nVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	nError = wcstombs_s(&nStringLength, m_szVideoCardDescription, 128, adapterDesc.Description, 128);

	if (nError != 0)
		return E_FAIL;

	SAFE_DELETE_ARRAY(pArrDisplayModeList);

	SAFE_RELEASE_D3DCONTENTS(pAdapterOutput);
	SAFE_RELEASE_D3DCONTENTS(pAdapter);
	SAFE_RELEASE_D3DCONTENTS(pFactory);

	//Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = nScreenWidth;
	swapChainDesc.BufferDesc.Height = nScreenHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	if (m_bVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = nNumerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = nDenominator;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = m_hWnd;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;
	//end

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext)))
		return E_FAIL;

	if (!m_pSwapChain || !m_pDevice || !m_pDeviceContext)
		return E_FAIL;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView)))
		return E_FAIL;

	SAFE_RELEASE_D3DCONTENTS(pBackBuffer);

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthBufferDesc.Width = nScreenWidth;
	depthBufferDesc.Height = nScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//end

	if (FAILED(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pTexDepthStencilBuffer)))
		return E_FAIL;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//end

	if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
		return E_FAIL;

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	// Initailze the depth stencil view.

	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//end

	if (FAILED(m_pDevice->CreateDepthStencilView(m_pTexDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView)))
		return E_FAIL;

	// Setup the raster description which will determine how and what polygons will be drawn.
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	//end

	if (FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState)))
		return E_FAIL;

	// Setup the viewport for rendering.

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.Width = (float)nScreenWidth;
	viewport.Height = (float)nScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	//end

	m_pDeviceContext->RSSetViewports(1, &viewport);

	fFieldOfView = (float)DirectX::XM_PI * 0.25f;// 1/4
	fScreenAspect = (float)nScreenWidth / (float)nScreenHeight;

	m_matProjection = DirectX::XMMatrixPerspectiveFovLH(fFieldOfView, fScreenAspect, fScreenNear, fScreenDepth);

	m_matWorld = DirectX::XMMatrixIdentity();

	m_matOrtho = DirectX::XMMatrixOrthographicLH((float)nScreenWidth, (float)nScreenHeight, fScreenNear, fScreenDepth);

	return S_OK;
}

void CRenderer::Render()
{
	BeginScene();

	
	EndScene();
}

void CRenderer::BeginScene()
{
	float fArrColor[4] = { 0.0f,0.0f,1.0f,1.0f };

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, fArrColor);

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderer::EndScene()
{
	if (m_bVsyncEnabled)
	{
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		m_pSwapChain->Present(0, 0);
	}
}