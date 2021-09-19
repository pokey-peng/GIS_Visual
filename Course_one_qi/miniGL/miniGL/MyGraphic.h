/******************************
*file base:MyGraphic.cpp                          
*��дͼ�ι�դ���㷨�Ľӿ�
*******************************/
#ifndef _MYGRAPHIC_H
#define _MYGRAPHIC_H
#include "Graphic.h"


/*�ߵĹ�դ��--�е�Bresenham�㷨*/
void MidPointLine(PixelPoint p1, PixelPoint p2);

/*������*/
void DrawPolyline();

/*�����ľ���*/
void DrawRectangle();

/*�����Ķ����*/
void DrawPolygon();

/*��Բ�˸����Ӧλ��*/
void CirclePlot(int x, int y, int xa,int ya, unsigned color);

/*����Բ�ĸ���Ӧ��*/
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color);


#endif // !_MYGRAPHIC_H

