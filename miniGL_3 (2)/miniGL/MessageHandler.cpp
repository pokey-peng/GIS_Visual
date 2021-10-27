#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "Graphic.h"
#include <vector>
#include "GeoDefine.h"
#include "GeometryFactory.h"
#include "Raster.h"

using namespace std;

enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline, otDrawCircle, otDrawEllipse, 
	otDrawTriangle,otDrawTriangleOutline
};

OperationType g_OperationType = otNone;//��ǰ��������
Dataset g_Dataset;
Layer* g_pLayer = NULL;



///����˵���Ϣ
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
	case ID_2D_DRAW_POLYGON_OUTLINE:
		g_OperationType = otDrawPolygonOutline;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_TRIANGLE_Outline:
		g_OperationType = otDrawTriangleOutline;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_DRAW_TRIANGLE:
		g_OperationType = otDrawTriangle;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_CIRCLE_OUTLINE:
		g_OperationType = otDrawCircle;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	case ID_2D_DRAW_ELLIPSE_OUTLINE:
		g_OperationType = otDrawEllipse;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
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
					if (pt1.x == pt2.x && pt1.y == pt2.y) return;

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
					if (x1 == x2 || y1 == y2) return;

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

			g_pLayer->addGeometry(GeometryFactory::createPolylineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}
		break;
		
		case otDrawTriangle:
		case otDrawTriangleOutline:
		{
			if (c < 3) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			if (g_OperationType == otDrawTriangle)
				g_pLayer->addGeometry(GeometryFactory::createPolygonGeometry(pts.data(), 3));
			else
				g_pLayer->addGeometry(GeometryFactory::createPolygonOutlineGeometry(pts.data(), 3));
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
		break;		
		}
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

void drawDataset();

///���������Ϣ
void display()
{
	setYUp(true);//y������
	setOrig(getWindowWidth() / 2, getWindowHeight() / 2);// ԭ��Ϊ��������

	drawDataset();

}

void drawLayer(Layer* pLayer);
void drawDataset()
{
	for (int s = g_Dataset.getLayerCount(), i = s - 1; i >= 0; --i)
	{
		drawLayer(g_Dataset[i]);
	}
	//for (int i = 0; i < g_State.Seed_Points.size(); ++i)
	//{
	//	PixelPoint seed = g_State.Seed_Points[i];
	//	Raster::FloodFill4(seed.x, seed.y, getPixel(seed.x, seed.y), getPenColor());
	//}
}

//��������ͼ��
void drawLayer(Layer* pLayer)
{
	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
	{
		Geometry2D* pGeometryBase = (*pLayer)[i];
		switch (pGeometryBase->getGeomType())
		{
		case gtPoint:
		{
			PointGeometry* pGeometry = (PointGeometry*)pGeometryBase;

			//-----------------------����-----------------------
			Raster::drawPoint( pGeometry->x, pGeometry->y );
			//--------------------------------------------------

		}
		break;
		case gtPolyline:
		{
			PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryBase;
			const vector<Point2D>& pts = pGeometry->getPts();
			for (int i = 0, c = pts.size(); i < c - 1; ++i)
			{

				//----------------------------------------����--------------------------------------
				Raster::drawLine(pts[i].x, pts[i].y, pts[(i + 1)].x, pts[(i + 1)].y, getPenColor());
				//----------------------------------------------------------------------------------

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
			switch (g_OperationType) {
			case otDrawTriangleOutline:Raster::drawTriangleOutline(pts2.data(), getPenColor());break;
			case otDrawTriangle: Raster::drawTriangle(pts2.data(), getPenColor());break;
			case otDrawPolygonOutline:Raster::drawPolygonOutline(pts2.data(), pts2.size(), getPenColor());break;
			case otDrawPolygon:break;
			}
		}
		break;

		case gtCircle:
		{
			CircleGeometry* pGeometry = (CircleGeometry*)pGeometryBase;

			//-------------------------------��Բ----------------------------------------
			Raster::drawCircleOutline(pGeometry->x, pGeometry->y, pGeometry->r, getPenColor());
			//---------------------------------------------------------------------------
		}
		break;

		case gtEllipse:
		{
			EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryBase;
			double x, y;
			pGeometry->getCenter(x, y);

			//----------------------------------����Բ-----------------------------------------------
			Raster::drawEllipses(x, y, pGeometry->getWidth(), pGeometry->getHeight(), getPenColor());
			//---------------------------------------------------------------------------------------
		}
		break;
		
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
		g_pLayer = new Layer();
		g_Dataset.addLayer(g_pLayer);
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

