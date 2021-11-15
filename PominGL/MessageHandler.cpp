#include <windows.h>
#include "MessageHandler.h"
#include "Resource.h"
#include "Graphic.h"
#include "MyGraphic.h"
#include "MyGeoDefine.h"
#include "MyGeometryLibrary.h"
#include "MyRaster.h"
#include "DrawState.h"
using namespace std;
//enum OperationType {
//	otNone, otDrawRectangle, otDrawRectangleOutline,
//	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline,
//	otDrawCircle, otDrawEllipse,
//	//���ػ�ͼ
//	otPixel, ot10Network, ot20Network,
//	// ���
//	otAreaFill
//
//};
//
//OperationType g_OperationType = otNone; // ��ǰ��������
boolean Reset = false;
// ����һ�����ݼ������ͼ��
Dataset g_Dataset;

Color g_Color = RED;
int g_PointCout;

PixelPoint p1[2];

// ��ȡ��ǰģʽ
RubberMode rm;
PixelPoint g_Points[100];

// ����һ��ͼ��ָ��
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
///����˵���Ϣ
void handleMenuMessage(int menuID)
{
	switch (menuID)
	{
	case ID_DRAWMODE_GRID_10:
	{
		g_State.DrawMode = dmGrid;
		g_State.drawPixelCB = Raster::drawCell;
		refreshWindow();
		break;
	}

	case ID_DRAWMODE_PIXEL:
	{
		g_Color = RED;
		g_State.g_OperationType = otPixel;
		setRubberMode(rmNone);
		
		refreshWindow();
		break;
	}

	case ID_DRAWMODE_GRID_20:
	{
		g_State.DrawMode = dmGrid;
		g_State.GridHeight = 20;
		g_State.GridWidth = 20;
		g_State.drawPixelCB = Raster::drawCell;
		refreshWindow();
		break;
	}
	case ID_DRAWMODEL_GEOMETRY:
	{
		g_State.DrawMode = dmGemotry;
		g_State.drawPixelCB = setPixel;
		refreshWindow();
		break;
	}
	case ID_2D__RECTANGLE_OUTLINE:
	{
		setCursor(csSize);
		setRubberMode(rmRectangle);
		g_State.g_OperationType = otDrawRectangleOutline;
		refreshWindow();
		break;
	}
	case ID_2D_LINE:
	{
		setCursor(csArrow);
		setRubberMode(rmLine);
		g_State.g_OperationType = otDrawLine;
		//refreshWindow();
		break;
	}
	case ID_2D_POLYLINE:
	{
		setCursor(csHand);
		setRubberMode(rmPolyline);
		g_State.g_OperationType = otDrawPolyline;
		refreshWindow();
		break;
	}
	case ID_2D_POLYGON_OUTLINE:
	{
		setCursor(csCross);
		setRubberMode(rmPolygon);
		g_State.g_OperationType = otDrawPolygonOutline;
		break;
	}
	case ID_2D_POLYGON:
	{
		setPenColor(BLUE);
		setCursor(csCross);
		setRubberMode(rmPolygon);
		g_State.g_OperationType = otDrawPolygon;
		//refreshWindow();
		break;
	}
	case ID_2D_CIRCLE_OUTLINE:
	{
		setCursor(csHand);
		setRubberMode(rmCircle);
		g_State.g_OperationType = otDrawCircle;
		break;
	}
	case ID_2D_ELLIPSE_OUTLINE:
	{
		//setPenColor(BLUE);
		setCursor(csCross);
		setRubberMode(rmEllipse);
		g_State.g_OperationType = otDrawEllipse;
		break;
	}
	case ID_AREA_FILL:
	{
		g_State.g_OperationType = otAreaFill;
		setRubberMode(rmLine);
		setCursor(csArrow);
		//setPenColor(YELLOW);
		break;
	}
	case ID_RESET:
	{
		// ��յ�ǰ���������һ���µ�ͼ��
		Reset = true;
		if (g_player->EmptyGeometry()) {} // ����ǰͼ��Ϊ���������ͼ��
		else
		{
			g_player = new Layer();
			g_Dataset.addLayer(g_player);
		}
		refreshWindow();
		break;
	}

	case ID_COLOR_RED:
	{
		setPenColor(RED);
		break;
	}
	case ID_COLOR_GREEN:
		setPenColor(GREEN);
		break;
	case ID_COLOR_BLUE:
		setPenColor(BLUE);
		break;

	case IDM_EXIT:
		PostQuitMessage(0);
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
	case 'R':
	{
		g_Color = BLUE;
		refreshWindow();
		break;
	}
		
		
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
		int c = getRubberPointCount();
		if (g_State.g_OperationType == otAreaFill && c==2)
		{
			
			getRubberPoints(p1);
			//PointFill(p1[0].x, p1[0].y, YELLOW);
			refreshWindow();
			break;
		}
		
		rm = getRubberMode();
		//if (rm == rmNone || rm == rmPolygon || rm == rmPolygon) break;
		if (rm == rmNone) break;
		
		if (c == 2)
		{
			PixelPoint pt1, pt2;
			getRubberPoints(pt1, pt2);

			switch (g_State.g_OperationType)
			{
			case otDrawLine:
			{
				if (pt1.x == pt2.x && pt1.y == pt2.y) return;
				g_player->addGeometry(GeometryLibrary::createPolylineGeometry(pt1, pt2,getPenColor()));
				refreshWindow();
				break;
			}
			case otDrawCircle:
			{
				if (pt1.x == pt2.x && pt1.y == pt2.y) return;

				/*if (g_g_OperationType == otDrawLine)
					g_player->addGeometry(GeometryLibrary::createPolylineGeometry(pt1, pt2,getPenColor()));
				else*/
				g_player->addGeometry(GeometryLibrary::createCircleGeometry(pt1.x, pt1.y, pt2.x, pt2.y,getPenColor()));
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

				if (g_State.g_OperationType == otDrawRectangle)
					g_player->addGeometry(GeometryLibrary::creatRectangleGeometry(x1, y1, x2, y2));
				else if (g_State.g_OperationType == otDrawRectangleOutline)
					g_player->addGeometry(GeometryLibrary::creatRectangleOutlineGeometry(x1, y1, x2, y2));
				else
					g_player->addGeometry(GeometryLibrary::createEllipseGeometry(x1, y1, x2, y2,getPenColor()));
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
		switch (g_State.g_OperationType)
		{
		case otDrawPolyline:
		{
			if (c < 2) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			g_player->addGeometry(GeometryLibrary::createPolylineGeometry(pts.data(), pts.size(),getPenColor()));

			refreshWindow();
		}
		break;
		case otDrawPolygon:
		{
			if (c < 3) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());
			g_player->addGeometry(GeometryLibrary::createPolygonGeometry(pts.data(), pts.size()));
			refreshWindow();
		}
		case otDrawPolygonOutline:
		{
			if (c < 3) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());
			g_player->addGeometry(GeometryLibrary::createPolygonOutlineGeometry(pts.data(), pts.size(),getPenColor()));

			refreshWindow();
		}
		break;
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
	for (int row = irow; row < irow + h; ++row)//��ָ����ɫ���ƾ���
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
	if (g_State.DrawMode == dmGrid)
	{
		Raster::drawGrid();
	}
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
			if (g_State.DrawMode == dmGrid)
			{
				vector<PixelPoint> pt(pts.size());
				for (int i = 0, s = pts.size(); i < s; ++i)
				{
					pt[i].x = pts[i].x;
					pt[i].y = pts[i].y;
				}
				Raster::ChangeOf_XYs(pt.data(), pt.size());
				for (int i = 0, c = pt.size(); i < c - 1; ++i)
				{
					Raster::drawLine(pt[i].x, pt[i].y, pt[(i + 1)].x, pt[(i + 1)].y, pGeometry->color);
				}
			}
			else
			{
				for (int i = 0, c = pts.size(); i < c - 1; ++i)
				{
					Raster::drawLine(pts[i].x, pts[i].y, pts[(i + 1)].x, pts[(i + 1)].y, pGeometry->color);
				}
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
			Raster::drawPolygon(pts2.data(), pts2.size(), pGeometry->color);
		}
		break;
		case gtCircle:
		{
			CircleGeometry* pGeometry = (CircleGeometry*)pGeometryBase;
			Raster::drawCircle(pGeometry->x, pGeometry->y, pGeometry->r, pGeometry->color);
		}
		break;

		case gtEllipse:
		{
			EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryBase;
			double x, y;
			pGeometry->getCenter(x, y);
			Raster::drawEllipse(x, y, pGeometry->getWidth(), pGeometry->getHeight(),pGeometry->color);
		}
		break;
		}
	}
}
///���������Ϣ
void display()
{
	
	int w = getWindowWidth();//1423
	int h = getWindowHeight();//730

	setYUp(true);//y������
	setOrig(0, h);// ԭ��Ϊ�������½�
	rm = getRubberMode();
	if (!Reset)
	{
		
		drawDataset();
		if (g_State.g_OperationType == otAreaFill)
		{
			//PointFill(p1[0].x, p1[0].y, YELLOW);
			//BoundaryFill(p1[0].x, p1[0].y, BLACK, YELLOW);
			ScanLineAreaFill(p1[0].x, p1[0].y, BLACK, RED);
		}
	}
	else
	{
		Reset = false;
		return;
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
		g_player = new Layer();
		g_Dataset.addLayer(g_player);
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

