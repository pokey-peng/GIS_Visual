#ifndef _DRAWSTATE_H
#define _DRAWSTATE_H
#include "Graphic.h"
typedef void (*DrawModelCallBack) (int x, int y, Color color);
enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline,
	otDrawCircle, otDrawEllipse,
	//像素画图
	otPixel, ot10Network, ot20Network,
	// 填充
	otAreaFill

};
enum DrawModeType { dmPixel, dmGrid, dmGemotry };
enum DrawLineMethodType { dlDDA, dlMidBresenham };
enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
enum FillMethodType { seedFill, scanFill };
struct State
{
	
	OperationType g_OperationType = otNone;
	DrawModelCallBack drawPixelCB = setPixel;
	// 绘制模式
	DrawModeType DrawMode;
	int GridWidth = 10; // 网格宽
	int GridHeight = 10; // 网格高

	Color GridColor1 = WHITE;
	Color GridColor2 = GREEN;

	// 画线方法
	DrawLineMethodType DrawLineMethod;
	// 多边形填充算法
	DrawPolygonMethodType DrawPolygonMethod;
	// 边表节点
	struct tagEDGE {
		double x;
		double dx;
		int ymax;
	};

	// 区域填充算法
	

	FillMethodType FillMethod;

	State();
    
};
extern State g_State;
#endif // !_DRAWSTATE_H
