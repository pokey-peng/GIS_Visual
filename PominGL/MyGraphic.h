/******************************
*file base:MyGraphic.cpp                          
*编写图形光栅化算法的接口
*******************************/
#ifndef _MYGRAPHIC_H
#define _MYGRAPHIC_H
#include "Graphic.h"
#include <vector>
#include "DrawState.h"
using namespace std;


/*线的光栅化--中点Bresenham算法*/
void MidPointLine(double x0, double y0, double x1, double y1, unsigned color);

/*画折线*/
void DrawPolyline();

/*画空心矩形*/
void DrawRectangle();

/*画空心多边形*/
void DrawPolygon();

/*画圆八个点对应位置*/
void CirclePlot(int x, int y, int xa,int ya, unsigned color);

/*画椭圆四个对应点*/
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color);


/*水平线直接填充*/
void HorizonEdgeFill(PixelPoint* data, int size, unsigned color);
void GetYMinMax(PixelPoint* data,int size,int& ymin, int& ymax);
/*初始化新边表*/
void InitET(vector<vector<State::tagEDGE>>& etEDGE,PixelPoint* data,int size,int ymin,int ymax);
/*初始化活动边表*/
void InitAET();
/*多边形扫描填充*/
void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax,unsigned color);

/*区域填充算法*/

void PointFill(int x, int y, unsigned newColor); // 内部点定义区域
void BoundaryFill(int x, int y, unsigned bdrColor, unsigned newColor); // 边界定义的区域
void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor);
#endif // !_MYGRAPHIC_H

