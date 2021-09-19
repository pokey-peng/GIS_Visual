#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "Graphic.h"

enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline, otDrawCircle, otDrawEllipse
};

OperationType g_OperationType = otNone;
PixelPoint g_Points[100];
int g_PointCout = 0;

///处理菜单消息
void handleMenuMessage(int menuID)
{
	switch (menuID)
	{
	case ID_2D_LINE:
		setCursor(csArrow);
		setRubberMode(rmLine);
		g_OperationType = otDrawLine;
		break;
	case ID_2D_RECT:
		setCursor(csHand);
		setRubberMode(rmRectangle);
		g_OperationType = otDrawRectangleOutline;
		break;
	case ID_2D_POLYLINE:
		setCursor(csCross);
		setRubberMode(rmPolyline);
		g_OperationType = otDrawPolyline;
		break;
	case ID_2D_POLYGON:
		setCursor(csCross);
		setRubberMode(rmPolygon);
		g_OperationType = otDrawPolygonOutline;
		break;
	}
}

///处理按键消息
void handleKeyMessage(int key)
{
	bool bCtrlPressed = isCtrlKeyPressed();//是否同时按下了Ctrl键
	bool bShiftPressed = isShiftKeyPressed();//是否同时按下了Shift键
	switch (key)
	{
	case VK_UP: // 上一行，上光标	
		break;
	case VK_DOWN:
		break;
	case VK_LEFT:
		break;
	case VK_RIGHT:
		//refreshWindow();
		break;
		
	}
}

///处理鼠标消息
void handleMouseMessage(UINT message, int x, int y, int det)
{
	DPtToLPt(x, y, x, y);//窗口设备坐标转为窗口逻辑坐标

	switch (message)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
	{
		RubberMode rm = getRubberMode();
		if (rm == rmNone) return;

		int c = getRubberPointCount();
		if (c != 2) return;

		g_PointCout = 2;
		getRubberPoints(g_Points);

		refreshWindow();
	}
		break;
	case WM_RBUTTONUP:
	{
		RubberMode rm = getRubberMode();
		if (rm == rmNone) return;

		int c = getRubberPointCount();
		if ( g_OperationType == otDrawPolyline && c < 2 ) return;
		if ( g_OperationType == otDrawPolygon && c < 3 ) return;

		g_PointCout = c;
		getRubberPoints(g_Points);
		refreshWindow();
	}
		break;
	case WM_MOUSEWHEEL:
	{
		if (det > 0)
		{

		}
		break;
	}
	}
}

///处理绘制消息
void display()
{
	setYUp(true);//y轴向上
	setOrig(getWindowWidth() / 2, getWindowHeight() / 2);// 原点为窗口中心

	if( g_OperationType == otDrawRectangleOutline && g_PointCout == 2 )
	{
		int x0 = g_Points[0].x, y0 = g_Points[0].y;
		int x1 = g_Points[1].x, y1 = g_Points[1].y;
		int xmin = min(x0, x1), xmax = max(x0, x1);
		int ymin = min(y0, y1), ymax = max(y0, y1);
		for (int i = xmin; i <= xmax; ++i)//用指定颜色绘制矩形
		{
			for (int j = ymin; j <= ymax; ++j)
			{
				setPixel(i, j, _RGB(255, 255, 0));
				//setPixel( i, j, YELLOW );
			}
		}
	}
}

///所有消息的入口点
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://菜单消息
	{
		int menuID = LOWORD(wParam);
		handleMenuMessage(menuID);
		return FALSE;
	}
		
	case WM_PAINT://绘制消息
		display();
		return FALSE;
		
	case WM_CREATE:
		init((unsigned)hWnd);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_KEYDOWN://按键消息
	{
		int key = wParam;//获得按键代码
		handleKeyMessage(key);
		return FALSE;
	}		
	case WM_MOUSEMOVE://鼠标移动消息
	case WM_LBUTTONDOWN://鼠标左键按下消息	
	case WM_LBUTTONUP://鼠标左键弹起消息
	case WM_RBUTTONUP://鼠标右键弹起消息
	case WM_RBUTTONDOWN://鼠标右键按下消息
	case WM_LBUTTONDBLCLK://鼠标双击消息
	case WM_MOUSEWHEEL://鼠标滚轮消息
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		int det = message == WM_MOUSEWHEEL ? (short)HIWORD(wParam) : 0 ;
		handleMouseMessage(message, x, y, det);
		return FALSE;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

