#ifndef _MYRASTER_H
#define _MYRASTER_H
#include "Graphic.h"

class Raster
{
public:
	static void drawPoint(int x, int y, unsigned color);
	static void drawCell(int row, int col, unsigned color);
	static void drawGrid();
	static void drawLine(double x0, double y0, double x1, double y1, unsigned color);
	static void drawPolyline();
	static void drawPolygon(PixelPoint* data,int size, unsigned color);
	static void drawPolygonOutline();
	static void drawRectangle();
	static void drawRectangleOutline();
	static void drawCircle(double x, double y, double r, unsigned color);
	static void drawCircleOutline(double x, double y, double r, unsigned color);
	static void drawEllipse(double xCenter, double yCenter, double width, double height, unsigned color);

	static void ChangeXYToGrid(int& x0, int& y0, int& x1, int& y1);
	static void ChangeOf_XYs(PixelPoint* Points, int Count);

};
#endif // !_MYRASTER_H
