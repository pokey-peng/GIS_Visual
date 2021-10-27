#pragma once
#include "Graphic.h"


class Raster
{
public:
	static void drawCell(int row, int col , unsigned color );
	static void drawGrid();
	static void drawPoint(double x, double y);
	static void drawLine(double x1, double y1, double x2, double y2, unsigned color);
	static void drawPolygonOutline(PixelPoint* pts, int size, unsigned color);
	static void drawTriangleOutline(PixelPoint* pts, unsigned color);
	static void drawCircleOutline(double x, double y, double r, unsigned color);
	static void drawEllipses(double xc, double yc, double a, double b, unsigned color);
	static void drawTriangle(PixelPoint* pts,unsigned color);
	//static void drawPolygon(PixelPoint* pts, int size, unsigned color);
};


