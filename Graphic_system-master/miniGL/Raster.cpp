#include "Raster.h"
#include "State.h"
#include "Graphic.h"
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <list>
#include <iostream>
#include"GeometryContainer.h"

//��һ��С������㷨
void Raster::drawCell(int row, int col, unsigned color)
{
	for (int i = row * g_State.GridHeight; i < (row + 1) * g_State.GridHeight; ++i)
		for (int j = col * g_State.GridWidth; j < (col + 1) * g_State.GridWidth; ++j)
			setPixel(j, i, color);
}

//�����̸���㷨
void Raster::drawGrid()
{
	int orig_x, orig_y;
	getOrig(orig_x, orig_y);

	int left = (int)((float)orig_x / g_State.GridWidth + 0.5);
	int right = (int)((float)(getWindowWidth() - orig_x) / g_State.GridWidth + 0.5);
	int top = (int)((float)orig_y / g_State.GridHeight + 0.5);
	int bottom = (int)((float)(getWindowHeight() - orig_y) / g_State.GridHeight + 0.5);

	int ymin, ymax;
	if (isYUp())
	{
		ymin = -bottom;
		ymax = top;
	}
	else
	{
		ymin = -top;
		ymax = bottom;
	}
	for (int col = -left; col <= right; ++col)
		for (int row = ymin; row <= ymax; ++row)
		{
			drawCell(row, col, (row + col) % 2 ? g_State.GridColor1 : g_State.GridColor2);
		}
}

void Raster::Draw_Line_grid(int x1,int y1,int x2,int y2, unsigned color)
{
	//�������̸�˼����DDA�㷨��û���õ��Ż������Կ���Bresenham�㷨��
	int Col = (int)((float)(x2 - x1) / g_State.GridWidth + 0.5);
	int Row = (int)((float)(y2 - y1) / g_State.GridWidth + 0.5);

	float dx, dy,i,j;
	dy = (float)Row / fmax(abs(Col), abs(Row));
	dx = (float)Col / fmax(abs(Col), abs(Row));
	if (x2 > x1)
	{
		for (i = 0, j = 0; i <= Col; i = i + dx, j = j + dy)
		{
			int end_i = (int)round(i); 
			int end_j = (int)round(j);

			for (int i = end_j * g_State.GridHeight; i < (end_j + 1) * g_State.GridHeight; ++i)
				for (int j = end_i * g_State.GridWidth; j < (end_i + 1) * g_State.GridWidth; ++j)
				{
					setPixel(x1+j, y1+i, color);
				}
			//drawCell(end_j, end_i,  (end_i + end_j) % 2 ? g_State.GridColor1 : g_State.GridColor2);
		}	
	}
	else
	{
		for (i = 0, j = 0; i >= Col; i = i + dx, j = j + dy)
		{
			int end_i = (int)round(i);
			int end_j = (int)round(j);

			for (int i = end_j * g_State.GridHeight; i < (end_j + 1) * g_State.GridHeight; ++i)
				for (int j = end_i * g_State.GridWidth; j < (end_i + 1) * g_State.GridWidth; ++j)
				{
					setPixel(x1 + j, y1 + i, color);
				}
			//drawCell(end_j, end_i,  (end_i + end_j) % 2 ? g_State.GridColor1 : g_State.GridColor2);
		}
	}
}

//��"����"���ӵĴ�С��ת��
//���ƾ��ε��㷨
void Raster::draw_Rectangle(PixelPoint& point1, PixelPoint& point2, Color color)
{
	double z1, z2;
	if (g_State.VertexDeclare == State::vdXYZ)
	{
		z1 = ((PixelPoint_Z&)point1).z;
		z2 = ((PixelPoint_Z&)point2).z;
	}

	//�¶���4��������������Ļ���
	int x1, y1, x2, y2;
	x1 = point1.x; y1 = point1.y;
	x2 = point2.x; y2 = point2.y;

	//����"����"�߳���С�ı任
	ChangeOf_XYs(&x1, &y1, &x2, &y2);

	//�ж���ʼ�����յ����ʼλ�ã���������ı任
	if (x1 > x2){
		x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
	}
	if (y1 > y2){
		y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
	}
	//����ͼ��
	for (int x = x1; x < x2; ++x)
		for (int y = y1; y < y2; ++y)
		{
			if (g_State.DrawMode == State::dmPixel) //��ģʽ��ѡ�񣬼��ټ���ѭ���Ĵ���
				setPixel(x, y, color);
			else
				drawCell(y, x, color);
		}
}

void Raster::DDA_draw_Line(PixelPoint point1, PixelPoint point2,Color color)
{
	//�������겢���б任
	int x1, y1, x2, y2;
	x1 = point1.x; y1 = point1.y;
	x2 = point2.x; y2 = point2.y;
	ChangeOf_XYs(&x1, &y1, &x2, &y2);

	float x, y;
	float dx, dy;
	dy = (float)((float)y2 - (float)y1) / fmax(abs(x2 - x1), abs(y2 - y1));
	dx = (float)((float)x2 - (float)x1) / fmax(abs(x2 - x1), abs(y2 - y1));

	if (x2 > x1)
		for (x = (float)x1, y = (float)y1; x <= x2; x = x + dx, y = y + dy)
		{
			if (g_State.DrawMode == State::dmPixel)
				setPixel(x, y, color);
			else
				drawCell((int)round(y), (int)round(x), color);
		}	
	else
		for (x = (float)x1, y = (float)y1; x >= x2; x = x + dx, y = y + dy)
		{
			if (g_State.DrawMode == State::dmPixel)
				setPixel(x, y, color);
			else
				drawCell((int)round(y), (int)round(x), color);
		}
}

//#include <Windows.h>
//extern HDC g_hDC;

void Raster::Bresenham_draw_Line(PixelPoint point1, PixelPoint point2, Color color)
{
	/*MoveToEx(g_hDC, point1.x, point1.y, 0 );
	LineTo(g_hDC, point2.x, point2.y );
	return;*/

	//��ȡ���겢��������ת��
	int x1, y1, x2, y2;
	x1 = point1.x; y1 = point1.y;
	x2 = point2.x; y2 = point2.y;
	ChangeOf_XYs(&x1, &y1, &x2, &y2);

	float k, d0, d;
	//�������ȶ���һ��k�����ж�
	k = (float)(y2 - y1) / (x2 - x1);
	int x, y;
	if (k >= 0 && k <= 1) {
		d0 = (float)(0.5 - k);
		if (x1 > x2)//����x1,x2
		{
			x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
		}
		if (y1 > y2)//����y1,y2
		{
			y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
		}
		for (x = x1, y = y1, d = d0; x < x2;)
		{
			if (d >= 0) {
				x++; 
				if (g_State.DrawMode == State::dmPixel)
					setPixel(x, y, color);
				else
					drawCell(y, x, color);
				d = d - k;
			}
			else {
				x++; y++; 
				if (g_State.DrawMode == State::dmPixel)
					setPixel(x, y, color);
				else
					drawCell(y, x, color);
				d = d + 1 - k;
			}
		}
	}
	if (k > 1.0)
	{

		x1 += y1; y1 = x1 - y1; x1 = x1 - y1;
		x2 += y2; y2 = x2 - y2; x2 = x2 - y2;
		k = (float)(y2 - y1) / (x2 - x1);//���¼���k
		d0 = (float)(0.5 - k);
		if (x1 > x2)//����x1,x2
		{
			x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
		}
		if (y1 > y2)//����y1,y2
		{
			y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
		}
		for (x = x1, y = y1, d = d0; x < x2;)
		{
			if (d >= 0) {
				x++; 
				if (g_State.DrawMode == State::dmPixel)
					setPixel(y, x, color);
				else
					drawCell(x, y, g_State.PenColor);
				d = d - k;
			}
			else {
				x++; y++; drawCell(x, y, g_State.PenColor); d = d + 1 - k;
			}
		}
	}
	if (k < 0 && k >= -1)
	{
		y1 = -y1; y2 = -y2;
		k = (float)(y2 - y1) / (x2 - x1);//���¼���k
		d0 = (float)(0.5 - k);
		if (x1 > x2)//����x1,x2
		{
			x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
		}
		if (y1 > y2)//����y1,y2
		{
			y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
		}
		for (x = x1, y = y1, d = d0; x < x2;)
		{
			if (d >= 0) {
				x++;
				if (g_State.DrawMode == State::dmPixel)
					setPixel(x, -y, color);
				else
					drawCell(-y, x, g_State.PenColor);
				d = d - k;
			}
			else {
				x++; y++; drawCell(-y, x, g_State.PenColor); d = d + 1 - k;
			}
		}
	}
	if (k < -1)
	{
		int m = x1, n = x2;
		//��Ҫע���x1����yֵ�󣬺�������x1��ֵ�Ѿ��ı�
		x1 = y1; y1 = -m; x2 = y2; y2 = -n;
		k = (float)(y2 - y1) / (x2 - x1);//���¼���k
		d0 = (float)(0.5 - k);
		if (x1 > x2)//����x1,x2
		{
			x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
		}
		if (y1 > y2)//����y1,y2
		{
			y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
		}
		for (x = x1, y = y1, d = d0; x < x2;)
		{
			if (d >= 0) {
				x++; 
				if (g_State.DrawMode == State::dmPixel)

					setPixel(-y, x, color);
				else
					drawCell(x,-y, g_State.PenColor);
				d = d - k;
			}
			else {
				x++; y++; 
				if (g_State.DrawMode == State::dmPixel)
					setPixel(-y, x, color);
				else
					drawCell(x, -y, g_State.PenColor);
				d = d + 1 - k;
			}
		}
	}

}

void Raster::Bresenham_draw_Polyline(PixelPoint Points[], int count, Color color)
{
	//ChangeOf_XYs(Points,count-1); //��ʼ��ʱ���ý�������ת�����ں���Ļ���ֱ�ߵ�ʱ�����"����"ת��
	//PixelPoint point1, point2;
	for (int i = 0; i < count - 1; i++)//���һ�ε��Ҽ�Ҳ��
	{
		PixelPoint& point1 = Points[i];
		PixelPoint& point2 = Points[i+1];
		Raster::Bresenham_draw_Line(point1, point2, color);
	}
}

void Raster::Bresenham_draw_Circle(PixelPoint point1, PixelPoint point2,Color color)
{
	//����ת��
	int x1, y1, x2, y2;
	x1 = point1.x; y1 = point1.y;
	x2 = point2.x; y2 = point2.y;
	ChangeOf_XYs(&x1, &y1, &x2, &y2);

	int R, x0, y0, x, y;
	double x_star, y_star, x_end, y_end, d0, d;
	x_star = (double)x1; y_star = (double)y1;
	x_end = (double)x2; y_end = (double)y2;
	//���������������⣿
	//setOrig(getWindowWidth()/2-x1, getWindowHeight()/2-y1);
	R = (int)sqrt(pow(x_end - x_star, 2) + pow(y_end - y_star, 2));
	x0 = 0; y0 = R; d0 = 1.25 - R;

	//���ĸ����������ཻ�ĵ��������
	drawCell(y0+y1, x0+x1, g_State.PenColor);
	drawCell(-y0 + y1, x0 + x1, g_State.PenColor);
	drawCell(x0+y1,y0+x1, g_State.PenColor);
	drawCell(x0 + y1, -y0 + x1, g_State.PenColor);

	for (x = x0, y = y0, d = d0; x <= R / 1.414;)  //���ݶԳ��ԣ�Ҳ��ֱ���Ƶ��㷨
	{
		
		if (d >= 0) {
			x = x + 1; y = y - 1;
			drawCell(y + y1, x + x1, g_State.PenColor); drawCell(x + y1, y + x1, g_State.PenColor); drawCell(-x + y1, y + x1, g_State.PenColor); drawCell(-y + y1, x + x1, g_State.PenColor);
			drawCell(-y + y1, -x + x1, g_State.PenColor); drawCell(-x + y1, -y + x1, g_State.PenColor); drawCell(x + y1, -y + x1, g_State.PenColor);	drawCell(y + y1, -x + x1, g_State.PenColor);

			
			d = d + 2.0 * x - 2.0 * y + 5.0;
		}
		if (d < 0)
		{
			x = x + 1; y = y;

			drawCell(y + y1, x + x1, g_State.PenColor); drawCell(x + y1, y + x1, g_State.PenColor); drawCell(-x + y1, y + x1, g_State.PenColor); drawCell(-y + y1, x + x1, g_State.PenColor);
			drawCell(-y + y1, -x + x1, g_State.PenColor); drawCell(-x + y1, -y + x1, g_State.PenColor); drawCell(x + y1, -y + x1, g_State.PenColor);	drawCell(y + y1, -x + x1, g_State.PenColor);

			
			d = d + 2.0 * x + 3.0;
		}
	}
}

void Raster::Bresenham_draw_ellipse(PixelPoint point1, PixelPoint point2, Color color)
{
	
	//����ת��
	int x1, y1, x2, y2;
	x1 = point1.x; y1 = point1.y;
	x2 = point2.x; y2 = point2.y;
	ChangeOf_XYs(&x1, &y1, &x2, &y2);

	drawCell(y1, x1, g_State.PenColor);
	drawCell(2*y2-y1, x1, g_State.PenColor);
	double x_star = 0, y_star, x_end, y_end = 0, a, b, x, y;
	y_star = (double)y1 - y2; x_end = (double)x2 - x1;
	a = x_end, b = y_star;
	double d0, d;
	d0 = b * b + a * a * (0.25 - b);
	double data1 = b * b;
	double data2 = a * a;
	for (x = x_star, y = y_star, d = d0; data1 * (x + 1.0) < data2 * (y - 0.5) && x >= x_star && x < x_end && y >= 0;)
	{
		int m, n;
		if (d < 0) {
			x = x + 1; y = y;
			m = (int)x; n = (int)y;

			drawCell(n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, -m + x1, g_State.PenColor);
			drawCell(n + y2, -m + x1, g_State.PenColor);


			
			d = d + data1 * (2.0 * x + 3.0);
		}
		if (d >= 0) {
			x = x + 1; y = y - 1;
			m = (int)x; n = (int)y;
			drawCell(n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, -m + x1, g_State.PenColor);
			drawCell(n + y2, -m + x1, g_State.PenColor);

			
			d = d + data1 * (2.0 * x + 3.0) + data2 * (2.0 - 2.0 * y);
		}
	}

	d0 = data1 * (x + 0.5) * (x + 0.5) + data2 * (y - 1.0) * (y - 1.0) - data1 * data2;
	double x_star1 = x, y_star1 = y;
	for (x = x_star1, y = y_star1, d = d0; data1 * (x + 1.0) >= data2 * (y - 0.5) && x <= x_end && x > x_star && y > 0;)
	{
		int m, n;
		if (d < 0) {
			x = x + 1; y = y - 1;
			m = (int)x; n = (int)y;

			drawCell(n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, -m + x1, g_State.PenColor);
			drawCell(n + y2, -m + x1, g_State.PenColor);

			
			d = d + 2.0 * data1 * (x + 1.0) + data2 * (3.0 - y);
		}
		if (d >= 0) {
			x = x; y = y - 1;
			m = (int)x; n = (int)y;
			drawCell(n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, m + x1, g_State.PenColor);
			drawCell(-n + y2, -m + x1, g_State.PenColor);
			drawCell(n + y2, -m + x1, g_State.PenColor);

			
			d = d + data2 * (3.0 - 2.0 * y);
		}
	}
}

//���Ա��еĽṹ�嶨��
//���ȶ���һ���ߵĽṹ�壬����洢���������ߵ��¶˵�x���϶˵�y�����о���x�ı仯��dx���Լ�ָ����һ���ߵ�ָ��
typedef struct Edge
{
	float x, Dx;  //�¶˵��x��DxΪ��Ҫ�������������������Ϊ������
	int ymax;
	Edge* next;
}Edge;
#define N 1000
void Raster::ActiveEdge_Polygon(PixelPoint pts[], int Num_point,Color color)
{
	ChangeOf_XYs(pts, Num_point - 1);
	//���ȼ�����ߵ��Y���꣬��͵��Y����
	int maxY = 0, minY = 1000;
	for (int i = 0; i < Num_point; i++)
	{
		if (pts[i].y > maxY)
			maxY = pts[i].y;
		if (pts[i].y < minY)
			minY = pts[i].y;
	}
	int Height_Y = maxY - minY;

	//��ʼ�����������
	Edge* ET[N];
	for (int i = 0; i < maxY; i++)
	{
		ET[i] = (Edge*)malloc(sizeof(Edge));
		//memset(&ET[i], 0, sizeof(Edge));
		if (ET[i])
		{
			ET[i]->next = NULL;
		}
	}
	Edge* AET;
	AET = (Edge*)malloc(sizeof(Edge));
	//memset(&AET, 0, sizeof(Edge));
	if(AET)
		AET->next = NULL;
	//�����ߵı�
	for (int i = 0; i < Num_point; i++)
	{
		//�ҵ��뵱ǰ��1ǰ�����ڵ�4���㣬��1�͵�2��Ϊѭ���ıߣ���0�͵�3��Ϊ�ж����
		int x0 = pts[(i - 1 + Num_point) % Num_point].x;
		int x1 = pts[i].x;
		int x2 = pts[(i + 1) % Num_point].x;
		int x3 = pts[(i + 2) % Num_point].x;
		int y0 = pts[(i - 1 + Num_point) % Num_point].y;
		int y1 = pts[i].y;
		int y2 = pts[(i + 1) % Num_point].y;
		int y3 = pts[(i + 2) % Num_point].y;
		//�����ˮƽ�ߣ�Ҳ����y1��y2����ȵģ���ôֱ������
		if (y1 == y2)
			continue;
		//�ֱ������ߵ����¶˵��Y���꣬�¶˵��X�����б�ʵĵ���
		//���㵹����Ҫ�Ǻ���ɨ��ʱ����X�����ϵ���������Ҫ��
		int ymin = y1 > y2 ? y2 : y1;
		int ymax = y1 > y2 ? y1 : y2;
		float x = y1 > y2 ? x2 : x1;
		float dx = (x1 - x2) * 1.0f / (y1 - y2);
		//��㴦����������2->1->0�𽥵�Y�����𽥼�С����ôy1����㣬���1->2->3�𽥼�С����ô��2�����
		if ((y1<y2 && y1>y0) || (y2<y1 && y2>y3))
		{
			ymin++;
			x += dx;
		}
		//�����±ߣ�����ߵı�ET
		Edge* p = (Edge*)malloc(sizeof(Edge));
		p->ymax = ymax;
		p->x = x;
		p->Dx = dx;
		p->next = ET[ymin]->next;
		ET[ymin]->next = p;
	}
	//ɨ����ÿ������ɨ�裬Y������ÿ������һ��
	//ǰ�������Ѿ��ҵ�����С��Y��ֵ�����Ծʹ���С��Y��ֵ��ʼ����ɨ��
	for (int i = 0; i < maxY; i++)
	{
		//ȡ��ET�е�ǰɨ���е����б߲�����X�ĵ���˳�����X��ȣ��Ͱ���DX����
		while (ET[i]->next)
		{
			//ȡ��ET�е�ǰɨ��
			Edge* pinsert = ET[i]->next;
			Edge* p = AET;
			//��AET���������ʵĲ���λ��?������AET���������ʵĲ���λ��
			while (p->next)
			{
				if (pinsert->x > p->next->x)
				{
					p = p->next;
					continue;
				}
				if (pinsert->x == p->next->x && pinsert->Dx > p->next->Dx)
				{
					p = p->next;
					continue;
				}
				//�ҵ�λ��
				break;
			}
			//��pinsert��ET��ɾ����������AET�еĵ�ǰ��λ��
			ET[i]->next = pinsert->next;
			pinsert->next = p->next;
			p->next = pinsert;
		}
		//Ȼ��AET�еı�������������Բ��������ɫ
		Edge* p_aet = AET;
		while (p_aet->next && p_aet->next->next)
		{
			for (int x = p_aet->next->x; x < p_aet->next->next->x; x++)
			{
				if(g_State.DrawMode == State::dmPixel)
					setPixel(x, i, color);
				//setPixel(x, i, BLACK);
				//ת��Ϊ�ø���ģʽ�»��ƣ�ͨ�����ӵı߳��Ĵ�С������Ҫ����ͼ�Ĵ�С
				else
					drawCell(i, x, color);
			}
			p_aet = p_aet->next->next;
		}
		//ɾ��AET
		p_aet = AET;
		while (p_aet->next)
		{
			if (p_aet->next->ymax == i)
			{
				Edge* pdelete = p_aet->next;
				p_aet->next = pdelete->next;
				pdelete->next = NULL;
				free(pdelete);
			}
			else {
				p_aet = p_aet->next;
			}
		}

		//����AET�бߵ�Xֵ��������һѭ��
		p_aet = AET;
		while (p_aet->next)
		{
			p_aet->next->x += p_aet->next->Dx;
			p_aet = p_aet->next;
		}
	}
}

/*���������������б任 */
void Raster::ChangeOf_XYs(int *x1, int *y1, int *x2, int *y2)
{
	*x1 /= g_State.GridWidth; *y1 /= g_State.GridHeight;
	*x2 /= g_State.GridWidth; *y2 /= g_State.GridHeight;

}

/*����Ϊ�Ե㼯����������б任�ĺ���*/
void Raster::ChangeOf_XYs(PixelPoint *Points,int Count)
{
	for (int i = 0; i <= Count; i++)
	{
		Points[i].x /= g_State.GridWidth;
		Points[i].y /= g_State.GridHeight;
	}	
}
/*���ҵ㼯������xֵ����Ϊ��Ե������������*/
int Raster::FindMax_x(PixelPoint Points[],int Count)
{
	int Max = Points[0].x;
	for (int i = 1; i <= Count; i++)
	{
		if (Points[i].x >= Max)
			Max = Points[i].x;
	}
	return Max;
}

/*��Ե�����㷨*/
void Raster::Edge_Fill(PixelPoint Points[], int Count,Color color)
{
	//����������x�������ĵ�
	int max_x = FindMax_x(Points, Count);
	//��������б任
	ChangeOf_XYs(Points, Count);
	//���б�Ե���
	//���ȵ�һ����Ҫ�ҵ�������֮������ϵ����ϵĵ㣬������յ���DDA�㷨�ҵ����ϵĵ�
	for (int i = 0; i < Count; i++)
	{
		int x1, y1, x2, y2;
		x1 = Points[i].x; y1 = Points[i].y;
		x2 = Points[i + 1].x; y2 = Points[i + 1].y;
		//��������
		if (y1 > y2)
		{
			y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
			x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
		}
		Boundary_SigleEdge(x1, y1, x2, y2, max_x,color);	
	}
}

void Raster::Boundary_SigleEdge(int x1, int y1, int x2, int y2,int max_x,Color color)
{
	//����pq��ΪDDA���ߵĵ�
	int p,q;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int k;
	float x = x1;
	float y = y1;
	float xIncre, yIncre;
	//ע�������DDA�㷨��ͬ��yIncre=1
	//if(abs(dx)>abs(dy))
	//eps1=abs(dx);
	//else
	int eps1 = abs(dy);
	xIncre = (float)dx / (float)eps1;
	yIncre = (float)dy / (float)eps1;
	int flag = 1;
	for (k = 0; k <= eps1; k++)
	{
		//��֪��Ϊʲô����+0.5�ķ�ʽ�ᷢ������  WHY
		/*p = (int)(x + 0.5);
		q = (int)(y + 0.5);*/
		p = (int)round(x);
		q = (int)round(y);
		//ÿ��ֱ�ߵ��϶˵㲻�������⽻�㴦�������Σ�û�ﵽ����Ч��
		if (k != 0)
			DDA(p, q, max_x, q,color);
		
		x += xIncre;
		y += yIncre;
	}
}

bool Raster::IsWhite(int y, int x)
{
	int index = 0;
	for (int i = y * g_State.GridHeight; i < (y + 1) * g_State.GridHeight; ++i)
		for (int j = x * g_State.GridWidth; j < (x + 1) * g_State.GridWidth; ++j)
		{
			if (getPixel(j, i) == WHITE)
				index++;
		}
	if (index == g_State.GridHeight * g_State.GridWidth)
		return true;
	else
		return false;
}

void Raster::Fence_Fill(PixelPoint Points[], int Count, Color color)
{
	
	//��������б任
	ChangeOf_XYs(Points, Count);

	//�ҵ�һ����ע���Xֵ��Ϊդ��
	int index_x = Points[0].x;

	//Draw_Polygon(Points, Count);
	//�����߶������˵������
	int x1, y1, x2, y2;
	int maxy=Points[0].y, miny= Points[0].y;
	for (int i = 0; i < Count; i++)
	{

		x1 = Points[i].x; y1 = Points[i].y;
		x2 = Points[i + 1].x; y2 = Points[i + 1].y;
		//�ж��������ڵ��yֵ�Ĵ�С�����ǰһ�����Yֵ�����򽻻������������
		if (y1 > y2)
		{
			y1 += y2; y2 = y1 - y2; y1 = y1 - y2;
			x1 += x2; x2 = x1 - x2; x1 = x1 - x2;
		}
		//��ÿһ���߽���ɨ�����
		Fence_SingleEdge(x1, y1, x2, y2, index_x, color);

		//����һ��Y�ķ�Χ
		if (Points[i + 1].y > maxy)
			maxy = Points[i + 1].y;
		if(Points[i + 1].y < miny)
			miny = Points[i + 1].y;

	}
	//�Ա߽��ٽ���һ�δ������ε����������������
	DrawDDA_Polygon(Points, Count,color);

	//�������դ������Ҫ�������ж������ǹؼ�
	for (int y = miny; y < maxy; y++)
	{
		if(IsWhite(y, index_x)&& (!IsWhite(y, index_x - 1)) && (!IsWhite(y, index_x + 1)))
			drawCell(y, index_x, color);
	}
}

void Raster::Fence_SingleEdge(int x1, int y1, int x2, int y2, int index_x, Color color)
{
	int p, q;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int k;
	float x = x1;
	float y = y1;
	float xIncre, yIncre;
	//ע�������DDA�㷨��ͬ��yIncre=1
	//if(abs(dx)>abs(dy))
	//eps1=abs(dx);
	//else
	int eps1 = abs(dy);
	xIncre = (float)dx / (float)eps1;
	yIncre = (float)dy / (float)eps1;
	int flag = 1;
	for (k = 0; k <= eps1; k++)
	{
		//��֪��Ϊʲô����+0.5�ķ�ʽ�ᷢ������  WHY
		/*p = (int)(x + 0.5);
		q = (int)(y + 0.5);*/
		p = (int)round(x);
		q = (int)round(y);
		//ÿ��ֱ�ߵ��϶˵㲻�������⽻�㴦�������Σ�û�ﵽ����Ч��
		if (k != 0)
		{
			DDA(p, q, index_x, q,color);
		}
			

		x += xIncre;
		y += yIncre;
	}
}
void Raster::DrawDDA_Polygon(PixelPoint Points[],int Count,Color color)
{
	ChangeOf_XYs(Points, Count);
	for (int i = 0; i < Count; i++)//���һ�ε��Ҽ�Ҳ��
	{
		int x1 = Points[i].x, y1 = Points[i].y;
		int x2 = Points[i + 1].x, y2 = Points[i + 1].y;

		DDA_draw_Line(Points[i], Points[i+1],color);
	}
	//Bresenham_draw_Line(Points[end].x, Points[end].y, Points[0].x, Points[0].y);
}

void Raster::DDA(int x1, int y1, int x2, int y2,Color color)
{
	/*x1 /= g_State.GridWidth;  x2 /= g_State.GridWidth;
	y1 /= g_State.GridHeight; y2 /= g_State.GridHeight;*/
	float x, y;
	float dx, dy;
	dy = (float)(y2 - y1) / fmax(abs(x2 - x1), abs(y2 - y1));
	dx = (float)(x2 - x1) / fmax(abs(x2 - x1), abs(y2 - y1));

	if (x2 > x1)
		for (x = (float)x1, y = (float)y1; x <= x2; x = x + dx, y = y + dy)
		{
			if (IsWhite((int)round(y), (int)round(x)))
			{
				drawCell((int)round(y), (int)round(x), color);
			}
			else
			{
				drawCell((int)round(y), (int)round(x), WHITE);
			}

		}
	else
		for (x = (float)x1, y = (float)y1; x >= x2; x = x + dx, y = y + dy)
		{
			if (IsWhite((int)round(y), (int)round(x)))
			{
				drawCell((int)round(y), (int)round(x), g_State.PenColor);
			}
			else
			{
				drawCell((int)round(y), (int)round(x), WHITE);
			}
		}
}

void Raster::Edge_TagMethod(PixelPoint Points[], int Count, Color color)
{
	ChangeOf_XYs(Points,Count);
	//���Ʊ߽�
	DrawDDA_Polygon(Points, Count,color); //�޸� κ������������ɫѡ��
	/*���б߱�־��*/
	//bool flag = false;
	int xmax=Points[0].x, xmin = Points[0].x, ymax = Points[0].y, ymin = Points[0].y; //�ҵ�X��Y�������Сֵ
	//Color color1, color2;//ǰ��Ϊ�߽�ɫ������Ϊ����ɫ
	//������е��е�X��Y�ķֱ��������С��ֵ
	for (int i = 0; i < Count; i++)
	{
		if (Points[i].x > xmax)
			xmax = Points[i].x;
		if (Points[i].x < xmin)
			xmin = Points[i].x;
	}
	for (int i = 0; i < Count; i++)
	{
		if (Points[i].y > ymax)
			ymax = Points[i].y;
		if (Points[i].y < ymin)
			ymin = Points[i].y;
	}

	for (int i = ymin+1; i < ymax; i++)
	{
		bool flag = false;
		int Black_x=0;
		for (int j = xmin; j <= xmax; j++)
		{
			if (!IsWhite(i, j) && (IsWhite(i, j + 1)))
			{
				flag = !flag;
			}
			if (flag)
				drawCell(i, j, color);			
		}

	}	
}

void Raster::Boundary_Fill(int x, int y, Color BoundaryColor,Color FillColor)
{
	
	//ͨ���ݹ���õķ�ʽ���У����ַ�����ռ�ýϴ��ջ�ռ�
	Color color;
	color = getPixel(x, y);
	if (color!=BoundaryColor&& color != FillColor)
	{
		//drawCell(y, x, fill_color );
		setPixel(x, y, FillColor);
		Boundary_Fill(x + 1, y, BoundaryColor, FillColor);

		Boundary_Fill(x, y + 1, BoundaryColor, FillColor);

		Boundary_Fill(x - 1, y, BoundaryColor, FillColor);

		Boundary_Fill(x, y - 1, BoundaryColor, FillColor);
	}
			
}

void Raster::PushSeed(PointsStack* Points, PixelPoint SeedPoint)
{
	PointsStack* NewPoint;
	NewPoint = (PointsStack*)malloc(sizeof(PointsStack));
	if (NewPoint == NULL)
		return;
	NewPoint->SeedPoint = SeedPoint;
	NewPoint->next = NULL;
	NewPoint->next = Points->next;
	Points->next = NewPoint;
}

int Raster::PopSeed(PointsStack* Points, PixelPoint& SeedPoint)
{
	PointsStack* Point;
	if (Points->next == NULL)
		return 0;
	else
	{
		Point = Points->next;
		SeedPoint = Point->SeedPoint;
		Points->next = Point->next;
		free(Point);

	}
	return 1;	
}

void Raster::SeedBoundaryFill4(PixelPoint SeedPoint, Color OldColor, Color FillColor)
{
	int x, y;
	PointsStack *Points = (PointsStack*)malloc(sizeof(PointsStack));
	if(Points)
		Points->next = NULL;
	PushSeed(Points, SeedPoint);
	//Raster::PopSeed(Points, SeedPoint);
	while(Points->next)
	{
		//��ջ
		PopSeed(Points, SeedPoint);
		//�����ӵ�������ɫ
		x = SeedPoint.x;
		y = SeedPoint.y;
		setPixel(x, y, FillColor);
		if (getPixel(x - 1, y) != OldColor && getPixel(x - 1, y) != FillColor)
		{
			SeedPoint.x = x - 1;
			SeedPoint.y = y;
			PushSeed(Points, SeedPoint);
		}
		if (getPixel(x + 1, y) != OldColor && getPixel(x + 1, y) != FillColor)
		{
			SeedPoint.x = x + 1;
			SeedPoint.y = y;
			PushSeed(Points, SeedPoint);
		}
		if (getPixel(x, y+1) != OldColor && getPixel(x, y+1) != FillColor)
		{
			SeedPoint.x = x;
			SeedPoint.y = y+1;
			PushSeed(Points, SeedPoint);
		}
		if (getPixel(x, y - 1) != OldColor && getPixel(x, y - 1) != FillColor)
		{
			SeedPoint.x = x;
			SeedPoint.y = y - 1;
			PushSeed(Points, SeedPoint);
		}

	}

}

void Raster::SeedBoundaryFill4_Line(PixelPoint SeedPoint, Color OldColor, Color FillColor)
{
	
	PointsStack* Points = (PointsStack*)malloc(sizeof(PointsStack));
	if (Points)
		Points->next = NULL;
	Raster::PushSeed(Points, SeedPoint);
	while (Points->next)
	{
		int x, y;
		PopSeed(Points, SeedPoint);
		//�����ӵ�������ɫ
		x = SeedPoint.x;
		y = SeedPoint.y;

		setPixel(x, y, FillColor);
		int xToleft, xToright;
		for (xToleft = x - 1; getPixel(xToleft, y) != OldColor; xToleft--)
		{
			setPixel(xToleft, y, FillColor);
		}
		for (xToright = x + 1; getPixel(xToright, y) != OldColor; xToright++)
		{
			setPixel(xToright, y, FillColor);
		}
		//�ҵ�ɨ���������ȡֵ��Χ
		xToleft += 1;
		xToright -= 1;
		x = xToright;//������ϴε��ұߵĶ˵�
		y=y+1;//��ʼ������һ��ɨ����
		while (x >= xToleft)
		{
			bool flag = false;//�ȶ��岻���
			while (x >= xToleft && (getPixel(x, y) != OldColor))
			{
				flag = true;
				x--;
			}
			if (flag)
			{
				PixelPoint newPoint;
				newPoint.x = x + 1;
				newPoint.y = y;
				PushSeed(Points, newPoint);
				flag = false;
			}
			//����Ҫ���������飬��ֹ����©���������䷶Χ
			while (x >= xToleft && (getPixel(x, y) == OldColor))
			{
				//flag = true;
				x--;
			}
		}
		//����Ŀ���ߵ���һ���߽��д���
		//x = xToright;
		//y = y - 2;
		//while (x >= xToleft)
		//{
		//	bool flag = false;//�ȶ��岻���
		//	while (x >= xToleft && (getPixel(x, y) != OldColor))
		//	{
		//		flag = true;
		//		x--;
		//	}
		//	if (flag)
		//	{
		//		PixelPoint newPoint;
		//		newPoint.x = x + 1;
		//		newPoint.y = y;
		//		PushSeed(Points, newPoint);
		//		flag = false;
		//	}
		//	//����Ҫ���������飬��ֹ����©���������䷶Χ
		//	while (x >= xToleft && (getPixel(x, y) == OldColor))
		//	{
		//		//flag = true;
		//		x--;
		//	}
		//}
	}
}

void Raster::FloodFill4(PixelPoint SeedPoint, Color OldColor, Color FillColor)
{
	int x, y;
	PointsStack* Points = (PointsStack*)malloc(sizeof(PointsStack));
	if (Points)
		Points->next = NULL;
	Raster::PushSeed(Points, SeedPoint);
	//Raster::PopSeed(Points, SeedPoint);
	while (Points->next)
	{
		//��ջ
		PopSeed(Points, SeedPoint);
		//�����ӵ�������ɫ
		x = SeedPoint.x;
		y = SeedPoint.y;
		setPixel(x, y, FillColor);
		if (getPixel(x - 1, y) == OldColor && getPixel(x - 1, y) != FillColor)
		{
			SeedPoint.x = x - 1;
			SeedPoint.y = y;
			PushSeed(Points, SeedPoint);
		}
		if (getPixel(x + 1, y) == OldColor && getPixel(x + 1, y) != FillColor)
		{
			SeedPoint.x = x + 1;
			SeedPoint.y = y;
			PushSeed(Points, SeedPoint);
		}
		if (getPixel(x, y + 1) == OldColor && getPixel(x, y + 1) != FillColor)
		{
			SeedPoint.x = x;
			SeedPoint.y = y + 1;
			PushSeed(Points, SeedPoint);
		}
		if (getPixel(x, y - 1) == OldColor && getPixel(x, y - 1) != FillColor)
		{
			SeedPoint.x = x;
			SeedPoint.y = y - 1;
			PushSeed(Points, SeedPoint);
		}
	}
}


struct Codepoint
{
	Codepoint(){}
	Codepoint(PixelPoint point)
	{
		this->x = point.x;
		this->y = point.y;
	}
	int x, y;
	char code[4];
};

Codepoint setCode(Codepoint point, int minx, int maxx, int miny, int maxy)
{
	Codepoint ptemp;
	if (point.y < miny)
		ptemp.code[0] = '1';
	else
		ptemp.code[0] = '0';
	if (point.y > maxy)
		ptemp.code[1] = '1';
	else
		ptemp.code[1] = '0';
	if (point.x > maxx)
		ptemp.code[2] = '1';
	else
		ptemp.code[2] = '0';
	if (point.x < minx)
		ptemp.code[3] = '1';
	else
		ptemp.code[3] = '0';
	ptemp.x = point.x;
	ptemp.y = point.y;
	return ptemp;
}

int visibility(Codepoint pt1, Codepoint pt2)
{
	int i, flag = 0;
	for (i = 0; i < 4; ++i)
	{
		if ((pt1.code[i] != '0') || (pt2.code[i] != '0'))
			flag = 1;
	}
	if (flag == 0)
		return 0;
	for (i = 0; i < 4; ++i)
	{
		if ((pt1.code[i] == pt2.code[i]) && pt1.code[i] == '1')
			flag = 0;
	}
	if (flag == 0)
		return 1;
	return 2;
}

PixelPoint resetendpt(Codepoint p1, Codepoint p2, int minx, int maxx, int miny, int maxy)
{
	Codepoint temp;
	PixelPoint point;
	int x, y;
	float m, k;
	if (p1.code[3] == '1')
		x = minx;
	if (p1.code[2] == '1')
		x = maxx;
	if ((p1.code[3] == '1') || (p1.code[2] == '1'))
	{
		m = (float)(p2.y - p1.y) / (p2.x - p1.x);
		k = (float)p1.y + (m * (x - p1.x));
		temp.y = k;
		temp.x = x;
		for (int i = 0; i < 4; ++i)
			temp.code[i] = p1.code[i];
		if (temp.y <= maxy && temp.y >= miny)
		{
			point.x = temp.x;
			point.y = temp.y;
			return point;
		}
	}
	if (p1.code[0] == '1')
		y = miny;
	if (p1.code[1] == '1')
		y = maxy;
	if (p1.code[0] == '1' || p1.code[1] == '1')
	{
		m = (float)(p2.y - p1.y) / (p2.x - p1.x);
		k = (float)p1.x +  (y - p1.y)/m;
		temp.x = k;
		temp.y = y;
		for (int i = 0; i < 4; ++i)
		{
			temp.code[i] = p1.code[i];
		}
		point.x = temp.x;
		point.y = temp.y;
		return point;
	}
	else
	{
		point.x = p1.x;
		point.y = p1.y;
		return point;
	}
}
void Raster::Cohen_Sutherland(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& container)
{
	//���ȶԲü���ķ�Χ����ȷ�������մ�С���н���
	int minx = point1.x < point2.x ? point1.x : point2.x;
	int maxx = point1.x > point2.x ? point1.x : point2.x;
	int miny = point1.y < point2.y ? point1.y : point2.y;
	int maxy = point1.y > point2.y ? point1.y : point2.y;   
	//����ѭ����ȷ��ÿһ���ߣ������б���
	for (int i = 0; i < container.size(); ++i)
	{
		StyledGeometry* pStyledGeometry = container[i];
		ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
		if (pStyle->drawMethod == otDrawLineDDA || pStyle->drawMethod == otDrawLine_Bresenham)
		{
			PixelPoint Lpt1, Lpt2;
			PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pStyledGeometry->pGeometry;
			const vector<Point2D>& pts = pPolylineGeometry->getPts();
			Lpt1.x = pts[0].x; Lpt1.y = pts[0].y;
			Lpt2.x = pts[1].x; Lpt2.y = pts[1].y; //�õ��ߵ������˵�����
			container.erase(container.begin() + i);
			PixelPoint point3, point4;
			Codepoint pt1 = setCode(Codepoint(Lpt1), minx, maxx, miny, maxy); //��pt1,pt2���б���
			Codepoint pt2 = setCode(Codepoint(Lpt2), minx, maxx, miny, maxy);
			int v = visibility(pt1, pt2);
			PolylineGeometry* nPolylineGeometry = new PolylineGeometry();
			ColorStyle* npStyle;
			switch (pStyle->drawMethod)
			{
			case otDrawLineDDA:
				npStyle = new ColorStyle(getPenColor(), otDrawLineDDA);
				break;
			case otDrawLine_Bresenham:
				npStyle = new ColorStyle(getPenColor(), otDrawLine_Bresenham);
				break;
			default:
				break;
			}
			switch (v)
			{
			case 0:			
				nPolylineGeometry->addPoint(Lpt1.x, Lpt1.y);
				nPolylineGeometry->addPoint(Lpt2.x, Lpt2.y);				
				break;
			case 1:
				break;
			case 2:
				point3 = resetendpt(pt1, pt2, minx, maxx, miny, maxy);
				point4 = resetendpt(pt2, pt1, minx, maxx, miny, maxy);				
				nPolylineGeometry->addPoint(point3.x, point3.y);
				nPolylineGeometry->addPoint(point4.x, point4.y);				
				break;
			}
			StyledGeometry* pStyledGeometry = new StyledGeometry(nPolylineGeometry, npStyle);
			container.push_back(pStyledGeometry);
		}
	}
 }

bool judgePointInnerOrOuter(PixelPoint p, PixelPoint m_window[2])
{
	/*�жϵ��ǲ����ھ��ε��ڲ��������ⲿ��������ⲿ������Ϊfalse����֮Ϊtrue*/
	int minx, miny, maxx, maxy;
	minx = m_window[0].x > m_window[1].x ? m_window[1].x : m_window[0].x;
	maxx = m_window[0].x <= m_window[1].x ? m_window[1].x : m_window[0].x;
	miny = m_window[0].y > m_window[1].y ? m_window[1].y : m_window[0].y;
	maxy = m_window[0].y <= m_window[1].y ? m_window[1].y : m_window[0].y;
	if (p.x < minx || p.x>maxx || p.y<miny || p.y>maxy)
		return false;
	else
		return true;
}

//�۰���ҽ���
PixelPoint bSearch(PixelPoint out, PixelPoint inn, PixelPoint m_window[2])
{
	PixelPoint mid; //�����е�����
	bool code;
	int d1, d2;
	d1 = inn.x - out.x;
	d2 = inn.y - out.y;
	mid.x = out.x + d1 / 2;
	mid.y = out.y + d2 / 2;
	while (abs(out.x - inn.x) > 1 || abs(out.y - inn.y) > 1)
	{
		code = judgePointInnerOrOuter(mid, m_window);
		if (code)
			inn = mid;
		else
			out = mid;
		d1 = inn.x - out.x;
		d2 = inn.y - out.y;
		mid.x = out.x + d1 / 2;
		mid.y = out.y + d2 / 2;

	}
	return inn;
}
//�������ж������߶ε�λ�ù�ϵ
bool judgeLineToLine(PixelPoint a, PixelPoint b, PixelPoint c, PixelPoint d)
{
	long int d1, d2, d3, d4;
	d1 = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	d2 = (b.x - a.x) * (d.y - a.y) - (b.y - a.y) * (d.x - a.x);
	d3 = (d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x);
	d4 = (d.x - c.x) * (b.y - c.y) - (d.y - c.y) * (b.x - c.x);
	if (d1 * d2 <= 0 && d3 * d4 <= 0)
		return true;//�������ཻ�����غϵ�
	else
		return false;//���뽻
}

PixelPoint getPoint(PixelPoint out1, PixelPoint out2, PixelPoint point1, PixelPoint point11)
{
	int a = out2.x - out1.x;
	int b = point1.x - point11.x;
	int c = out2.y - out1.y;
	int d = point1.y - point11.y;
	int g = point1.x - out1.x;
	int h = point1.y - out1.y;
	int f = a * d - b * c;
	double t = double((d * g - b * h)) / double(f);
	PixelPoint m;
	m.x = out1.x + t * (out2.x - out1.x);
	m.y = out1.y + t * (out2.y - out1.y);
	return m;
}
//�е�ָ��㷨
void Raster::midpointSubdivisionAlgorithm(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& container)
{
	PixelPoint m_window[2];
	//���ȶԲü���ķ�Χ����ȷ�������մ�С���н���
	m_window[0].x = point1.x < point2.x ? point1.x : point2.x;
	m_window[1].x = point1.x > point2.x ? point1.x : point2.x;
	m_window[0].y = point1.y < point2.y ? point1.y : point2.y;
	m_window[1].y = point1.y > point2.y ? point1.y : point2.y;
	//����ѭ����ȷ��ÿһ���ߣ������б���
	for (int i = 0; i < container.size(); ++i)
	{
		StyledGeometry* pStyledGeometry = container[i];
		ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
		if (pStyle->drawMethod == otDrawLineDDA || pStyle->drawMethod == otDrawLine_Bresenham)
		{
			PixelPoint Lpt1, Lpt2;
			PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pStyledGeometry->pGeometry;
			const vector<Point2D>& pts = pPolylineGeometry->getPts();
			Lpt1.x = pts[0].x; Lpt1.y = pts[0].y;
			Lpt2.x = pts[1].x; Lpt2.y = pts[1].y; //�õ��ߵ������˵�����
			container.erase(container.begin() + i);
			bool code1, code2, code3;
			code1 = judgePointInnerOrOuter(Lpt1, m_window);
			code2 = judgePointInnerOrOuter(Lpt2, m_window);
			PixelPoint out1, out2, inn1, inn2, mid;
			int d1, d2;
			if (code1 && code2)
			{
				/*  �߶ε������˵㶼�ھ��ο��ڲ����Ǿͷ����������㣬�������κα任*/
				inn1 = Lpt1; inn2 = Lpt2;
			}
			if ((code1 == true && code2 == false) || (code1 == false && code2 == true))
			{
				/*  һ���˵��ھ��ε��ڲ�����һ���˵��ھ��ε��ⲿ*/
				if (code1)
				{
					out1 = Lpt2;
					inn2 = inn1 = Lpt1;
				}
				else
				{
					out1 = Lpt1;
					inn2 = inn1 = Lpt2;
				}
				inn1 = bSearch(out1, inn1, m_window);
			}
			if (code1 == false && code2 == false)
			{
				//���������Ϊ���ӣ���Ϊ�ߵ������˵�ͬʱ�ھ�������������Ƿǳ����
				//���ʱ����ʹ�þ��η�Χ�ĶԽ��߽��д���
				out1 = Lpt1; out2 = Lpt2;
				PixelPoint point1, point11, point2, point22;//���ڱ��������Խ���
				point1 = m_window[0];
				point11 = m_window[1];
				point2.x = m_window[0].x; point2.y = m_window[1].y;
				point22.x = m_window[1].x; point22.y = m_window[0].y;
				//�ж��߶��Ƿ���Խ����н���
				bool code4 = judgeLineToLine(out1, out2, point1, point11);
				bool code5 = judgeLineToLine(out1, out2, point2, point22);
				if (code4 || code5)
				{
					//��������߶���Խ��ߵĽ���r
					PixelPoint r;
					if (code4)
						r = getPoint(out1, out2, point1, point11);
					else
						r = getPoint(out1, out2, point2, point22);
					inn1 = inn2 = r;
					inn1 = bSearch(out1, inn1, m_window);
					inn2 = bSearch(out2, inn2, m_window);

				}
				else
				{
					//�������������ԭ�����߶����߿�û�н��㣬���Խ�inn������Ϊ��
					inn1.x = inn1.y = inn2.x = inn2.y = 999;
				}
			}
			//get the result
			if (inn1.x != 999 && inn2.x != 999)
			{
				PolylineGeometry* nPolylineGeometry = new PolylineGeometry();
				ColorStyle* npStyle;
				switch (pStyle->drawMethod)
				{
				case otDrawLineDDA:
					npStyle = new ColorStyle(getPenColor(), otDrawLineDDA);
					break;
				case otDrawLine_Bresenham:
					npStyle = new ColorStyle(getPenColor(), otDrawLine_Bresenham);
					break;
				default:
					break;
				}
				nPolylineGeometry->addPoint(inn1.x, inn1.y);
				nPolylineGeometry->addPoint(inn2.x, inn2.y);
				StyledGeometry* pStyledGeometry = new StyledGeometry(nPolylineGeometry, npStyle);
				container.push_back(pStyledGeometry);
			}
		}
	}
}

//Liang-Barsky�㷨
int LBLineClipTest(float p, float q, float& umax, float& umin)
{
	float r = 0.0;
	if (p < 0.0)
	{                        //pС��0ʱ�Ƚ����ֵ
		r = q / p; 
		if (r > umin)
			return 0;      //umax<uminʱֱ�߶β����ڴ����ڵĲ���
		else if (r > umax)
			umax = r;
	}
	else if (p > 0.0)
	{                    //p>0ʱ��Ƚ���Сֵ
		r = q / p;
		if (r < umax)
			return 0;           //umax<uminʱֱ�߶β����ڴ����ڵĲ���
		else if (r < umin)
			umin = r;

	}
	else if (q < 0.0)     //����p=0ʱ������
		return 0;
	return 1;
}

void Raster::LBLine(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& container)
{
	float xwl, xwr, ywb, ywt;
	xwl = point1.x < point2.x ? point1.x : point2.x;
	xwr = point1.x > point2.x ? point1.x : point2.x;
	ywb = point1.y < point2.y ? point1.y : point2.y;
	ywt = point1.y > point2.y ? point1.y : point2.y;
	for (int i = 0; i < container.size(); ++i)
	{
		StyledGeometry* pStyledGeometry = container[i];
		ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
		if (pStyle->drawMethod == otDrawLineDDA || pStyle->drawMethod == otDrawLine_Bresenham)
		{
			PixelPoint Lpt1, Lpt2;
			PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pStyledGeometry->pGeometry;
			const vector<Point2D>& pts = pPolylineGeometry->getPts();
			Lpt1.x = pts[0].x; Lpt1.y = pts[0].y;
			Lpt2.x = pts[1].x; Lpt2.y = pts[1].y; //�õ��ߵ������˵�����
			container.erase(container.begin() + i);
			float umax, umin, deltax, deltay;
			deltax = Lpt2.x - Lpt1.x;
			deltay = Lpt2.y - Lpt1.y;
			umax = 0.0; umin = 1.0;
			if (LBLineClipTest(-deltax, Lpt1.x - xwl, umax, umin))
				if (LBLineClipTest(deltax, xwr - Lpt1.x, umax, umin))
					if (LBLineClipTest(-deltay, Lpt1.y - ywb, umax, umin))
						if (LBLineClipTest(deltay, ywt - Lpt1.y, umax, umin))
						{
							Lpt1.x = int(Lpt1.x + umax * deltax + 0.5);
							Lpt1.y = int(Lpt1.y + umax * deltay + 0.5);
							Lpt2.x = int(Lpt1.x + umin * deltax + 0.5);
							Lpt2.y = int(Lpt1.y + umin * deltay + 0.5);
							PolylineGeometry* nPolylineGeometry = new PolylineGeometry();
							ColorStyle* npStyle;
							switch (pStyle->drawMethod)
							{
							case otDrawLineDDA:
								npStyle = new ColorStyle(getPenColor(), otDrawLineDDA);
								break;
							case otDrawLine_Bresenham:
								npStyle = new ColorStyle(getPenColor(), otDrawLine_Bresenham);
								break;
							default:
								break;
							}							
							nPolylineGeometry->addPoint(Lpt1.x, Lpt1.y);
							nPolylineGeometry->addPoint(Lpt2.x, Lpt2.y);
							StyledGeometry* npStyledGeometry = new StyledGeometry(nPolylineGeometry, npStyle);
							container.push_back(npStyledGeometry);
						}
		}
	}	
}


//����εĲü�
//����һ���µĵ����
struct point
{
public:
	double x, y;
	point()
	{
		this->x = this->y = 0;
	}
	point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	void setX(double x)
	{
		this->x = x;
	}
	void setY(double y)
	{
		this->y = y;
	}
	double getX()
	{
		return this->x;
	}
	double getY()
	{
		return this->y;
	}
	
};
//�ٶ���һ����������
struct VectorLine:point
{
	VectorLine(point start, point end)
	{
		this->start = start;
		this->end = end;
	}
	point start;   //��ʼ����
	point end; //�ս��
};

double Multi(point p0, point p1, point p2)
{
	//���һ�����Ƿ���һ���ߵ��ڲ⣬���ʱ��ĵ�Ӧ����Ϊ����ģ�����������ϵģ���ôҲ������
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool isInside( point& p, VectorLine& v)
{
	//ͨ���������ⷽ���Ƿ�Ϊ��������Ϊ����
	return Multi(p, v.start, v.end) >= 0 ? true : false;
}

point Intersection(point start0, point end0, point start1, point end1)
{
	//ʹ�����Ҷ�������Ƶ�
	if (Multi(start0, end1, start1) - Multi(end0, end1, start1) != 0)
	{
		double pX = (Multi(start0, end1, start1) * end0.x - Multi(end0, end1, start1) * start0.x) /
			(Multi(start0, end1, start1) - Multi(end0, end1, start1));
		double pY = (Multi(start0, end1, start1) * end0.y - Multi(end0, end1, start1) * start0.y) /
			(Multi(start0, end1, start1) - Multi(end0, end1, start1));
		return point(pX, pY);
	}
	

}

void Raster::Sutherland_Hodgeman(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*> &container)
{
	int minx = point1.x, maxx = point2.x, miny = point1.y, maxy = point2.y;
	for (int i = 0; i < container.size(); ++i)
	{		
		StyledGeometry* pStyledGeometry = container[i];
		ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
		if (pStyle->drawMethod == otDrawPolygon)
		{
			
			vector<point> points;
			PolygonGeometry* pPolygonGeometry = (PolygonGeometry*)pStyledGeometry->pGeometry;
			const vector<Point2D>& pts = pPolygonGeometry->getPts();
			if (pts.size() > 0)
			{
				for (int j = 0; j < pts.size() - 1; ++j)
					points.push_back(point(pts[j].x, pts[j].y)); //�������һ��LIST��
			}
			
			container.erase(container.begin()+i);
			vector<VectorLine> vectors;
			vectors.push_back(VectorLine(point(maxx, miny), point(maxx, maxy)));
			vectors.push_back(VectorLine(point(maxx, maxy), point(minx, maxy)));
			vectors.push_back(VectorLine(point(minx, maxy), point(minx, miny)));
			vectors.push_back(VectorLine(point(minx, miny), point(maxx, miny)));
			vector<point> result, cur;
			int vectorsSize = vectors.size();
			int pointSize = points.size();
			if (points.size() > 0)
			{
				point& S = points[points.size() - 1];
				//��ʼ�����鼯��
				for (int j = 0; j < points.size(); ++j)
					result.push_back(points[j]);
				bool flag;
				for (int j = 0; j < vectors.size(); ++j)
				{
					//flag ==false��ʾ���ڲ�,λtrue�Ļ���ʾ�����
					if (isInside(S, vectors[j]))
						flag = false;
					else
						flag = true;
					int resultSize = result.size();
					for (int k = 0; k < resultSize; ++k)
					{
						if (isInside(result[k], vectors[j]))
						{
							//���ǰһ������vector����࣬��ô�����ǵĽ�����뵽��ǰ�Ľ������
							if (flag)
							{
								flag = false;
								cur.push_back(Intersection(S, result[k], vectors[j].start, vectors[j].end));
							}
							cur.push_back(result[k]);
						}
						else
						{
							if (!flag)
							{
								flag = true;
								cur.push_back(Intersection(S, result[k], vectors[j].start, vectors[j].end));
							}

						}
						//���ڸ����״αȽϵĽڵ�
						S = result[k];

					}
					//����һ�εĽ�����б��棬����Ϊ��һ�ζԱȵ��������������εĽ���������
					int reaultLen = cur.size();
					result.clear();
					for (int k = 0; k < reaultLen; ++k)
					{
						result.push_back(cur[k]);
					}
					cur.clear();
				}

				int K = result.size();
				PixelPoint* poinTs = new PixelPoint[K + 1];
				int j;
				for (j = 0; j < result.size(); j++)
				{
					poinTs[j].x = result[j].x;
					poinTs[j].y = result[j].y;
				}
				poinTs[j].x = poinTs[0].x;
				poinTs[j].y = poinTs[0].y;


				PolygonGeometry* npPolygonGeometry = new PolygonGeometry();
				for (int j = 0; j < K; ++j)
				{
					npPolygonGeometry->addPoint(result[j].x, result[j].y);
				}
				if (result.size() > 0)
				{
					npPolygonGeometry->addPoint(result[0].x, result[0].y);
				}

				ColorStyle* pStyle;
				pStyle = new ColorStyle(getPenColor(), otDrawPolygon);
				StyledGeometry* pStyledGeometry = new StyledGeometry(npPolygonGeometry, pStyle);
				container.push_back(pStyledGeometry);
				//Raster::Bresenham_draw_Polyline(poinTs, result.size() + 1, getPenColor()); //DDA�̷�ȱ��
				free(poinTs);
				result.clear();
				vectors.clear();
				points.clear();
			}				
		}		
	}
}


void Raster::drawTriangle(Triangle* pTriangle)
{
	double z0, z1,z2;
	switch (g_State.VertexDeclare)
	{
		case State::vdXYZ:
		{
			z0 = ((PixelPoint_Z*)pTriangle->pts[0])->z;
			z1 = ((PixelPoint_Z*)pTriangle->pts[1])->z;
			z2 = ((PixelPoint_Z*)pTriangle->pts[2])->z;
		}
	}

}





void Raster::Draw3DGraph(Vec3d (&Graph3D)[8])
{
	PixelPoint T3DPoints[8];
	for (int i = 0; i < 8; ++i)
	{		
		T3DPoints[i].x = Graph3D[i].x;
		T3DPoints[i].y = Graph3D[i].y;
	}

	Raster::Bresenham_draw_Line(T3DPoints[0], T3DPoints[1], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[1], T3DPoints[2], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[2], T3DPoints[3], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[3], T3DPoints[0], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[4], T3DPoints[5], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[5], T3DPoints[6], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[6], T3DPoints[7], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[7], T3DPoints[4], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[0], T3DPoints[4], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[1], T3DPoints[5], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[2], T3DPoints[6], BLACK);
	Raster::Bresenham_draw_Line(T3DPoints[3], T3DPoints[7], BLACK);
}