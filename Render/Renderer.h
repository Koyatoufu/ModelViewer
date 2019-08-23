#pragma once

#include "RenderDefine.h"
#include "SingleTon.h"
#include "D3Dclass.h"

class CCamera;
class CBaseModel;
class CShader;
class CLight;

class CRenderer:public CSingleTonT<CRenderer>
{
	friend CSingleTonT;
private:
	HWND m_hWnd;

	bool m_bVsyncEnabled;
	int m_nVideoCardMemory;
	char m_szVideoCardDescription[128];

	CCamera* m_pCamera;
	CBaseModel* m_pTestModel;
	CShader* m_pTestShader;
	CLight*	m_pLight;

	CD3DClass* m_pD3D;

	float m_fTestRotation;

private:
	CRenderer(CRenderer&) {};

protected:
	CRenderer();
	~CRenderer();
public:
	HRESULT Initialize(HWND hWnd,int nWidth, int nHeight, bool bFullScreen, bool bSync);

	void Update();
	void Render();

	CCamera* GetRendererCamera() { return m_pCamera; }
	CLight* GetLight() { return m_pLight; }
	CD3DClass* GetD3D() { return m_pD3D; }
};