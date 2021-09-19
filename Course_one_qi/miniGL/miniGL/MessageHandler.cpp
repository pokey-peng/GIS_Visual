#include <windows.h>
#include "MessageHandler.h"
#include "Resource.h"
#include "Graphic.h"
#include "MyGraphic.h"
#include "MyGeoDefine.h"
#include "MyGeometryLibrary.h"
#include "MyRaster.h"

using namespace std;

enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline, 
	otDrawCircle, otDrawEllipse,
	//像素画图
	otPixel,ot10Network,ot20Network

};
// 定义一个数据集，存放图层
Dataset g_Dataset;
Dataset* g_Dataset_f = &g_Dataset;


Color g_Color = RED;
int g_PointCout;

// 获取当前模式
RubberMode rm;
OperationType g_OperationType = otNone; // 当前操作类型
PixelPoint g_Points[100];

// 定义一个图层指针
Layer* g_player = NULL;
/*
void ColorChange()
{
	if (g_OperationType == otPixel)
	{
		Color colors[3] = { RED, GREEN, BLUE };
		static int tag = 1;
		g_Color = colors[tag];
		refreshWindow();
		tag++;
		if (tag == 3)
			tag = 0;
	}
	else
	{
		static int tag = 3;
		switch (tag)
		{
		case 3:
			g_Color_3 = GREEN;
			break;
		case 2:
			g_Color_1 = BLUE;
			break;
		case 1:
		{
			g_Color_1 = RED;
			g_Color_3 = BLUE;
			tag = 4;
			break;
		}

		}
		tag--;
		refreshWindow();
	}
}
*/
///处理菜单消息
void handleMenuMessage(int menuID)
{
	switch (menuID)
	{
	case ID_DRAWMODE_10:
	{
		
		g_OperationType = ot10Network;
		setRubberMode(rmNone);
		refreshWindow();
		break;
	}

	case ID_DRAWMODE_PIXEL:
	{
		g_Color = RED;
		g_OperationType = otPixel;
		setRubberMode(rmNone);
		refreshWindow();
		break;
	}

	case ID_DRAWMODE_20:
	{
		
		g_OperationType = ot20Network;
		setRubberMode(rmNone);
		refreshWindow();
		break;
	}
	case ID_2D_RECTANGLE:
	{
		setCursor(csSize);
		setRubberMode(rmRectangle);
		g_OperationType = otDrawRectangleOutline;
		refreshWindow();
		break;
	}
	case ID_2D_Line:
	{
		setCursor(csArrow);
		setRubberMode(rmLine);
		g_OperationType = otDrawLine;
		refreshWindow();
		break;
	}
	case ID_2D_POLYLINE:
	{
		setCursor(csHand);
		setRubberMode(rmPolyline);
		g_OperationType = otDrawPolyline;
		refreshWindow();
		break;
	}
	case ID_2D_POLYGON:
	{
		setCursor(csCross);
		setRubberMode(rmPolygon);
		g_OperationType = otDrawPolygonOutline;
		refreshWindow();
		break;
	}
	case ID_2D_drawCircle:
	{
		setCursor(csSize);
		setRubberMode(rmLine);
		g_OperationType = otDrawCircle;
		break;
	}
	case ID_2D_Ellipse:
	{
		setCursor(csCross);
		setRubberMode(rmRectangle);
		g_OperationType = otDrawEllipse;
		break;
	}
	case ID_RESET:
	{
		// 清空当前画布，添加一个新的图层
		setRubberMode(rmNone);
		g_OperationType = otNone;
		if (g_player->EmptyGeometry()) {} // 若当前图层为空则不添加新图层
		else
		{
			g_player = new Layer();
			g_Dataset.addLayer(g_player);
		}
		refreshWindow();
		break;
	}

	case IDM_EXIT:
		PostQuitMessage(0);
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
	case 'R':
	{
		g_Color = BLUE;
		refreshWindow();
		break;
	}
		
		
	}
}

///处理鼠标消息
void handleMouseMessage(UINT message, int x, int y, int det)
{
	DPtToLPt(x, y, x, y);//窗口设备坐标转为窗口逻辑坐标

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		//MessageBox(hWnd, TEXT("哎呀，我是彭棋！"), TEXT("你好啊！"), MB_OK);
		break;
	}
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONUP:
	{
		rm = getRubberMode();
		if (rm == rmNone || rm == rmPolygon || rm == rmPolygon) break;

		int c = getRubberPointCount();
		if (c == 2)
		{
			PixelPoint pt1, pt2;
			getRubberPoints(pt1, pt2);

			switch (g_OperationType)
			{
			case otDrawLine:
			case otDrawCircle:
			{
				if (pt1.x == pt2.x && pt1.y == pt2.y) return;

				if (g_OperationType == otDrawLine)
					g_player->addGeometry(GeometryLibrary::createPolylineGeometry(pt1, pt2));
				else
					g_player->addGeometry(GeometryLibrary::createCircleGeometry(pt1.x, pt1.y, pt2.x, pt2.y));
				refreshWindow();
			}
			break;
			case otDrawRectangle:
			case otDrawRectangleOutline:
			case otDrawEllipse:
			{
				double x1 = pt1.x;
				double y1 = pt1.y;
				double x2 = pt2.x;
				double y2 = pt2.y;
				if (x1 == x2 || y1 == y2) return;

				if (g_OperationType == otDrawRectangle)
					g_player->addGeometry(GeometryLibrary::creatRectangleGeometry(x1, y1, x2, y2));
				else if (g_OperationType == otDrawRectangleOutline)
					g_player->addGeometry(GeometryLibrary::creatRectangleOutlineGeometry(x1, y1, x2, y2));
				else
					g_player->addGeometry(GeometryLibrary::createEllipseGeometry(x1, y1, x2, y2));
				refreshWindow();
			}
			break;
			}
		}
	}
		break;
	case WM_RBUTTONUP:
	{
		RubberMode rm = getRubberMode();
		if (rm != rmPolyline && rm != rmPolygon) break;

		int c = getRubberPointCount();
		switch (g_OperationType)
		{
		case otDrawPolyline:
		{
			if (c < 2) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			g_player->addGeometry(GeometryLibrary::createPolylineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}
		break;
		case otDrawPolygon:
		case otDrawPolygonOutline:
		{
			if (c < 3) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			if (g_OperationType == otDrawPolygon)
				g_player->addGeometry(GeometryLibrary::createPolygonGeometry(pts.data(), pts.size()));
			else
				g_player->addGeometry(GeometryLibrary::createPolygonOutlineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}
		}
		/*
		RubberMode rm = getRubberMode();
		if (rm == rmNone) return;

		int c = getRubberPointCount();
		if (g_OperationType == otDrawPolyline && c < 2) return;
		if (g_OperationType == otDrawPolygonOutline && c < 3) return;

		g_PointCout = c;
		getRubberPoints(g_Points);
		refreshWindow();
		*/
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
void drawgrid(int irow, int icol, int w, int h,Color Color)
{
	for (int row = irow; row < irow + h; ++row)//用指定颜色绘制矩形
	{
		for (int col = icol; col < icol + w; ++col)
		{
			setPixel(row, col, Color);
		}
	}
}
void drawDataset();
void drawLayer(Layer* player);
void drawDataset()
{
	drawLayer(g_Dataset.layerSet.back());
	/*
	for (int s = g_Dataset.getLayerCount(), i = s - 1; i >= 0; --i)
	{
		drawLayer(g_Dataset[i]);
	}
	*/
}
void drawLayer(Layer* player)
{
	for (int i = 0, size = player->getGeometryCount(); i < size; ++i)
	{
		Geometry2D* pGeometryBase = (*player)[i];
		switch (pGeometryBase->getGeomType())
		{
		case gtPoint:
		{
			PointGeometry* pGeometry = (PointGeometry*)pGeometryBase;
			Raster::drawPoint( pGeometry->x, pGeometry->y ,getPenColor());
		}
		break;
		case gtPolyline:
		{
			PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryBase;
			const vector<Point2D>& pts = pGeometry->getPts();
			for (int i = 0, c = pts.size(); i < c - 1; ++i)
			{
				Raster::drawLine(pts[i].x, pts[i].y, pts[(i + 1)].x, pts[(i + 1)].y, getPenColor());
			}
		}
		break;
		case gtPolygon:
		{
			PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryBase;
			const vector<Point2D>& pts = pGeometry->getPts();
			vector<PixelPoint> pts2(pts.size());
			for (int i = 0, s = pts.size(); i < s; ++i)
			{
				pts2[i].x = pts[i].x;
				pts2[i].y = pts[i].y;
			}
			Raster::drawPolygon(pts2.data(), pts2.size(), getPenColor());
		}
		break;
		case gtCircle:
		{
			CircleGeometry* pGeometry = (CircleGeometry*)pGeometryBase;
			Raster::drawCircle(pGeometry->x, pGeometry->y, pGeometry->r, getPenColor());
		}
		break;

		case gtEllipse:
		{
			EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryBase;
			double x, y;
			pGeometry->getCenter(x, y);
			Raster::drawEllipse(x, y, pGeometry->getWidth(), pGeometry->getHeight(), getPenColor());
		}
		break;
		}
	}
}
///处理绘制消息
void display()
{

	int w = getWindowWidth();//1423
	int h = getWindowHeight();//730

	setYUp(true);//y轴向上
	setOrig(0, h);// 原点为窗口左下角
	rm = getRubberMode();
	if (g_OperationType && rm)
	{
		drawDataset();
	}
	else
	{
		return;
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
		g_player = new Layer();
		g_Dataset.addLayer(g_player);
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

