#pragma once

#include <windows.h>
#include <vector>

namespace help {

	// window handle vector
	typedef std::vector<HWND> Wins;


	RECT centerWindow(RECT* workspace, long width, long height);

	// call after child window class is registered
	Wins drawNice(RECT* workspace, TCHAR* winClass, TCHAR* winTitle, HINSTANCE hInstance, int nCmdShow);

}

namespace pen {
	// parallelogram, used for bounding information for letters and letter groups
	// not used by letters because would require scaling these too when letters are scaled, plus they're not really used
	class PARM {
	public:
		// lower left
		Point ll;
		// top left
		Point tl;
		// lower right
		Point lr;
		// top right
		Point tr;

		PARM(
			Point ll = Point(0, 0),
			Point tl = Point(1.0 / 3.0, 1.0),
			Point tr = Point(1.0, 1.0),
			Point lr = Point(2.0 / 3.0, 0)
		) : ll(ll), tl(tl), lr(lr), tr(tr) {};
		~PARM() {};

		/*
		* @returns width of bounding rectangle
		* assumes parrallelogram is uniform
		*/
		double width() const;
		/*
		* @returns height of bounding rectangle
		* assumes parrallelogram is uniform
		*/
		double height() const;

	};
	class Point {
	public:
		double x;
		double y;

		Point(double x, double y) : x(x), y(y) {}

		/*
		* @param p1
		* @param p2
		* @returns distance between point 1 & 2
		*/
		double distance(Point p2) const;
		/*
		* @param by amount to scale by
		* scales the vector associated with the point, so pretty much multiplies its coordinates by [by]
		*/
		void scale(double by);
	};
	typedef std::vector<Point> Points;
	typedef std::vector<Points> LetterPoints;
	// windows along lines are drawn from start to end
	class Line {
	public:
		Point start;
		Point end;
		Line(Point start, Point end) : start(start), end(end) {};
		~Line() {};

		/**
		* @param pointsPerUnit number of points per unit of measurement, since unit is pixel should be less than 1
		* @returns points between the Line's start and end points
		*/
		Points interpolate(double pointsPerUnit, bool includeFirst = true, bool includeLast = true) const;

		/*
		* @returns length of line
		*/
		double length() const;
	};
	typedef  std::vector<Line> Lines;
	// points of parallelogram that bound letters, are the same for all letters
	struct Bounds { Point bl; Point tl; Point br; Point tr; };

	class Letter; // forward declaration for Letters typedef
	typedef std::vector<Letter> Letters;

	class Letter {
	public:
		double scaled;
		Lines lines;
		Letter(Lines lines) : lines(lines), scaled(1) {};
		~Letter() {};

		void scale(double by);

		/**
		* @param pointsPerUnit number of points per unit of measurement, since unit is pixel should be less than 1
		* @returns all of a letter's points in order
		*/
		Points interpolate(double pointsPerUnit) const;

		static LetterPoints interpolateLetters(Letters letters, double pointsPerUnit);
	};

	class LetterPresets {
	public:
		static const Letter N;
		static const Letter I;
		static const Letter C;
		static const Letter E;
	};
	/*
	* @param letters letters to scale
	* @param amount to scale [letters] by
	* scales [letters] by [by]
	*/
	void scaleLetters(Letters letters, double by);

	/*
	* @param numLetters number of letters in word
	* @param scale the factor by which letters are scaled by
	* @param padding the padding in pixels (i think) between letters
	*/
	PARM calculateBoundingParallelogram(int numLetters, double scale, double padding);
}