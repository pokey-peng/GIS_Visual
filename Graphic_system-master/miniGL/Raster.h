#pragma once
#include "Graphic.h"
#include <vector>
#include "GeoDefine.h"
#include "Vec.h"
#include "Matrix.h"
using namespace std;

struct PixelPoint_Z : public PixelPoint
{
	double z;
};

struct Triangle
{
	//定义三角形的三个顶点
	PixelPoint* pts[3];
};

class Raster
{
public:

	static void drawTriangle(Triangle* pTriangle);

	//画一个小方格的循环算法
	static void drawCell(int row, int col, unsigned color);

	//绘制画布棋盘格的算法
	static void drawGrid();

	//绘制矩形的方法
	static void draw_Rectangle(PixelPoint& point1, PixelPoint& point2, Color color);

	//DDA画线的算法
	static void DDA_draw_Line(PixelPoint point1, PixelPoint point2, Color color);

	//Bresenham算法画直线
	static void Bresenham_draw_Line(PixelPoint point1, PixelPoint point2, Color color);
	
	//Bresenham算法画折线
	static void Bresenham_draw_Polyline(PixelPoint Points[], int count, Color color);


	//绘制多边形
	static void DrawDDA_Polygon(PixelPoint Points[], int Count,Color color);

	

	//基于Bresenham算法的八分之一圆的算法，采用的是对称性的原则，圆的光栅化
	static void Bresenham_draw_Circle(PixelPoint point1, PixelPoint point2, Color color);

	//基于Bresenham算法的椭圆的算法，采用的也是对称性的原则，椭圆的光栅化
	static void Bresenham_draw_ellipse(PixelPoint point1, PixelPoint point2, Color color);

	

	//活性边算法，首先需要的是定义一个比较大的N值定义数组
	static void ActiveEdge_Polygon(PixelPoint pts[], int Num_point,Color);

	//边缘填充算法
	static void Edge_Fill(PixelPoint Points[], int Num_point, Color color);

	//直线下的MODE为GRID
	static void Draw_Line_grid(int x1, int y1, int x2, int y2, unsigned color);


	

	//点集坐标转换函数
	static void ChangeOf_XYs(PixelPoint *Points, int Num);

	//对两个点坐标进行变换
	static void ChangeOf_XYs(int *x1, int *y1, int *x2, int *y2);

	//边缘填充算法中查找点的最大的x值
	static int FindMax_x(PixelPoint Points[], int Count);

	//一条边的填充
	static void Boundary_SigleEdge(int x1, int y1, int x2, int y2, int max_x, Color color);

	//判断对象，不论是像素还是网格下的，他们的颜色是不是一样的和白色的
	static bool Raster::IsWhite(int y, int x);

	//栅栏填充算法
	static void Raster::Fence_Fill(PixelPoint Points[], int Count, Color color);
	//栅栏填充一条边的
	static void Fence_SingleEdge(int x1, int y1, int x2, int y2, int index_x, Color color);

	//用于填充面的时候的DDA方式的扫描线
	static void DDA(int x1, int y1, int x2, int y2,Color color);
	
	//边标志法进行填充
	static void Edge_TagMethod(PixelPoint Points[], int Count, Color color);

	//区域填充算法
	//区域填充方法1  ——边界填充算法 采用递归调用的方式
	static void Boundary_Fill(int x, int y, Color BoundaryColor, Color FillColor);
	
	//将填充点进行栈结构定义
	typedef struct PointsStack
	{
		PixelPoint SeedPoint;
		struct PointsStack *next;
	}PointsStack;
	//将种子点进行压栈
	static void PushSeed(PointsStack* Points, PixelPoint SeedPoint);
	//将种子点出栈
	static int PopSeed(PointsStack* Points, PixelPoint& SeedPoint);
	//边界填充算法 种子点边界填充算法 采用压栈的方式
	static void SeedBoundaryFill4(PixelPoint SeedPoint, Color OldColor, Color FillColor);

	//边界填充算法优化，采用压栈扫描线的方式
	static void SeedBoundaryFill4_Line(PixelPoint SeedPoint, Color OldColor, Color FillColor);


	//边界填 范填充算法,相当于重新为已经着色的像素重新着色
	static void FloodFill4(PixelPoint SeedPoint, Color OldColor, Color FillColor);

	//直线的裁剪
	//Cohen_Sutherland算法
	static void Cohen_Sutherland(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& polylineObject);

	static void midpointSubdivisionAlgorithm(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*> &container);

	static void LBLine(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& container);

	static void Sutherland_Hodgeman(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*> &container);


	//三维部分
	static void Draw3DGraph(Vec3d(&Graph3D)[8]);
};





