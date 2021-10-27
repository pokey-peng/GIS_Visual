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
	//���������ε���������
	PixelPoint* pts[3];
};

class Raster
{
public:

	static void drawTriangle(Triangle* pTriangle);

	//��һ��С�����ѭ���㷨
	static void drawCell(int row, int col, unsigned color);

	//���ƻ������̸���㷨
	static void drawGrid();

	//���ƾ��εķ���
	static void draw_Rectangle(PixelPoint& point1, PixelPoint& point2, Color color);

	//DDA���ߵ��㷨
	static void DDA_draw_Line(PixelPoint point1, PixelPoint point2, Color color);

	//Bresenham�㷨��ֱ��
	static void Bresenham_draw_Line(PixelPoint point1, PixelPoint point2, Color color);
	
	//Bresenham�㷨������
	static void Bresenham_draw_Polyline(PixelPoint Points[], int count, Color color);


	//���ƶ����
	static void DrawDDA_Polygon(PixelPoint Points[], int Count,Color color);

	

	//����Bresenham�㷨�İ˷�֮һԲ���㷨�����õ��ǶԳ��Ե�ԭ��Բ�Ĺ�դ��
	static void Bresenham_draw_Circle(PixelPoint point1, PixelPoint point2, Color color);

	//����Bresenham�㷨����Բ���㷨�����õ�Ҳ�ǶԳ��Ե�ԭ����Բ�Ĺ�դ��
	static void Bresenham_draw_ellipse(PixelPoint point1, PixelPoint point2, Color color);

	

	//���Ա��㷨��������Ҫ���Ƕ���һ���Ƚϴ��Nֵ��������
	static void ActiveEdge_Polygon(PixelPoint pts[], int Num_point,Color);

	//��Ե����㷨
	static void Edge_Fill(PixelPoint Points[], int Num_point, Color color);

	//ֱ���µ�MODEΪGRID
	static void Draw_Line_grid(int x1, int y1, int x2, int y2, unsigned color);


	

	//�㼯����ת������
	static void ChangeOf_XYs(PixelPoint *Points, int Num);

	//��������������б任
	static void ChangeOf_XYs(int *x1, int *y1, int *x2, int *y2);

	//��Ե����㷨�в��ҵ������xֵ
	static int FindMax_x(PixelPoint Points[], int Count);

	//һ���ߵ����
	static void Boundary_SigleEdge(int x1, int y1, int x2, int y2, int max_x, Color color);

	//�ж϶��󣬲��������ػ��������µģ����ǵ���ɫ�ǲ���һ���ĺͰ�ɫ��
	static bool Raster::IsWhite(int y, int x);

	//դ������㷨
	static void Raster::Fence_Fill(PixelPoint Points[], int Count, Color color);
	//դ�����һ���ߵ�
	static void Fence_SingleEdge(int x1, int y1, int x2, int y2, int index_x, Color color);

	//����������ʱ���DDA��ʽ��ɨ����
	static void DDA(int x1, int y1, int x2, int y2,Color color);
	
	//�߱�־���������
	static void Edge_TagMethod(PixelPoint Points[], int Count, Color color);

	//��������㷨
	//������䷽��1  �����߽�����㷨 ���õݹ���õķ�ʽ
	static void Boundary_Fill(int x, int y, Color BoundaryColor, Color FillColor);
	
	//���������ջ�ṹ����
	typedef struct PointsStack
	{
		PixelPoint SeedPoint;
		struct PointsStack *next;
	}PointsStack;
	//�����ӵ����ѹջ
	static void PushSeed(PointsStack* Points, PixelPoint SeedPoint);
	//�����ӵ��ջ
	static int PopSeed(PointsStack* Points, PixelPoint& SeedPoint);
	//�߽�����㷨 ���ӵ�߽�����㷨 ����ѹջ�ķ�ʽ
	static void SeedBoundaryFill4(PixelPoint SeedPoint, Color OldColor, Color FillColor);

	//�߽�����㷨�Ż�������ѹջɨ���ߵķ�ʽ
	static void SeedBoundaryFill4_Line(PixelPoint SeedPoint, Color OldColor, Color FillColor);


	//�߽��� ������㷨,�൱������Ϊ�Ѿ���ɫ������������ɫ
	static void FloodFill4(PixelPoint SeedPoint, Color OldColor, Color FillColor);

	//ֱ�ߵĲü�
	//Cohen_Sutherland�㷨
	static void Cohen_Sutherland(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& polylineObject);

	static void midpointSubdivisionAlgorithm(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*> &container);

	static void LBLine(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*>& container);

	static void Sutherland_Hodgeman(PixelPoint point1, PixelPoint point2, vector<StyledGeometry*> &container);


	//��ά����
	static void Draw3DGraph(Vec3d(&Graph3D)[8]);
};





