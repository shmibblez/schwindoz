#include "helpers.h"

namespace help {

	RECT centerWindow(RECT* desktop, long width, long height) {

		long center_x = (desktop->right - desktop->left) / 2;
		long center_y = (desktop->bottom - desktop->top) / 2;
		long half_width = width / 2;
		long half_height = height / 2;

		RECT win;
		win.left = center_x - half_width;
		win.right = center_x + half_width;
		win.top = center_y - half_height;
		win.bottom = center_y + half_height;

		return win;
	}

	Wins drawLolXD(RECT* workspace, TCHAR* winClass, TCHAR* winTitle, HINSTANCE hInstance, int nCmdShow) {
		// TODO: draw letters, currently just draws windows along diagonal
		// should also draw in thread or async somehow so all windows aren't
		// drawn immediately, but instead one after another, looks much better
		// also would look cool if windows are removed one after another in same order
		// they were added in -> if they were "kept alive" for a set amount of time
		//
		// what you could do then is add all of them to a single window array, and in thread
		// add them 1 after another, then after a while remove them in the same order ->
		// 1. create windows & store in array, 2: show windows, 3: destroy/hide windows

		Wins handles;

		// number of windows to draw
		int num_windows = 30;

		Point start{ workspace->left, workspace->top };
		Point end{ workspace->right,workspace->bottom };
		Point unit{ (end.x - start.x) / num_windows, (end.y - start.y) / num_windows };


		for (int i = 0; i < num_windows; i++) {
			Point temp_unit{ unit.x * i, unit.y * i };
			// create child window
			HWND childWnd = CreateWindow(
				winClass,
				winTitle,
				WS_OVERLAPPEDWINDOW,
				temp_unit.x, temp_unit.y,
				100, 100,
				NULL,
				NULL,
				hInstance,
				NULL
			);
			handles.push_back(childWnd);
			ShowWindow(childWnd, nCmdShow);
			UpdateWindow(childWnd);
		}

		return handles;
	}

}