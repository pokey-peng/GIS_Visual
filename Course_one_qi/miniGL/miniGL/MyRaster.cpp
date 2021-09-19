#include "MyRaster.h"
#include "Graphic.h"
#include "DrawState.h"
#include "MyGraphic.h"
#include <algorithm>

void Raster::drawPoint(int x, int y, unsigned color)
{
	setPixel(x, y, color);
}
void Raster::drawCell(int row, int col, unsigned color)
{
	//TODO drawCell(未实现)
}

void Raster::drawGrid()
{
	//TODO drawGrid(未实现)
}

void Raster::drawLine(int x0, int y0, int x1, int y1, unsigned color)
{
	//TODO drawLine(未实现)


}

void Raster::drawPolyline()
{
	//TODO drawPolyline(未实现）
}

void Raster::drawPolygon(PixelPoint* data, int size, unsigned color)
{
	//TODO drawPolygon(未实现)
	data[0].x = 3;

}

void Raster::drawPolygonOutline()
{
	//TODO drawPolygonOutline(未实现)
}

void Raster::drawRectangle()
{
	//TODO drawRectangle(未实现)
}

void Raster::drawRectangleOutline()
{
	//TODO drawRectangleOutline(未实现)
}

void Raster::drawCircle(double x, double y, double r, unsigned color)
{
	//TODO drawCircle(未实现)
	//setOrig(x, y);
	// MidBresenham algorithm
	int x0, y0, d,xCenter,yCenter;

	x0 = 0;
	y0 = r;
	d = 1 - r;
	xCenter = x;
	yCenter = y;
	setPixel(xCenter, xCenter + y0, color);
	while (x0 <= y0)
	{
		if (d < 0)
		{
			d += 2 * x0 + 3;
		}
		else
		{
			d += 2 * (x0 - y0) + 5;
			y0--;
		}
		x0++;
		CirclePlot(xCenter, yCenter, x0, y0, color);
	}
}

void Raster::drawCircleOutline(double x, double y, double r, unsigned color)
{
	//TODO drawCircleOutline未完成
	/*
	setOrig(x, y);
	// MidBresenham algorithm
	int x0 = 0, y0 = r;
	double d = 1.25 - r;
	setPixel(x0, y0, color);
	setPixel(x0, -y0, color);
	setPixel(-y0, x0, color);
	setPixel(y0, x0, color);
	while (x <= y)
	{
		if (d < 0)
		{
			d += 2 * x0 + 3;
		}
		else
		{
			d += 2 * (x0 - y0) + 5;
			y0--;
		}
		x0++;
		setPixel(x0, y0, color);
		setPixel(x0, -y0, color);
		setPixel(-y0, x0, color);
		setPixel(y0, x0, color);
	}
	*/

	
}

void Raster::drawEllipse(double xCenter, double yCenter, double width, double height, unsigned color)
{
	/*
	int ra = width / 2;
	int rb = height / 2;
	int ra2 = ra * ra;
	int rb2 = rb * rb;
	int px = 0;
	int py = 2 * ra2 * rb;
	int xD = 0;
	int yD = rb;
	
	int xCenter = x;
	int yCenter = y;
	EllipsePlot(xCenter, yCenter, xD, yD,color);
	double p = rb2 - (ra2 * rb) + 0.25 * ra2;
	while (ra2*(yD - 0.5) > rb2*(xD + 1))
	{
		if (p < 0)
		{
			p += rb2  * (xD + xD + 3);
		}
		else
		{
			p += rb2 * (xD + xD + 3) + ra2 * (-yD -yD + 2);
			yD--;
		}
		xD++;
		EllipsePlot(xCenter, yCenter, xD, yD, color);
	}
	p = rb2 * (xD + 0.5) * (x + 0.5) + ra2 * (yD - 1) * (y - 1) - ra2 * rb2;
	while (yD > 0)
	{
		if (p > 0)
		{
			p += rb2 * (xD + xD + 2) + ra2 * (3 - yD - yD);
			xD++;
		}
		else
		{
			p += ra2 * (3 - yD - yD);
		}
		yD--;
		EllipsePlot(xCenter, yCenter, xD, yD, color);
	}
	*/
	int Rx = width / 2;
	int Ry = height / 2;
	int Rx2 = Rx * Rx;
	int Ry2 = Ry * Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2 * y;
	EllipsePlot(x, yCenter, x, y,color);
	/*区域1*/
	p = round(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
	while (px < py)
	{
		px += twoRy2;
		if (p < 0)
		{
			p += Ry2 + px;
		}
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		x++;
		EllipsePlot(xCenter, yCenter, x, y,color);
	}
	/*区域2*/
	p = round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
	while (y > 0)
	{
		py -= twoRx2;
		if (p > 0) {
			p += Rx2 - py;
		}
		else
		{
			x++;
			px += twoRx2;
			p += Rx2 - py + px;
		}
		y--;
		EllipsePlot(xCenter, yCenter, x, y,color);
	}

}



















