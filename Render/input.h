#pragma once
#include "RenderDefine.h"
#include "SingleTon.h"
#include <dinput.h>

class CInput:public CSingleTonT<CInput>
{
	friend CSingleTonT;
private:
	IDirectInput8* m_pDirectInput;
	IDirectInputDevice8* m_pKeyboardInput;
	IDirectInputDevice8* m_pMouseInput;
	unsigned char		 m_szKeyboardState[256];
	DIMOUSESTATE		 m_mouseState;

	int	m_nScreenWidth;
	int m_nScreenHeight;
	POINT				 m_mousePoint;
	long				 m_nMouseWheelValue;
public:
	HRESULT Initialize(HINSTANCE hInstance, HWND hWnd, int nScreenWidth, int nScreenHeight);

	bool Update();

	bool IsEscapePressed();
	POINT GetMouseLocation();
	long	GetMouseWheelValue() { return m_nMouseWheelValue; }
protected:
	CInput();
	~CInput();
private:
	CInput(CInput&) {}

	HRESULT	ReadKeyboard();
	HRESULT	ReadMouse();
	void	ProcessInput();
};