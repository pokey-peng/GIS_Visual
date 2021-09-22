/******************************
*file base:MyGraphic.cpp                          
*��дͼ�ι�դ���㷨�Ľӿ�
*******************************/
#ifndef _MYGRAPHIC_H
#define _MYGRAPHIC_H
#include "Graphic.h"
#include <vector>
#include "DrawState.h"
using namespace std;


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


void GetYMinMax(PixelPoint* data,int size,int ymin, int ymax);
/*��ʼ���±߱�*/
void InitET(vector<vector<State::tagEDGE>>& etEDGE,PixelPoint* data,int size,int ymin,int ymax);
/*��ʼ����߱�*/
void InitAET();
/*�����ɨ�����*/
void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax,unsigned color);

#endif // !_MYGRAPHIC_H

