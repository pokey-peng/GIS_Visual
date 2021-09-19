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

///����˵���Ϣ
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

///��������Ϣ
void handleKeyMessage(int key)
{
	bool bCtrlPressed = isCtrlKeyPressed();//�Ƿ�ͬʱ������Ctrl��
	bool bShiftPressed = isShiftKeyPressed();//�Ƿ�ͬʱ������Shift��
	switch (key)
	{
	case VK_UP: // ��һ�У��Ϲ��	
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

///���������Ϣ
void handleMouseMessage(UINT message, int x, int y, int det)
{
	DPtToLPt(x, y, x, y);//�����豸����תΪ�����߼�����

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

///���������Ϣ
void display()
{
	setYUp(true);//y������
	setOrig(getWindowWidth() / 2, getWindowHeight() / 2);// ԭ��Ϊ��������

	if( g_OperationType == otDrawRectangleOutline && g_PointCout == 2 )
	{
		int x0 = g_Points[0].x, y0 = g_Points[0].y;
		int x1 = g_Points[1].x, y1 = g_Points[1].y;
		int xmin = min(x0, x1), xmax = max(x0, x1);
		int ymin = min(y0, y1), ymax = max(y0, y1);
		for (int i = xmin; i <= xmax; ++i)//��ָ����ɫ���ƾ���
		{
			for (int j = ymin; j <= ymax; ++j)
			{
				setPixel(i, j, _RGB(255, 255, 0));
				//setPixel( i, j, YELLOW );
			}
		}
	}
}

///������Ϣ����ڵ�
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://�˵���Ϣ
	{
		int menuID = LOWORD(wParam);
		handleMenuMessage(menuID);
		return FALSE;
	}
		
	case WM_PAINT://������Ϣ
		display();
		return FALSE;
		
	case WM_CREATE:
		init((unsigned)hWnd);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_KEYDOWN://������Ϣ
	{
		int key = wParam;//��ð�������
		handleKeyMessage(key);
		return FALSE;
	}		
	case WM_MOUSEMOVE://����ƶ���Ϣ
	case WM_LBUTTONDOWN://������������Ϣ	
	case WM_LBUTTONUP://������������Ϣ
	case WM_RBUTTONUP://����Ҽ�������Ϣ
	case WM_RBUTTONDOWN://����Ҽ�������Ϣ
	case WM_LBUTTONDBLCLK://���˫����Ϣ
	case WM_MOUSEWHEEL://��������Ϣ
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

