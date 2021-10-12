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


/// <summary>
/// �ڲ��㼯������������㷨
/// </summary>
/// <param name="x">���ӵ�x����</param>
/// <param name="y">���ӵ�y����</param>
/// <param name="newColor">����������������ɫ</param>
void PointFill(int x, int y, unsigned newColor);

/// <summary>
/// �߽綨����������㷨
/// </summary>
/// <param name="x">���ӵ�x����</param>
/// <param name="y">���ӵ�y����</param>
/// <param name="bdrColor">�߽����ɫ</param>
/// <param name="newColor">������������ɫ</param>
void BoundaryFill(int x, int y, unsigned bdrColor, unsigned newColor);

/// <summary>
/// ɨ������������㷨
/// </summary>
/// <param name="x">���ӵ�x����</param>
/// <param name="y">���ӵ�y����</param>
/// <param name="bdrColor">�߽���ɫ</param>
/// <param name="newColor">�����ɫ</param>
void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor);
#endif // !_MYGRAPHIC_H

