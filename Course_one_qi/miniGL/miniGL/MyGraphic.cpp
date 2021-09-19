#include <windows.h>
#include "MyGraphic.h"
#include "Graphic.h"
#include <vector>
extern Color g_Color; // 线段颜色
extern PixelPoint g_Points[100]; // 存放需要绘制的点的数组
extern int g_PointCout;  //点的个数

int sign(int val)
{
	if (val > 0)
		return 1;
	else if (val < 0)
		return -1;
	else
		return 0;
}
/// <summary>
/// Bresenham中点算法
/// </summary>
/// <param name="startp">开始绘制起点坐标</param>
/// <param name="endp">绘制终点坐标</param>
void MidPointLine(PixelPoint startp, PixelPoint endp)
{
	int dex = abs(startp.x - endp.x);
	int dey = abs(startp.y - endp.y);
	bool tag = false;
	// 当直线斜率大于1时
	if (dex < dey)
	{
		std::swap(dex, dey);
		tag = true;
	}
	// x,y增量
	int dx = sign(endp.x - startp.x); int dy = sign(endp.y - startp.y);
	int d = dex - dey - dey; // 初始增量
	int x = startp.x; int y = startp.y; // 起点
	for (int i = 0; i <= dex; ++i)
	{
		setPixel(x, y, g_Color);
		if (d < 0) // d<0时，x,y分别加上加dx,dy
		{
			x += dx; y += dy;
			d += (dex + dex - dey - dey); // 增量更新2dex-2dey
		}
		else
		{
			// d>0且斜率大于1时，y更新
			if (tag == true) 
				y += dy;
			else // d>0且斜率不大于1时，x更新
				x += dx;
			d -= (dey + dey); // 增量更新-2dey
		}
	}
	
}
/// <summary>
/// DDA直线算法
/// </summary>
/// <param name="startp">开始绘制起点坐标</param>
/// <param name="endp">绘制终点坐标</param>
void DDALine(PixelPoint startp, PixelPoint endp)
{
	int x = 0,dex,dey,dexx,deyy;
	float dx,dy,xp,yp;
	dexx = endp.x - startp.x;
	deyy = endp.y - startp.y;
	dex = abs(dexx);
	dey = abs(deyy);
	dx = (float)dexx / max(dex, dey);
	dy = (float)deyy / max(dex, dey);
	xp = startp.x;
	yp = startp.y;
	if (dex < dey)
	{
		std::swap(dex, dey);
	}
	while (x < dex)
	{
		setPixel(round(xp), round(yp), g_Color);
		xp += dx;
		yp += dy;
		x++;
	}

}

// 绘制折线
void DrawPolyline()
{
	// 起始索引
	int startp = 0; int endp = 1;
	// 遍历数组中存在的点
	while (endp < g_PointCout)
	{
		// 每两个点之间画一条线段
		//MidPointLine(g_Points[startp], g_Points[endp]);
		DDALine(g_Points[startp], g_Points[endp]);
		startp++;
		endp++;
	}
}
// 画空心矩形
void DrawRectangle()
{
	int x0 = g_Points[0].x, y0 = g_Points[0].y;
	int x1 = g_Points[1].x, y1 = g_Points[1].y;
	int xmin = min(x0, x1), xmax = max(x0, x1);
	int ymin = min(y0, y1), ymax = max(y0, y1);
	for (int j = ymin; j <= ymax; ++j)
	{
		setPixel(xmin, j, g_Color);
		setPixel(xmax, j, g_Color);
	}
	for (int i = xmin; i <= xmax; ++i)//用指定颜色绘制空心矩形
	{
		setPixel(i, ymin, g_Color);
		setPixel(i, ymax, g_Color);
	}
}
// 画空心多边形
void DrawPolygon()
{
	// 先把数组中的点按顺序依次画出线段
	int startp = 0; int endp = 1;

	while (endp < g_PointCout)
	{
		MidPointLine(g_Points[startp], g_Points[endp]);
		startp++;
		endp++;
	}
	// 最后在起始点和终点之间画线，闭合多边形
	MidPointLine(g_Points[0], g_Points[endp - 1]);
}

// 画圆对应八个点位置
void CirclePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	setPixel(xCenter + x, yCenter + y, color);
	setPixel(xCenter - x, yCenter + y, color);
	setPixel(xCenter + x, yCenter - y, color);
	setPixel(xCenter - x, yCenter - y, color);
	setPixel(xCenter + y, yCenter + x, color);
	setPixel(xCenter - y, yCenter + x, color);
	setPixel(xCenter + y, yCenter - x, color);
	setPixel(xCenter - y, yCenter - x, color);
}

// 画圆对应四个点位置
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	setPixel(xCenter + x, yCenter + y, color);
	setPixel(xCenter - x, yCenter + y, color);
	setPixel(xCenter + x, yCenter - y, color);
	setPixel(xCenter - x, yCenter - y, color);
}