#include "MyRaster.h"
#include "Graphic.h"
#include "DrawState.h"
#include "MyGraphic.h"
#include <algorithm>
#include <vector>
//using namespace State;
using namespace std;
// 像素坐标转换
void Raster::ChangeXYToGrid(int& x0, int& y0, int& x1, int& y1)
{
	x0 /= g_State.GridWidth; y0 /= g_State.GridHeight;
	x1 /= g_State.GridWidth; y1 /= g_State.GridHeight;
}
// 点集像素坐标转换
void Raster::ChangeOf_XYs(PixelPoint* Points, int Count)
{
	for (int i = 0; i < Count; i++)
	{
		Points[i].x /= g_State.GridWidth;
		Points[i].y /= g_State.GridHeight;
	}
}
void Raster::drawPoint(int x, int y, unsigned color)
{
	setPixel(x, y, color);
}
// 绘制网格像素
void Raster::drawCell(int col, int row, unsigned color)
{
	for (int i = row * g_State.GridHeight; i < (row+1) * g_State.GridHeight; ++i)
		for (int j = col * g_State.GridWidth; j < (col+1) * g_State.GridWidth; ++j)
			setPixel(j, i, color);
}

// 绘制网格背景图
void Raster::drawGrid()
{
	
	int Rows = (int)((float)getWindowHeight() / g_State.GridHeight + 0.5);
	int Cols = (int)((float)getWindowWidth() / g_State.GridWidth + 0.5);
	for (int col = 0; col <= Cols; ++col)
		for (int row = 0; row <= Rows; ++row)
		{
			drawCell(col, row, (row + col) % 2 ? g_State.GridColor1 : g_State.GridColor2);
		}
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
	int x0, y0, d, R = int(r);
	int xCenter = int(x), yCenter = int(y);
	x0 = 0;
	y0 = R;
	d = 1 - R;
	CirclePlot(xCenter, yCenter, x0, y0, color);
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
		CirclePlot(xCenter,yCenter, x0, y0, color);
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
	double Rx = width / 2, Ry = height / 2;
	int a = int(Rx);
	int b = int(Ry);
	int x0 = int(xCenter);
	int y0 = int(yCenter);
	int Rx2 = a * a, Ry2 = b * b;   // 椭圆半径的平方
	int x = 0, y = b;          // 相对中心偏移量x,y
	double d;  // 中点值
	EllipsePlot(x0, y0, x, y, color);
	/*区域1*/
	d = Ry2 - (Rx2 * Ry) + (0.25 * Rx2);
	while (Ry2*(x+1) < Rx2*(y-0.5))
	{
		
		if (d < 0)
		{
			d += Ry2*(2*x+3);
		}
		else
		{

			d += Ry2*(2*x+3) + Rx2*(-2*y+2);
			y--;
		}
		x++;
		EllipsePlot(x0, y0, x, y, color);
	}
	/*区域2*/
	d = Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2;
	while (y > 0)
	{
		if (d < 0) {

			d += Ry2 * (2 * x + 2) + Rx2 * (-2 * y + 3);
			x++;
		}
		else
		{
			d += Rx2*(-2 * y + 3);
		}
		y--;
		EllipsePlot(x0, y0, x, y, color);
	}
	//double Rx = width / 2, Ry = height / 2; // 椭圆半径x轴Rx, y轴半径Ry,
	//double Rx2 = Rx * Rx, Ry2 = Ry * Ry;   // 椭圆半径的平方
	//double twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;// 两倍的椭圆半径平方
	//double x = 0, y = Ry;          // 相对中心偏移量x,y
	//double d;  // 中点值
	//double px = twoRy2; 
	//double py = twoRx2 * y - Rx2;
	//EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	///*区域1*/
	//d = Ry2 - (Rx2 * Ry) + (0.25 * Rx2);
	//while (px < py)
	//{
	//	px += twoRy2;
	//	if (d < 0)
	//	{
	//		d += Ry2 + px;
	//	}
	//	else
	//	{
	//		d += Ry2 + px - py + Rx2;
	//		y--;
	//		py -= twoRx2;
	//	}
	//	x++;
	//	EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	//}
	///*区域2*/
	//d = Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2;
	//while (y >= 0)
	//{
	//	py -= twoRx2;
	//	if (d < 0) {
	//		d += Rx2 - py + px;
	//		x++;
	//		px += twoRx2;
	//	}
	//	else
	//	{
	//		d += Rx2 - py;
	//	}
	//	y--;
	//	EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	//}

}



















