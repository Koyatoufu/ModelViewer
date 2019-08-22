#include "Renderer.h"
#include <stdlib.h>
#include "D3Dclass.h"
#include "TextureModel.h"
#include "TextureShader.h"
#include "ColorModel.h"
#include "ColorShader.h"
#include "LightModel.h"
#include "LightShader.h"
#include "Camera.h"
#include "Light.h"
#include "ImportUtil.h"

CRenderer::CRenderer():
	m_hWnd(NULL),
	m_pD3D(NULL),
	m_bVsyncEnabled(false),
	m_nVideoCardMemory(0),
	m_pCamera(NULL),
	m_pTestModel(NULL)
{
	CImportUtil::CreateInstance();
}

CRenderer::~CRenderer()
{
	CImportUtil::ReleaseInstnace();

	SAFE_DELETE(m_pTestShader);
	SAFE_DELETE(m_pTestModel);
	SAFE_DELETE(m_pLight);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pD3D);
}

HRESULT CRenderer::Initialize(HWND hWnd, int nWidth, int nHeight, bool bFullScreen, bool bSync)
{
	m_hWnd = hWnd;

	m_pD3D = new CD3DClass();
	if (!m_pD3D->Initialize(nWidth, nHeight, bSync, m_hWnd, bFullScreen, 1000.f, 0.1f))
		return E_FAIL;

	ID3D11Device* pDevice = m_pD3D->GetDevice();

	m_pCamera = new CCamera();
	m_pCamera->SetPosition(0.f, 0.f, -10.f);

	m_pLight = new CLight();
	m_pLight->SetDirection(0.0f, -1.0f, 1.0f);
	m_pLight->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_pLight->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLight->SetSpecularColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_pLight->SetSpecularPower(0.5f);

	//m_pTestModel = new CTextureModel();
	m_pTestModel = new CLightModel();
	if (FAILED(m_pTestModel->Initialize(pDevice,nullptr)))
	{
		return E_FAIL;
	}

	//m_pTestShader = new CTextureShader();
	m_pTestShader = new CLightShader();
	if (FAILED(m_pTestShader->Initialize(pDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CRenderer::Update()
{
	m_pCamera->Update();
}

void CRenderer::Render()
{
	m_pD3D->BeginScene(0.0f,0.0f,0.0f,1.0f);
	
	MatrixBufferType matrixBuffer;
	ID3D11DeviceContext* pDeviceContext = m_pD3D->GetDeviceContext();

	m_pD3D->GetWorldMatrix(matrixBuffer.world);
	m_pCamera->GetViewMatrix(matrixBuffer.view);
	m_pD3D->GetProjectionMatrix(matrixBuffer.projection);
	
	m_pTestModel->Render(pDeviceContext);

	CMaterial* pMaterial = m_pTestModel->GetMaterial();

	LightBufferType lightBuffer;

	if (m_pLight)
	{
		lightBuffer.ambientColor	 = m_pLight->GetAmbientColor();
		lightBuffer.diffuseColor	 = m_pLight->GetDiffuseColor();
		lightBuffer.specularColor	 = m_pLight->GetSpecularColor();
		lightBuffer.specularPower	 = m_pLight->GetSpecularPower();
		lightBuffer.lightDirection	 = m_pLight->GetDirection();
	}

	CameraBufferType cameraBuffer;
	if (m_pCamera)
	{
		cameraBuffer.cameraPosition = m_pCamera->GetPosition();
		cameraBuffer.padding = m_pCamera->GetPadding();
	}

	if(m_pTestModel->IsInstceUse() == false)
		m_pTestShader->Render(pDeviceContext, m_pTestModel->GetIndexCount(), pMaterial, &matrixBuffer, &lightBuffer);
	else
		m_pTestShader->Render(pDeviceContext,m_pTestModel->GetIndexCount(),m_pTestModel->GetInstanceCount(), pMaterial, &matrixBuffer, &lightBuffer);
	
	m_pD3D->EndScene();
}