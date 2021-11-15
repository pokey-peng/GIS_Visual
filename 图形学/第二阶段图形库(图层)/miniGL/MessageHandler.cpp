#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "Graphic.h"
#include <vector>
#include "GeoDefine.h"
#include "GeometryFactory.h"
#include "State.h"
#include "Raster.h"
#include "Base2DRenderer.h"
#include "Map2DRenderer.h"
#include "Utils.h"

using namespace std;

enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline, otDrawCircle, otDrawEllipse
};

OperationType g_OperationType = otNone;//当前操作类型
Dataset g_Dataset;
Layer* g_pLayer = NULL;
Base2DRenderer* g_pRenderer = NULL;

void initialize()
{
	g_pLayer = new Layer();
	g_Dataset.addLayer(g_pLayer);
	//g_pRenderer = new Base2DRenderer();
	//g_pRenderer->init(&g_Dataset);

	g_pRenderer = new Map2DRenderer();
}

void destroy()
{
	delete g_pRenderer;
}

///处理菜单消息
void handleMenuMessage(int menuID)
{
	switch (menuID)
	{
	case ID_2D_DRAW_RECT:
		g_OperationType = otDrawRectangle;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		break;
	case ID_2D_DRAW_LINE:
		g_OperationType = otDrawLine;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	case ID_2D_DRAW_POLYLINE:
		g_OperationType = otDrawPolyline;
		setRubberMode(rmPolyline);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_POLYGON:
		g_OperationType = otDrawPolygon;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_CIRCLE:
		g_OperationType = otDrawCircle;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	case ID_GRID_MODE:
		g_State.DrawMode = State::dmGrid;
		refreshWindow();
		break;
	case ID_PIXEL_MODE:
		g_State.DrawMode = State::dmPixel;
		refreshWindow();
		break;
	case ID_ADD_LAYER:
		{
			Layer* pLayer = Utils::openLayer();
			if (pLayer)
			{
				g_Dataset.addLayer(pLayer);
				if (g_Dataset.getLayerCount() == 2)//添加第一个图层时设置初始变换矩阵和比例尺
				{
					double dx = pLayer->envelop.centerX();
					double dy = pLayer->envelop.centerY();

					((Map2DRenderer*)(g_pRenderer))->init(&g_Dataset, dx, dy, pLayer->envelop.width(), pLayer->envelop.height(), getWindowWidth(), getWindowHeight());
				}
				refreshWindow();
			}
		}
		
		break;
	}
}

///处理按键消息
void handleKeyMessage(int key)
{
	bool bCtrlPressed = isCtrlKeyPressed();//是否同时按下了Ctrl键
	bool bShiftPressed = isShiftKeyPressed();//是否同时按下了Shift键

	double pan = 10.0;

	switch (key)
	{
	case VK_UP: // 上一行，上光标			
		g_pRenderer->translate(0, pan);
	  refreshWindow();
		break;
	case VK_DOWN:
		g_pRenderer->translate(0, -pan);
	  refreshWindow();
		break;
	case VK_LEFT:
		if (isCtrlKeyPressed())
			g_pRenderer->rotate(90);
		else
			g_pRenderer->translate(-pan, 0);
	  refreshWindow();
		break;
	case VK_RIGHT:
		if (isCtrlKeyPressed())
			g_pRenderer->rotate(-90);
		else
			g_pRenderer->translate(pan, 0);
	  refreshWindow();
		break;
	}
}

///处理鼠标消息
void handleMouseMessage(UINT message, int x, int y, int det)
{
	DPtToLPt(x, y, x, y);//窗口设备坐标转为窗口逻辑坐标

	static bool dragging = false;
	static Point2D pt1;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		//if (g_OperationType == otPan)
		{
			pt1.x = x;
			pt1.y = y;
			dragging = true;
		}
		break;
	case WM_MOUSEMOVE:
		if (dragging)
		{
			if (pt1.x != x || pt1.y != y)
			{
				g_pRenderer->translate(x - pt1.x, y - pt1.y);
				pt1.x = x;
				pt1.y = y;
				refreshWindow();
			}
		}
		break;
	case WM_LBUTTONUP:
	{
		dragging = false;

		RubberMode rm = getRubberMode();
		if( rm == rmNone || rm == rmPolygon || rm == rmPolygon ) break;

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
				if (pt1.x == pt2.x && pt1.y == pt2.y) return ;

				if (g_OperationType == otDrawLine)
					g_pLayer->addGeometry(GeometryFactory::createPolylineGeometry(pt1, pt2));
				else
					g_pLayer->addGeometry(GeometryFactory::createCircleGeometry(pt1.x, pt1.y, pt2.x, pt2.y));
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
				if (x1 == x2 || y1 == y2) return ;

				if (g_OperationType == otDrawRectangle)
					g_pLayer->addGeometry(GeometryFactory::creatRectangleGeometry(x1, y1, x2, y2));
				else if (g_OperationType == otDrawRectangleOutline)
					g_pLayer->addGeometry(GeometryFactory::creatRectangleOutlineGeometry(x1, y1, x2, y2));
				else
					g_pLayer->addGeometry(GeometryFactory::createEllipseGeometry(x1, y1, x2, y2));
				refreshWindow();
			}
			break;
			}
		}		
	}
		break;
	case WM_RBUTTONUP:
	{
		dragging = false;

		RubberMode rm = getRubberMode();
		if (rm != rmPolyline && rm != rmPolygon) break;

		int c = getRubberPointCount();
		switch (g_OperationType)
		{
		case otDrawPolyline:
		{
			if (c < 2) return ;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			g_pLayer->addGeometry(GeometryFactory::createPolylineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}
		break;
		case otDrawPolygon:
		case otDrawPolygonOutline:
		{
			if (c < 3) return ;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			if (g_OperationType == otDrawPolygon)
				g_pLayer->addGeometry(GeometryFactory::createPolygonGeometry(pts.data(), pts.size()));
			else
				g_pLayer->addGeometry(GeometryFactory::createPolygonOutlineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}
		}
	}
		break;
	case WM_MOUSEWHEEL:
	{
		if (isCtrlKeyPressed())
		{
			x = 0;
			y = 0;
		}
		if (det > 0)
		{
			g_pRenderer->scale((double)x, (double)y, 0.9, 0.9);
		}
		else
		{
			g_pRenderer->scale(x, y, 1.0 / 0.9, 1.0 / 0.9);
		}
		refreshWindow();
		break;
	}
	}
}

//void drawDataset();

///处理绘制消息
void display()
{
	setYUp(true);//y轴向上
	setOrig(getWindowWidth() / 2, getWindowHeight() / 2);// 原点为窗口中心

	//if (g_State.DrawMode == State::dmGrid)
	//{
	//	Raster::drawGrid();
	//}
	//drawDataset();

	g_pRenderer->render();
}

//void drawLayer(Layer* pLayer);
//void drawDataset()
//{
//	for (int s = g_Dataset.getLayerCount(), i = s - 1; i >= 0; --i)
//	{
//		drawLayer(g_Dataset[i]);
//	}
//	//for (int i = 0; i < g_State.Seed_Points.size(); ++i)
//	//{
//	//	PixelPoint seed = g_State.Seed_Points[i];
//	//	Raster::FloodFill4(seed.x, seed.y, getPixel(seed.x, seed.y), getPenColor());
//	//}
//}
//
////————图层
//void drawLayer(Layer* pLayer)
//{
//	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
//	{
//		Geometry2D* pGeometryBase = (*pLayer)[i];
//		switch (pGeometryBase->getGeomType())
//		{
//		case gtPoint:
//		{
//			PointGeometry* pGeometry = (PointGeometry*)pGeometryBase;
//			//Raster::drawPoint( pGeometry->x, pGeometry->y );
//		}
//		break;
//		case gtPolyline:
//		{
//			PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryBase;
//			const vector<Point2D>& pts = pGeometry->getPts();
//			for (int i = 0, c = pts.size(); i < c - 1; ++i)
//			{
//				//Raster::drawLine(pts[i].x, pts[i].y, pts[(i + 1)].x, pts[(i + 1)].y, getPenColor());
//			}
//		}
//		break;
//		case gtPolygon:
//		{
//			PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryBase;
//			const vector<Point2D>& pts = pGeometry->getPts();
//			vector<PixelPoint> pts2(pts.size());
//			for (int i = 0, s = pts.size(); i < s; ++i)
//			{
//				pts2[i].x = pts[i].x;
//				pts2[i].y = pts[i].y;
//			}
//			//Raster::drawPolygon(pts2.data(), pts2.size(), getPenColor());
//		}
//		break;
//		case gtCircle:
//		{
//			CircleGeometry* pGeometry = (CircleGeometry*)pGeometryBase;
//			//Raster::drawCircle(pGeometry->x, pGeometry->y, pGeometry->r, getPenColor());
//		}
//		break;
//
//		case gtEllipse:
//		{
//			EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryBase;
//			double x, y;
//			pGeometry->getCenter(x, y);
//			//Raster::drawEllipses(x, y, pGeometry->getWidth(), pGeometry->getHeight(), getPenColor());
//		}
//		break;
//		}
//	}
//}

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
		initialize();
		return TRUE;
	case WM_DESTROY:
		destroy();
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

