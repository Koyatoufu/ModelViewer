#include "stdafx.h"
#include "WinApi.h"
#include "resource.h"

#define MAX_LOADSTRING 100

WCHAR g_szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR g_szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CWinAPI::CWinAPI() : m_hWnd(NULL),m_hInstance(NULL),m_bLoop(false)
{
}

CWinAPI::~CWinAPI()
{
	CRenderer::ReleaseInstnace();
	m_pRenderer = nullptr;
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

	HWND hWnd = CreateWindowW(g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);
		
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	m_hWnd = hWnd;

	m_pRenderer = CRenderer::CreateInstance();

	if (FAILED(m_pRenderer->Initialize(m_hWnd)))
		return E_FAIL;

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
			if (m_pRenderer)
			{
				m_pRenderer->Update();
				m_pRenderer->Render();
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

