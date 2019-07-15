#pragma once
#include <Windows.h>
#include "SingleTon.h"
#include "Renderer.h"

class CWinAPI : public CSingleTonT<CWinAPI>
{
	friend CSingleTonT;
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	CRenderer* m_pRenderer;

	bool m_bLoop;
protected:
	CWinAPI();
	virtual ~CWinAPI();
public:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

	void Loop();

	void SetLoop(bool bLoop) { m_bLoop = bLoop; };
};