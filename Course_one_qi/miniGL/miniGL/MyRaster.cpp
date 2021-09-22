#include "MyRaster.h"
#include "Graphic.h"
#include "DrawState.h"
#include "MyGraphic.h"
#include <algorithm>
#include <vector>
//using namespace State;
using namespace std;

void Raster::drawPoint(int x, int y, unsigned color)
{
	setPixel(x, y, color);
}
void Raster::drawCell(int row, int col, unsigned color)
{
	//TODO drawCell(δʵ��)
}

void Raster::drawGrid()
{
	//TODO drawGrid(δʵ��)
}

void Raster::drawLine(int x0, int y0, int x1, int y1, unsigned color)
{
	//TODO drawLine(δʵ��)


}

void Raster::drawPolyline()
{
	//TODO drawPolyline(δʵ�֣�
}

void Raster::drawPolygon(PixelPoint* data, int size, unsigned color)
{
	//TODO drawPolygon(δʵ��)
	int ymin = 0;
	int ymax = 0;
	GetYMinMax(data,size,ymin,ymax);
	vector<State::tagEDGE> aetEDGE; // ���Ա߱�AET
	vector<vector<State::tagEDGE>> etEDGE(ymax-ymin+1); // �߱�
	InitET(etEDGE,data, size,ymin,ymax);
	ScanLineFill(aetEDGE,etEDGE,ymin,ymax,color);
}

void Raster::drawPolygonOutline()
{
	//TODO drawPolygonOutline(δʵ��)
}

void Raster::drawRectangle()
{
	//TODO drawRectangle(δʵ��)
}

void Raster::drawRectangleOutline()
{
	//TODO drawRectangleOutline(δʵ��)
}

void Raster::drawCircle(double x, double y, double r, unsigned color)
{
	//TODO drawCircle(δ����)
	//setOrig(x, y);
	// MidBresenham algorithm
	double x0, y0, d;

	x0 = 0;
	y0 = r;
	d = 1 - r;
	CirclePlot(round(x), round(y),round(x0),round(y0), color);
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
		CirclePlot(round(x), round(y), round(x0), round(y0), color);
	}
}

void Raster::drawCircleOutline(double x, double y, double r, unsigned color)
{
	//TODO drawCircleOutlineδ���
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
	double Rx = width / 2, Ry = height / 2; // ��Բ�뾶x��Rx, y��뾶Ry,
	double Rx2 = Rx * Rx, Ry2 = Ry * Ry;   // ��Բ�뾶��ƽ��
	double twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;// ��������Բ�뾶ƽ��
	double x = 0, y = Ry;          // �������ƫ����x,y
	double d;  // �е�ֵ
	double px = twoRy2; 
	double py = twoRx2 * y - Rx2;
	EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	/*����1*/
	d = Ry2 - (Rx2 * Ry) + (0.25 * Rx2);
	while (px < py)
	{
		px += twoRy2;
		if (d < 0)
		{
			d += Ry2 + px;
		}
		else
		{

			d += Ry2 + px - py + Rx2;
			y--;
			py -= twoRx2;
		}
		x++;
		EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	}
	/*����2*/
	d = Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2;
	while (y > 0)
	{
		py -= twoRx2;
		if (d < 0) {

			d += Rx2 - py + px;
			x++;
			px += twoRx2;
		}
		else
		{
			d += Rx2 - py;
		}
		y--;
		EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	}

}



















