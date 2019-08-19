#include "Renderer.h"
#include <stdlib.h>
#include "D3Dclass.h"
#include "ColorModel.h"
#include "ColorShader.h"
#include "Camera.h"

CRenderer::CRenderer():
	m_hWnd(NULL),
	m_pD3D(NULL),
	m_bVsyncEnabled(false),
	m_nVideoCardMemory(0),
	m_pCamera(NULL),
	m_pTestModel(NULL)
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
	SAFE_DELETE(m_pD3D);
	SAFE_DELETE(m_pTestShader);
	SAFE_DELETE(m_pTestModel);
	SAFE_DELETE(m_pCamera);
}

HRESULT CRenderer::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	m_pD3D = new CD3DClass();
	if (!m_pD3D->Initialize(1600, 900, true, m_hWnd, false, 1000.f, 0.1f))
	{
		return E_FAIL;
	}

	ID3D11Device* pDevice = m_pD3D->GetDevice();

	m_pCamera = new CCamera();
	m_pCamera->SetPosition(0.f, 0.f, -10.f);

	m_pTestModel = new CColorModel();
	if (FAILED(m_pTestModel->Initialize(pDevice,nullptr)))
	{
		return E_FAIL;
	}

	m_pTestShader = new CColorShader();
	if (FAILED(m_pTestShader->Initialize(pDevice)))
	{
		return E_FAIL;
	}

	//m_pTestModel->SetShader(pShader);

	return S_OK;
}

void CRenderer::Update()
{

}

void CRenderer::Render()
{
	m_pD3D->BeginScene(0.0f,0.0f,1.0f,1.0f);

	m_pCamera->Render();
	
	MatrixBufferType matrixBuffer;
	ID3D11DeviceContext* pDeviceContext = m_pD3D->GetDeviceContext();

	m_pD3D->GetWorldMatrix(matrixBuffer.world);
	m_pCamera->GetViewMatrix(matrixBuffer.view);
	m_pD3D->GetProjectionMatrix(matrixBuffer.projection);
	
	m_pTestModel->Render(pDeviceContext);

	m_pTestShader->Render(pDeviceContext,m_pTestModel->GetIndexCount(), matrixBuffer);
	
	m_pD3D->EndScene();
}