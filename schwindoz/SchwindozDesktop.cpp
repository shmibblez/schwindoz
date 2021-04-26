// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <string>

// schwindoz stuff
#include "helpers.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("SchwindozMain");

// child window class name.
static TCHAR cWindowClass[] = _T("SchwindozChild");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Welcome to Schwindoz, the latest in windows technology");

// child window class title.
static TCHAR cTitle[] = _T("child window");

HINSTANCE hInst;

static RECT desktop_rect;
static RECT main_window_rect;

static HWND mainWin;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(125, 0, 0));// default: (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;

	RECT work_area;

	// Get desktop params for available width and height
	SystemParametersInfo(
		SPI_GETWORKAREA,
		0,
		&work_area,
		0
	);
	desktop_rect = work_area;

	RECT window_rect = help::centerWindow(&work_area, 450, 200);
	main_window_rect = window_rect;

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		window_rect.left, window_rect.top,
		window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	mainWin = hWnd;

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// child window class
	WNDCLASSEX wcexc;
	wcexc.cbSize = sizeof(WNDCLASSEX);
	wcexc.style = CS_HREDRAW | CS_VREDRAW;
	wcexc.lpfnWndProc = WndProc;
	wcexc.cbClsExtra = 0;
	wcexc.cbWndExtra = 0;
	wcexc.hInstance = hInstance;
	wcexc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcexc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexc.hbrBackground = CreateSolidBrush(RGB(125, 0, 0));// default: (HBRUSH)(COLOR_WINDOW + 1);
	wcexc.lpszMenuName = NULL;
	wcexc.lpszClassName = cWindowClass;
	wcexc.hIconSm = LoadIcon(wcexc.hInstance, IDI_APPLICATION);

	// register child window class
	if (!RegisterClassEx(&wcexc))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}
	//
	//// create child window
	//HWND childWnd = CreateWindow(
	//	cWindowClass,
	//	cTitle,
	//	WS_OVERLAPPEDWINDOW,
	//	0, 0,
	//	50, 50,
	//	NULL,
	//	NULL,
	//	hInstance,
	//	NULL
	//);

	//if (!childWnd)
	//{
	//	MessageBox(NULL,
	//		_T("Call to CreateWindow failed!"),
	//		_T("Windows Desktop Guided Tour"),
	//		NULL);

	//	return 1;
	//}

	//// show child window
	//ShowWindow(childWnd, nCmdShow);
	//UpdateWindow(childWnd);

	help::drawLolXD(&desktop_rect, cWindowClass, cTitle, hInstance, nCmdShow);


	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("hello, hows it goin");
	TCHAR child_greeting[] = _T("nice");

	switch (message)
	{
	case WM_PAINT: {
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"
		// in the top left corner.
		/*LPCWSTR str = L"epic";
		DrawText(
			hdc,
			str,
			4,
			&main_window_rect,
			DT_CENTER
		);*/
		TextOut(
			hdc,
			5, 5,
			(hWnd == mainWin) ? greeting : child_greeting,
			(hWnd == mainWin) ? _tcslen(greeting) : _tcslen(child_greeting)
		);
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		if (hWnd == mainWin) {
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}