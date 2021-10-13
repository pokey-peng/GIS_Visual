#ifndef _DRAWSTATE_H
#define _DRAWSTATE_H
#include "Graphic.h"
typedef void (*DrawModelCallBack) (int x, int y, Color color);
enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline,
	otDrawCircle, otDrawEllipse,
	//���ػ�ͼ
	otPixel, ot10Network, ot20Network,
	// ���
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
	// ����ģʽ
	DrawModeType DrawMode;
	int GridWidth = 10; // �����
	int GridHeight = 10; // �����

	Color GridColor1 = WHITE;
	Color GridColor2 = GREEN;

	// ���߷���
	DrawLineMethodType DrawLineMethod;
	// ���������㷨
	DrawPolygonMethodType DrawPolygonMethod;
	// �߱�ڵ�
	struct tagEDGE {
		double x;
		double dx;
		int ymax;
	};

	// ��������㷨
	

	FillMethodType FillMethod;

	State();
    
};
extern State g_State;
#endif // !_DRAWSTATE_H
