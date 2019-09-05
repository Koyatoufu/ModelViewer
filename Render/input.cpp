#include "input.h"

CInput::CInput():
	m_pDirectInput(nullptr),
	m_pKeyboardInput(nullptr),
	m_pMouseInput(nullptr),
	m_nScreenWidth(0),
	m_nScreenHeight(0),
	m_mousePoint(),
	m_nMouseWheelValue(0)
{
	m_mousePoint.x = 0;
	m_mousePoint.y = 0;
}

CInput::~CInput()
{
	if(m_pMouseInput)
		m_pMouseInput->Unacquire();
	if(m_pKeyboardInput)
		m_pKeyboardInput->Unacquire();

	SAFE_RELEASE_D3DCONTENTS(m_pDirectInput);
	SAFE_RELEASE_D3DCONTENTS(m_pKeyboardInput);
	SAFE_RELEASE_D3DCONTENTS(m_pMouseInput);
}

HRESULT CInput::Initialize(HINSTANCE hInstance, HWND hWnd, int nScreenWidth, int nScreenHeight)
{
	m_nScreenWidth = nScreenWidth;
	m_nScreenWidth = nScreenHeight;

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_pDirectInput,nullptr)))
		return E_FAIL;

	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard,&m_pKeyboardInput,nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboardInput->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyboardInput->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pKeyboardInput->Acquire()))
		return E_FAIL;

	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseInput, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouseInput->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if(FAILED(m_pMouseInput->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	if (FAILED(m_pMouseInput->Acquire()))
		return E_FAIL;

	return S_OK;
}

bool CInput::Update()
{
	if (FAILED(ReadKeyboard()))
		return false;

	if (FAILED(ReadMouse()))
		return false;

	ProcessInput();

	return true;
}

bool CInput::IsEscapePressed()
{
	if (m_szKeyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

POINT CInput::GetMouseLocation()
{
	return m_mousePoint;
}

HRESULT CInput::ReadKeyboard()
{
	HRESULT result = m_pKeyboardInput->GetDeviceState(sizeof(unsigned char) * 256, (LPVOID)&m_szKeyboardState);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_pKeyboardInput->Acquire();
		}
		else
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CInput::ReadMouse()
{
	HRESULT result = m_pMouseInput->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_pMouseInput->Acquire();
		}
		else
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void CInput::ProcessInput()
{
	m_mousePoint.x += m_mouseState.lX;
	m_mousePoint.y += m_mouseState.lY;

	m_nMouseWheelValue = m_mouseState.lZ;

	if (m_mousePoint.x < 0)
		m_mousePoint.x = 0;
	if(m_mousePoint.y < 0)
		m_mousePoint.y = 0;

	if (m_mousePoint.x > m_nScreenWidth)
		m_mousePoint.x = m_nScreenWidth;
	if (m_mousePoint.y > m_nScreenHeight)
		m_mousePoint.y = m_nScreenHeight;
}
