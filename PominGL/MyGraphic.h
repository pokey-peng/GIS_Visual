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
void MidPointLine(double x0, double y0, double x1, double y1, unsigned color);

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


/*ˮƽ��ֱ�����*/
void HorizonEdgeFill(PixelPoint* data, int size, unsigned color);
void GetYMinMax(PixelPoint* data,int size,int& ymin, int& ymax);
/*��ʼ���±߱�*/
void InitET(vector<vector<State::tagEDGE>>& etEDGE,PixelPoint* data,int size,int ymin,int ymax);
/*��ʼ����߱�*/
void InitAET();
/*�����ɨ�����*/
void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax,unsigned color);

/*��������㷨*/

void PointFill(int x, int y, unsigned newColor); // �ڲ��㶨������
void BoundaryFill(int x, int y, unsigned bdrColor, unsigned newColor); // �߽綨�������
void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor);
#endif // !_MYGRAPHIC_H

