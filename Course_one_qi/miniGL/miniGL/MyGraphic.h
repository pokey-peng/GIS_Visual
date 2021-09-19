/******************************
*file base:MyGraphic.cpp                          
*编写图形光栅化算法的接口
*******************************/
#ifndef _MYGRAPHIC_H
#define _MYGRAPHIC_H
#include "Graphic.h"


/*线的光栅化--中点Bresenham算法*/
void MidPointLine(PixelPoint p1, PixelPoint p2);

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


#endif // !_MYGRAPHIC_H

