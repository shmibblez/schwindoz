#include "helpers.h"
#include <math.h>
#include <vector>

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

	Wins drawNice(RECT* workspace, TCHAR* winClass, TCHAR* winTitle, HINSTANCE hInstance, int nCmdShow) {
		// TODO: draw letters, currently just draws windows along diagonal
		// should also probably move to different thread, add callback when 
		// done displaying all windows, and then end program

		Wins handles;

		pen::Letters letters = {
			pen::LetterPresets::N,
			pen::LetterPresets::I,
			pen::LetterPresets::C,
			pen::LetterPresets::E,
		};

		pen::PARM initialBounds = pen::PARM();

		double unscaledPadding = 0.5;
		pen::PARM unscaledBoundingPARM = pen::calculateBoundingParallelogram(letters.size(), 1, unscaledPadding);
		double unscaledWidth = unscaledBoundingPARM.width();
		double unscaledHeight = unscaledBoundingPARM.height();
		double wordSizeRatio = unscaledWidth / unscaledHeight;

		double screenWidth = workspace->right - workspace->left;
		double screenHeight = workspace->top - workspace->bottom;
		double screenRatio = screenWidth / screenHeight;

		// determine which dimension limits scale factor
		double scaleLimiter = wordSizeRatio > screenRatio ?
			screenHeight : screenWidth;

		double letterScale; // each letter's bounding box will be this many pixels high and wide

		if (wordSizeRatio < screenRatio) {
			// if screen height limits bounds
			letterScale = screenHeight / unscaledHeight;
		}
		else {
			// if screen width limits bounds
			letterScale = screenWidth / unscaledWidth;
		}

		double pointsPerUnit = 6 / ((2 / 3) * letterScale); // means window will be drawn every this many pixels (6 windows per unit in this case)

		pen::scaleLetters(letters, letterScale);
		double scaledPadding = unscaledPadding * letterScale; // determines padding between letters in px

		pen::LetterPoints letterPoints = pen::Letter::interpolateLetters(letters, pointsPerUnit);

		// TODO: determine center offset and add to each window's coordinate when drawing

		pen::PARM scaledPARM = pen::calculateBoundingParallelogram(letters.size(), letterScale, scaledPadding);
		// horizontal offset of letter's bottom left corner
		double initialblHorzOffset = workspace->right - (screenWidth / 2) - (scaledPARM.width() / 2);
		double initialblVertOffset = workspace->top - (screenHeight / 2) - (scaledPARM.height() / 2);
		double blHorzOffset = 0;
		for (int i = 0; i < letterPoints.size(); i++) {
			blHorzOffset = initialblHorzOffset + i * (2 / 3) * letterScale + (i - 1) * scaledPadding;
			for (int l = 0; l < letterPoints[i].size(); l++) {
				HWND childWnd = CreateWindow(
					winClass,
					winTitle,
					WS_OVERLAPPEDWINDOW,
					letterPoints[i][l].x + blHorzOffset, letterPoints[i][l].y + initialblVertOffset,
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
		}




		//// number of windows to draw
		//int num_windows = 30;

		//pen::Point start{ workspace->left, workspace->top };
		//pen::Point end{ workspace->right,workspace->bottom };
		//pen::Point unit{ (end.x - start.x) / num_windows, (end.y - start.y) / num_windows };


		//for (int i = 0; i < num_windows; i++) {
		//	pen::Point temp_unit{ unit.x * i, unit.y * i };
		//	// create child window
		//	HWND childWnd = CreateWindow(
		//		winClass,
		//		winTitle,
		//		WS_OVERLAPPEDWINDOW,
		//		temp_unit.x, temp_unit.y,
		//		100, 100,
		//		NULL,
		//		NULL,
		//		hInstance,
		//		NULL
		//	);
		//	handles.push_back(childWnd);
		//	ShowWindow(childWnd, nCmdShow);
		//	UpdateWindow(childWnd);
		//}

		return handles;
	}

}

namespace pen {
	double PARM::width() const {
		return this->tr.x - this->ll.x;
	}
	double PARM::height() const {
		return this->tr.y - this->ll.y;
	}
	double Point::distance(Point p2) const {
		return sqrt(
			(this->x - p2.x) * (this->x - p2.x) +
			(this->y - p2.y) * (this->y - p2.y)
		);
	}
	double Line::length() const {
		return sqrt(
			(this->start.x - this->end.x) * (this->start.x - this->end.x) +
			(this->start.y - this->end.y) * (this->start.y - this->end.y)
		);
	}
	Points Line::interpolate(double pointsPerUnit, bool includeFirst, bool includeLast) const {
		Points ps;
		int numPoints = pointsPerUnit * this->length();

		if (includeFirst)
			ps.push_back(this->start);
		for (int i = 1; i < numPoints; i++) {
			double x = std::abs((this->end.x - this->start.x) / numPoints) * i;
			double y = std::abs((this->end.y - this->start.y) / numPoints) * i;
			ps.push_back(Point(x, y));
		}
		if (includeLast)
			ps.push_back(this->end);
		return ps;
	}
	void Point::scale(double by) {
		this->x = this->x * by;
		this->y = this->y * by;
	}
	void Letter::scale(double by) {
		double scaleFactor = by / this->scaled;
		for (int i = 0; i < this->lines.size(); i++) {
			this->lines[i].start.scale(scaleFactor);
			this->lines[i].end.scale(scaleFactor);
		}
	}
	Points Letter::interpolate(double pointsPerUnit) const {
		Points ps;
		for (int i = 0; i < this->lines.size(); i++) {
			bool includeFirst = i == 0 ? true : false;
			// points for line
			Points lps = this->lines[i].interpolate(pointsPerUnit, includeFirst, true);
			// append lps to ps
			ps.insert(std::end(ps), std::begin(lps), std::end(lps));
		}
		return ps;
	}
	LetterPoints Letter::interpolateLetters(Letters letters, double pointsPerUnit) {
		LetterPoints ps;
		for (int i = 0; i < letters.size(); i++) {
			//Points lps = letters[i].interpolate(pointsPerUnit);
			//ps.insert(std::end(ps), std::begin(lps), std::end(lps));
			ps.push_back(letters[i].interpolate(pointsPerUnit));
		}
		return ps;
	}
	const Letter LetterPresets::N = Letter({
		Line(Point(0,0), Point(1.0 / 3.0, 1)),
		Line(Point(1.0 / 3.0, 1.0), Point(2.0 / 3.0, 0.0)),
		Line(Point(2.0 / 3.0, 0.0), Point(1.0, 1.0)),
		});
	const Letter LetterPresets::I = Letter({
		Line(Point(1.0 / 3.0, 1.0), Point(1.0 , 1.0)),
		Line(Point(2.0 / 3.0, 1.0), Point(1.0 / 3.0, 0.0)),
		Line(Point(0, 0), Point(2.0 / 3.0, 0)),
		});
	const Letter LetterPresets::C = Letter({
		Line(Point(2.0 / 3.0, 0.0), Point(0.0 , 0.0)),
		Line(Point(0.0, 0.0), Point(1.0 / 3.0, 1.0)),
		Line(Point(1.0 / 3.0, 1.0), Point(1.0, 1.0)),
		});
	const Letter LetterPresets::E = Letter({
		Line(Point(1.0, 1.0), Point(1.0 / 3.0 , 1.0)),
		Line(Point(5.0 / 6.0, 0.5), Point(1.0 / 6.0, 0.5)),
		Line(Point(1.0 / 3.0, 0.0), Point(0.0, 0.0)),
		});

	void pen::scaleLetters(Letters letters, double by) {
		for (int i = 0; i < letters.size(); i++) {
			letters[i].scale(by);
		}
	}
	PARM pen::calculateBoundingParallelogram(int numLetters, double scale, double padding) {
		// all letter parms have following coordinates:
		// - ll: (0  , 0)
		// - tl: (1/3, 1)
		// - lr: (2/3, 0)
		// - tr: (1  , 1)
		// 2/3 since that's base width of all letters
		int horizontalSideLength = (2 / 3) * numLetters * scale + (numLetters - 1) * padding;
		int parmHeight = scale;
		return PARM(
			Point(0, 0),
			Point((1 / 3) * scale, parmHeight),
			Point((1 / 3) * scale + horizontalSideLength, parmHeight),
			Point(horizontalSideLength, 0)
		);
	}
}