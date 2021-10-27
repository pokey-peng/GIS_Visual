#include "MyRaster.h"
#include "Graphic.h"
#include "DrawState.h"
#include "MyGraphic.h"
#include <algorithm>
#include <vector>
//using namespace State;
using namespace std;
// ��������ת��
void Raster::ChangeXYToGrid(int& x0, int& y0, int& x1, int& y1)
{
	x0 /= g_State.GridWidth; y0 /= g_State.GridHeight;
	x1 /= g_State.GridWidth; y1 /= g_State.GridHeight;
}
// �㼯��������ת��
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
// ������������
void Raster::drawCell(int col, int row, unsigned color)
{
	for (int i = row * g_State.GridHeight; i < (row+1) * g_State.GridHeight; ++i)
		for (int j = col * g_State.GridWidth; j < (col+1) * g_State.GridWidth; ++j)
			setPixel(j, i, color);
}

// �������񱳾�ͼ
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
	//TODO drawLine(δʵ��)
	MidPointLine(x0, y0, x1, y1, color);


}

void Raster::drawPolyline()
{
	//TODO drawPolyline(δʵ�֣�
}

void Raster::drawPolygon(PixelPoint* data, int size, unsigned color)
{
	//ֱ�����ˮƽ��
	HorizonEdgeFill(data, size, color);
	int ymin = 0;
	int ymax = 0;
	GetYMinMax(data, size, ymin, ymax); // ��ȡ����������Сyֵ
	vector<State::tagEDGE> aetEDGE; // ���Ա߱�AET
	vector<vector<State::tagEDGE>> etEDGE(ymax-ymin+1); // �߱�
	InitET(etEDGE,data, size,ymin,ymax); // ��ʼ���߱�
	
	ScanLineFill(aetEDGE,etEDGE,ymin,ymax,color); // ����ɨ��
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
	double Rx = width / 2, Ry = height / 2;
	int a = int(Rx);
	int b = int(Ry);
	int x0 = int(xCenter);
	int y0 = int(yCenter);
	int Rx2 = a * a, Ry2 = b * b;   // ��Բ�뾶��ƽ��
	int x = 0, y = b;          // �������ƫ����x,y
	double d;  // �е�ֵ
	EllipsePlot(x0, y0, x, y, color);
	/*����1*/
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
	/*����2*/
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
	//double Rx = width / 2, Ry = height / 2; // ��Բ�뾶x��Rx, y��뾶Ry,
	//double Rx2 = Rx * Rx, Ry2 = Ry * Ry;   // ��Բ�뾶��ƽ��
	//double twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;// ��������Բ�뾶ƽ��
	//double x = 0, y = Ry;          // �������ƫ����x,y
	//double d;  // �е�ֵ
	//double px = twoRy2; 
	//double py = twoRx2 * y - Rx2;
	//EllipsePlot(round(xCenter), round(yCenter), round(x), round(y), color);
	///*����1*/
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
	///*����2*/
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



















