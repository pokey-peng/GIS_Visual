#include "Raster.h"
#include "State.h"
#include "Graphic.h"
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

//��һ��С����
void Raster::drawCell(int row, int col , unsigned color )
{
	for( int i = row * g_State.GridHeight; i < (row +1)* g_State.GridHeight;++i  )
		for ( int j = col * g_State.GridWidth; j < (col + 1)* g_State.GridWidth; ++j )
		{
			setPixel( j,  i, color);
		}
}

//������
void Raster::drawGrid()
{
	int orig_x, orig_y;
	getOrig( orig_x, orig_y );

	int left = (int)((float)orig_x / g_State.GridWidth + 0.5);
	int right = (int)((float)( getWindowWidth() - orig_x) / g_State.GridWidth + 0.5);
	int top = (int)((float)orig_y / g_State.GridHeight + 0.5);
	int bottom = (int)((float)( getWindowHeight() - orig_y) / g_State.GridHeight + 0.5);

	int ymin,ymax;
	if( isYUp())
	{
		ymin = - bottom;
		ymax = top;
	}
	else
	{
		ymin = -top;
		ymax = bottom;
	}
	for( int col = -left ; col <= right; ++col )
		for ( int row = ymin ; row <= ymax; ++row )
		{
			drawCell(row, col , (row + col)%2 ? g_State.GridColor1:g_State.GridColor2  );
		}
}

//��һ����
void Raster::drawPoint(double x, double y) {
	drawCell(int(x), int(y), BLACK);
}

//���� Bresenham�е㻭���㷨
void Raster::drawLine(double x1, double y1, double x2, double y2, unsigned color) {
	int w;//��
	int h;//��
	w = int(x2 - x1);
	h = int(y2 - y1);

	//�ж�ֱ�ߵ�б��k>0����<0
	int dx = ((w > 0) << 1) - 1;
	int dy = ((h > 0) << 1) - 1;
	w = abs(w);
	h = abs(h);
	int f, y, x, delta1, delta2;

	//ֱ��б��k<1��x����1
	if (w > h)
	{
		f = 2 * h - w;
		delta1 = 2 * h;
		delta2 = (h - w) * 2;
		for (x = x1, y = y1; x != x2; x += dx)
		{
			setPixel(x, y, color);
			if (f < 0)
			{
				f += delta1;
			}
			else
			{
				y += dy;
				f += delta2;
			}
		}
		setPixel(x, y, color);
	}
	//ֱ��б��k>1��y����1
	else
	{
		f = 2 * w - h;
		delta1 = w * 2;
		delta2 = (w - h) * 2;
		for (x = x1, y = y1; y != y2; y += dy)
		{
			setPixel(x, y, color);
			if (f < 0)
			{
				f += delta1;
			}
			else
			{
				x += dx;
				f += delta2;
			}
		}
		setPixel(x, y, color);
	}
}



//�������أ�����������
PixelPoint operator-(const PixelPoint& a,const PixelPoint& b) {
	PixelPoint c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}
PixelPoint operator*(const PixelPoint& a, const float& b) {
	PixelPoint c;
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	return c;
}
PixelPoint operator+(const PixelPoint& a, const PixelPoint& b) {
	PixelPoint c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}


//��ʵ��������
void Raster::drawTriangle(PixelPoint* pts,unsigned color) {
	
	PixelPoint t0 = pts[0];
	PixelPoint t1 = pts[1];
	PixelPoint t2 = pts[2];

	if (t0.y == t1.y && t0.y == t2.y) return;

	//t0,t1,t2 �ɸߵ���Ϊ�����ε���������
	if (t0.y > t1.y)std::swap(t0, t1);
	if (t0.y > t2.y)std::swap(t0, t2);
	if (t1.y > t2.y)std::swap(t1, t2);

	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		//second_half����������һ��Ϊ�����Ϻ��£�1Ϊ�ϰ벿�֣�0Ϊ�°벿��
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		//segment_heightΪ��t1��ˮƽ����t2��t0�ľ��룬��Ϊ����С�����εĸ�
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		//��Ϊt0��t2��ˮƽ�ߵļнǣ���Ϊt2��t1��ˮƽ�ߵļн�
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; 
		//AΪt0��t2�����ߣ�BΪt0��t1������
		PixelPoint A = t0 + (t2 - t0) * alpha;
		PixelPoint B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			setPixel(j, t0.y + i, color); 
		}
	}

}





//�����Ķ����
void Raster::drawPolygonOutline(PixelPoint* pts, int size, unsigned color) {
	for (int i = 0;i < size;i++) {
		if (i == size - 1)
			drawLine(pts[i].x, pts[i].y, pts[0].x, pts[0].y, color);
		else
			drawLine(pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y, color);

	}
}

void Raster::drawTriangleOutline(PixelPoint* pts,  unsigned color) {
	drawLine(pts[0].x, pts[0].y, pts[1].x, pts[1].y, color);
	drawLine(pts[0].x, pts[0].y, pts[2].x, pts[2].y, color);
	drawLine(pts[1].x, pts[1].y, pts[1].x, pts[1].y, color);
}


//��Բ �˷ַ���Բ�㷨 ����Բ
void Raster::drawCircleOutline(double x, double y, double r, unsigned color) {
	//Բ����ֱ���Ͼ���Ϊr�ĵ�(i,j)
	int i, j, d;

	i = 0;
	j = r;
	d = int(3 - 2 * r);

	setPixel(i, j, color);
	while(i<j){
		if (d < 0)
			d += 4 * i + 6;
		else {
			d += 4 * (i - j) + 10;
			j--;
		}
		i++;
		//x��y��Գ���
		setPixel(i+x, j+y, color);
		setPixel(i+x, -j+y, color);
		setPixel(-i+x, j+y, color);
		setPixel(-i+x, -j+y, color);
		//ֱ��y=x�Գ��Ի�ʣ�µ�1/2
		setPixel(j+x, i+y, color);
		setPixel(-j+x, i+y, color);
		setPixel(j+x, -i+y, color);
		setPixel(-j+x, -i+y, color);
	}
}

void Raster::drawEllipses(double xc, double yc, double a, double b, unsigned color) {
	int x, y;
	float d1, d2, aa, bb;
	aa = a * a;
	bb = b * b;
	d1 = bb + aa * (-b + 0.25);

	x = 0;
	y = b;

	setPixel(x + xc, y + yc, color);
	setPixel(-x + xc, -y + yc, color);
	setPixel(-x + xc, y + yc, color);
	setPixel(x + xc, -y + yc, color);
	while (bb * (x + 1) < aa * (y - 0.5))
	{
		if (d1 <= -0.000001)
		{
			d1 += bb * ((x << 1) + 3);

		}
		else
		{
			d1 += bb * ((x << 1) + 3) + aa * (2 - (y << 1));
			--y;
		}
		++x;
		setPixel(x + xc, y + yc, color);
		setPixel(-x + xc, -y + yc, color);
		setPixel(-x + xc, y + yc, color);
		setPixel(x + xc, -y + yc, color);
	}
	d2 = bb * (0.25 * x) + aa * (1 - (y << 1));
	while (y > 0)
	{
		if (d2 <= -0.000001)
		{
			++x;
			d2 += bb * ((x + 1) << 1) + aa * (3 - (y << 1));
		}
		else
		{
			d2 += aa * (3 - (y << 1));
		}
		--y;
		setPixel(x + xc, y + yc, color);
		setPixel(-x + xc, -y + yc, color);
		setPixel(-x + xc, y + yc, color);
		setPixel(x + xc, -y + yc, color);
	}

}
