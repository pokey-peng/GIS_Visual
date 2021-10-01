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
	//TODO drawCell(未实现)
}

void Raster::drawGrid()
{
	//TODO drawGrid(未实现)
}

void Raster::drawLine(double x0, double y0, double x1, double y1, unsigned color)
{
	//TODO drawLine(未实现)
	MidPointLine(x0, y0, x1, y1, color);


}

void Raster::drawPolyline()
{
	//TODO drawPolyline(未实现）
}

void Raster::drawPolygon(PixelPoint* data, int size, unsigned color)
{
	//直接填充水平线
	HorizonEdgeFill(data, size, color);
	int ymin = 0;
	int ymax = 0;
	GetYMinMax(data, size, ymin, ymax); // 获取坐标点最大最小y值
	vector<State::tagEDGE> aetEDGE; // 活性边表AET
	vector<vector<State::tagEDGE>> etEDGE(ymax-ymin+1); // 边表
	InitET(etEDGE,data, size,ymin,ymax); // 初始化边表
	
	ScanLineFill(aetEDGE,etEDGE,ymin,ymax,color); // 进行扫描
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
	//TODO drawCircle(未完善)
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
	double Rx = width / 2, Ry = height / 2; // 椭圆半径x轴Rx, y轴半径Ry,
	double Rx2 = Rx * Rx, Ry2 = Ry * Ry;   // 椭圆半径的平方
	double twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;// 两倍的椭圆半径平方
	double x = 0, y = Ry;          // 相对中心偏移量x,y
	double d;  // 中点值
	double px = twoRy2; 
	double py = twoRx2 * y - Rx2;
	EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	/*区域1*/
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
	/*区域2*/
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



















