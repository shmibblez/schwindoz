#pragma once

#include <windows.h>
#include <vector>

namespace help {

	struct Point { long x; long y; };
	// windows along lines are drawn from start to end
	struct Line { Point start; Point end; };
	typedef  std::vector<Line> Lines;
	// lines are drawn in order
	struct { Lines segments; } Letter;

	// window handle vector
	typedef std::vector<HWND> Wins;


	RECT centerWindow(RECT* workspace, long width, long height);

	// call after child window class is registered
	Wins drawLolXD(RECT* workspace, TCHAR* winClass, TCHAR* winTitle, HINSTANCE hInstance, int nCmdShow);

}