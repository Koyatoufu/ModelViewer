#include "stdafx.h"
#include "WinApi.h"
#include "resource.h"
#include "input.h"
#include "Renderer.h"
#include "Timer.h"
#include "CpuChecker.h"

#define MAX_LOADSTRING 100

WCHAR g_szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR g_szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWinAPI::CWinAPI() : m_hWnd(NULL),m_hInstance(NULL),m_bLoop(false)
{
}

CWinAPI::~CWinAPI()
{
	CCpuChecker::ReleaseInstnace();
	CTimer::ReleaseInstnace();

	CRenderer::ReleaseInstnace();
	CInput::ReleaseInstnace();
}

HRESULT CWinAPI::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VIEWER, g_szWindowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VIEWER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VIEWER);
	wcex.lpszClassName = g_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	m_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// 윈도우 화면 및 렌더러 화면 크기 등 설정
	int nWidth	= 800;
	int nHeight = 600;

	HWND hWnd = CreateWindowW(g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, nWidth, nHeight, nullptr, nullptr, hInstance, nullptr);
		
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	m_hWnd = hWnd;

	CRenderer* pRenderer = CRenderer::CreateInstance();

	if (FAILED(pRenderer->Initialize(m_hWnd, nWidth, nHeight,false,true)))
		return E_FAIL;

	CInput* pInput = CInput::CreateInstance();
	
	if (FAILED(pInput->Initialize(hInstance, hWnd, nWidth, nHeight)))
		return E_FAIL;

	CTimer::CreateInstance();
	CCpuChecker::CreateInstance();

	return S_OK;
}

void CWinAPI::Loop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	m_bLoop = true;

	while (m_bLoop && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (GetMessage(&msg, NULL, NULL, NULL))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			CTimer* pTimer = CTimer::GetInstance();
			CCpuChecker* pCpuChecker = CCpuChecker::GetInstance();
			CInput* pInput = CInput::GetInstance();
			CRenderer* pRenderer = CRenderer::GetInstance();

			float fDelta = 0.0f;

			if (pTimer)
			{
				pTimer->UpdateTimer();
				fDelta = pTimer->GetTickTime() / 60.0f;
			}

			if (pInput)
				pInput->Update();

			if (pRenderer)
			{
				pRenderer->Update(fDelta);
				pRenderer->Render();
			}
		}
	}
}

//// 정보 대화 상자의 메시지 처리기입니다.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}

