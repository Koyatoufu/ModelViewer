#pragma once

#include "RenderDefine.h"
#include "SingleTon.h"
#include "D3Dclass.h"

class CCamera;
class CColorModel;
class CColorShader;

class CRenderer:public CSingleTonT<CRenderer>
{
	friend CSingleTonT;
private:
	HWND m_hWnd;

	bool m_bVsyncEnabled;
	int m_nVideoCardMemory;
	char m_szVideoCardDescription[128];

	CCamera* m_pCamera;
	CColorModel* m_pTestModel;
	CColorShader* m_pShader;
	CD3DClass* m_pD3D;

private:
	CRenderer(CRenderer&) {};

protected:
	CRenderer();
	~CRenderer();
public:
	HRESULT Initialize(HWND hWnd);

	void Update();
	void Render();

	CCamera* GetRendererCamera() { return m_pCamera; }
};