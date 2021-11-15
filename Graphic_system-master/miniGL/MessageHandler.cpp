#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "Graphic.h"
#include <math.h>
#include "State.h"
#include "Raster.h"
#include <stdio.h>
#include <stdlib.h>  
#include <vector>
#include <iostream>
#include "Matrix.h"
#include "DialogHelper.h"
#include "GeoDefine.h"
#include "LayerImporter.h"
#include "State.h"
#include "Raster_byteacher.h"
#include "GeometryContainer.h"
#include "Matrix.h"
#include "Vec.h"
#include "HiddenRealColor.h"
using namespace std;


vector<void(*)()> pvec;


Dataset g_dataset; /*��ȡ��ͼ��������*/
GeometryContainer g_GeometryContainer; /*�Լ�����ͼ�ε�����*/
vector<StyledGeometry*>& container = g_GeometryContainer.mGeometryContainer;

Matrix g_TransformMatrix; //�Ի���ͼ�εı任����
Matrix g_MapMatrix; //��ͼ�ľ���


Vec3d TriangularPyramid[8] = {
	{0, 0, 0},
	{100, 0, 0},
	{100, 100, 0},
	{0, 100, 0},

	{0, 0, 100 },
	{100, 0,100},
	{100, 100, 100},
	{0, 100, 100} };
//Vec3d NewGraph3D[8];//��άͼ�� ������
//Vec3d eye(50, 50, 400),center(50,50,100),up(0, 1, 0);
Vec3d center(50, 50, 100), up(0, 1, 0);

Matrix4d ViewMatrix, ProjectionMatrix, VPMatrix , TransformMatrix; //�۲���� ͶӰ���� ���վ���


///������Ϣ����ڵ�
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://�˵���Ϣ
		return handleCommandMessage(hWnd, message, wParam, lParam);
		
	case WM_PAINT://������Ϣ
		return handlePaintMessage(hWnd, message, wParam, lParam);
		
	case WM_CREATE:
		init((unsigned)hWnd);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_KEYDOWN://������Ϣ
		return handleKeyMessage(hWnd, message, wParam, lParam);
	case WM_MOUSEMOVE://����ƶ���Ϣ
	case WM_LBUTTONDOWN://������������Ϣ	
	case WM_LBUTTONUP://������������Ϣ
	case WM_RBUTTONUP://����Ҽ�������Ϣ
	case WM_RBUTTONDOWN://����Ҽ�������Ϣ
	//case WM_LBUTTONDBLCLK://���˫����Ϣ
	case WM_MOUSEWHEEL://��������Ϣ
		return handleMouseMessage(hWnd, message, wParam, lParam);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	/*vec3d eye(1, 1, 1);
	vec3d center(3, 3, 3);
	vec3d z = eye - center;
	z.normalize();
	vec3d up(0, 1, 0);
	vec3d x = up ^ z;
	vec3d y = z ^ x;
	matrix4 viewMatrix;
	viewMatrix.makeViewMatrix(eye, center, up);

	double fovy, aspect, znear, zfar;
	matrix4 projectMatrix;
	projectMatrix.makePerspectiveMatrix(fovy, aspect, znear, zfar);

	vec4d pt;
	vec4d pt2 = pt * viewMatrix * projectMatrix;*/

	/*if (pt2.w > 0 && pt2.x >= -1 && pt2.x <= 1 && pt2.y >= -1 && pt2.x <= 1 && pt2.z >= 0 && pt2.z <= 1)
	{
		pt2 /= pt2.w;
		int x = (pt2.x * 0.5 + 0.5) * width;
		int y = (pt2.y * 0.5 + 0.5) * height;
		double z = pt2.z;*/
	/*}*/


	
}


//����Ĳ��ֵĴ�����Ҫ�ǵ������水ť�����á���Ҫע��setCursor(cs...)����
LRESULT  handleCommandMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	g_TransformMatrix.setmatrix();    //���ڱ任��������
	g_MapMatrix.setmatrix();
	
	int wmId = LOWORD(wParam);
	switch (wmId)
	{
	case ID_ADDLAYER:
	{
		char fullPath[1024] = { 0 };
		if (DialogHelper::selectSingleFile("", "shp", fullPath, 1024))
		{
			Layer* pLayer = LayerImporter::importShpLayer(fullPath);
			if (pLayer)
			{
				g_dataset.addLayer(pLayer);
				double dx = pLayer->envelop.centerX();
				double dy = pLayer->envelop.centerY();
				//g_TransformMatrix = g_TransformMatrix * Matrix::Translation(-dx, -dy);
				g_MapMatrix = g_MapMatrix * Matrix::Translation(-dx, -dy);
			}
		}
		refreshWindow();
	}
	break;
	case ID_MODE_PIXEL:
		g_State.DrawMode = State::dmPixel;
		g_State.GridWidth = g_State.GridHeight = 1;
		refreshWindow();
		return TRUE;
	case ID_MODE_GRID_10:
		g_State.DrawMode = State::dmGrid;
		g_State.GridWidth = g_State.GridHeight = 10;
		refreshWindow();
		return TRUE;
	case ID_MODE_GRID_4:
		g_State.DrawMode = State::dmGrid;
		g_State.GridWidth = g_State.GridHeight = 4;
		refreshWindow();
		return TRUE;
	case ID_MODE_GRID_2:
		g_State.DrawMode = State::dmGrid;
		g_State.GridWidth = g_State.GridHeight = 2;
		refreshWindow();
		return TRUE;
	case ID_2D_Grid:
		g_State.OperationType = State::otGridCell;
		setRubberMode(rmLine);
		setCursor(csArrow);
		return TRUE;
	case ID_2D_DRAW_RECT:
		g_State.OperationType = State::otDrawRectangle;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		refreshWindow();
		return TRUE;
	case ID_2D_LineDDA:
		g_State.OperationType = State::otDrawLineDDA;
		setRubberMode(rmLine);
		setCursor(csArrow);
		refreshWindow();
		return TRUE;
	case ID_2D_LineBresenham:
		g_State.OperationType = State::otDrawLine_Bresenham;
		setRubberMode(rmLine);
		setCursor(csArrow);
		refreshWindow();
		return TRUE;
	case ID_2D_DRAW_POLYLINE:
		g_State.OperationType = State::otDrawPolyline;
		setRubberMode(rmPolyline);
		setCursor(csArrow);
		refreshWindow();
		return TRUE;
	case ID_2D_DRAW_POLYGON:
		g_State.OperationType = State::otDrawPolygon;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_2D_Circle:
		g_State.OperationType = State::otDrawCircle;
		setRubberMode(rmLine);
		setCursor(csArrow);
		refreshWindow();
		return TRUE;
	case ID_2D_Ellipse:
		g_State.OperationType = State::otDrawEllipse;
		setRubberMode(rmLine);
		setCursor(csArrow);
		refreshWindow();
		return TRUE;
	case ID_2D_FillPolgon:
		g_State.OperationType = State::otFillPolygon;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_FillPolgon_Border:
		g_State.OperationType = State::otFillPolygon_Border;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_FenceFill:
		g_State.OperationType = State::otFillPolygon_FenceFill;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_Edge_TagMethod:
		g_State.OperationType = State::otFillPolygon_EdgeTag;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case otBoundary_fill:
		g_State.OperationType = State::otBoundary_SeedFill;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_SEED_FILL1:
		g_State.OperationType = State::StackSeedFill4;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_SEED_FILL2:
		g_State.OperationType = State::SeedLineFill1;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case ID_FLOOD_FILL:
		g_State.OperationType = State::otFloodFill;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		refreshWindow();
		return TRUE;
	case IDM_EXIT:
		g_State.OperationType = State::otRefreshWindow;	
		g_State.VisualMode = State::vm2DMap;
		if (DialogHelper::selectColor(g_State.PenColor, BLACK))
			refreshWindow();
		return TRUE;
	case ID_CHFONT:
		g_State.Font = State::orfont;
		if (DialogHelper::selectFont(g_State.FontType, g_State.FontSize))
			return TRUE;
		refreshWindow();
		return TRUE;
	case ID_SUTHERLAND:
		g_State.OperationType = State::otNone;
		g_State.SlipOperation = State::ocCohen_Sutherland;
		setCursor(csCross);
		setRubberMode(rmRectangle);
		refreshWindow();
		return TRUE;
	case ID_MIDPOINTDIVISION:
		g_State.OperationType = State::otNone;
		g_State.SlipOperation = State::ocMidpoint_Segmentation;
		setCursor(csCross);
		setRubberMode(rmRectangle);
		refreshWindow();
		return TRUE;
	case ID_LBLINECLIP:
		g_State.OperationType = State::otNone;
		g_State.SlipOperation = State::ocLiang_Barsky;
		setCursor(csCross);
		setRubberMode(rmRectangle);
		refreshWindow();
		return TRUE;
	case ID_HODGEMAN:
		g_State.OperationType = State::otNone;
		g_State.SlipOperation = State::ocSuth_Hodgeman;
		setCursor(csCross);
		setRubberMode(rmRectangle);		
		refreshWindow();
		return TRUE;
	case ID_3DPERSPECTIVE:
		g_State.VisualMode = State::vm3DBasic;
		g_State.BlankMethods = State::tZ_buffer;	
		Hiddens::initialization();//��Ļ��Χ��ʼ��
		refreshWindow();
		break;
	case ID_TSCANLINE3D:
		g_State.VisualMode = State::vm3DBasic;
		g_State.BlankMethods = State::tScanLine;
		refreshWindow();
		break;
	case ID_3DFRAME:
		g_State.VisualMode = State::vm3DBasic;
		g_State.BlankMethods = State::tFrameWork;
		refreshWindow();
		break;
	case ID_3DPINTER:
		g_State.VisualMode = State::vm3DBasic;
		g_State.BlankMethods = State::tPinter;
		refreshWindow();
		break;

	case ID_3DCONSTLIGHT:
		g_State.VisualMode = State::vm3DBasic;
		g_State.realColor = State::ConstantLight;
		refreshWindow();
		break;
	case ID_3DGOURAUD:
		g_State.VisualMode = State::vm3DBasic;
		g_State.realColor = State::Gouraud;
		refreshWindow();
		break;

	}
	return FALSE;
}


LRESULT  handleKeyMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int key = wParam;//��ð�������

	
	//��Ļ��Χ��ʼ��
	Hiddens::initialization();
	

	switch (key)
	{
	case VK_UP: // ��һ�У��Ϲ��
		switch (g_State.VisualMode)
		{
		case State::vm2DMap:
		case State::vm2DBasic:
			g_TransformMatrix = g_TransformMatrix * Matrix::Translation(0, 50); 
			g_MapMatrix = g_MapMatrix * Matrix::Translation(0, 1000); 
			break;
		case State::vm3DBasic:
			eye.y += 10;
			center.y += 10;

			if (isCtrlKeyPressed())
			{
				//Χ��X����ת
				TransformMatrix = TransformMatrix * Matrix4d::RotateAroundXaxis(PI / 3);
			}
			break;
		default:
			break;
		}		
		refreshWindow();
		return TRUE;
	case VK_DOWN:
		switch (g_State.VisualMode)
		{
		case State::vm2DMap:
		case State::vm2DBasic:
			g_TransformMatrix = g_TransformMatrix * Matrix::Translation(0, -50);  
			g_MapMatrix = g_MapMatrix * Matrix::Translation(0, -1000);  
			break;
		case State::vm3DBasic:
			eye.y -= 10;
			center.y -= 10;

			if (isCtrlKeyPressed())
			{
				//Χ��X����ת
				TransformMatrix = TransformMatrix * Matrix4d::RotateAroundXaxis(-PI / 3);
			}
			break;
		default:
			break;
		}
		refreshWindow();
		return TRUE;
	case VK_LEFT:
		if (isCtrlKeyPressed())
		{			
			switch (g_State.VisualMode)
			{
			case State::vm2DMap:
			case State::vm2DBasic:
				g_TransformMatrix = g_TransformMatrix * Matrix::RotationChange(90);
				g_MapMatrix = g_MapMatrix * Matrix::RotationChange(90);
				break;
			case State::vm3DBasic:
				//Χ��Z����ת
				TransformMatrix = TransformMatrix * Matrix4d::RotateAroundZaxis(-PI / 3);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (g_State.VisualMode)
			{
			case State::vm2DMap:
			case State::vm2DBasic:
				g_TransformMatrix = g_TransformMatrix * Matrix::Translation(-50, 0);  
				g_MapMatrix = g_MapMatrix * Matrix::Translation(-1000, 0); 
				break;
			case State::vm3DBasic:
				eye.x -= 10;
				center.x -= 10;
				break;
			default:
				break;
			}
		}
		refreshWindow();
		return TRUE;
	case VK_RIGHT:
		if (isCtrlKeyPressed())
		{
			switch (g_State.VisualMode)
			{
			case State::vm2DMap:
			case State::vm2DBasic:
				g_TransformMatrix = g_TransformMatrix * Matrix::RotationChange(-90);
				g_MapMatrix = g_MapMatrix * Matrix::RotationChange(-90);
				break;
			case State::vm3DBasic:
				//Χ��Z����ת
				TransformMatrix = TransformMatrix * Matrix4d::RotateAroundZaxis(PI / 3);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (g_State.VisualMode)
			{
			case State::vm2DMap:
			case State::vm2DBasic:
				g_TransformMatrix = g_TransformMatrix * Matrix::Translation(50, 0);
				g_MapMatrix = g_MapMatrix * Matrix::Translation(1000, 0);
				break;
			case State::vm3DBasic:
				eye.x += 10;
				center.x += 10;
				break;
			default:
				break;
			}
		}
		refreshWindow();
		return TRUE;
	case VK_NUMPAD0:
		g_TransformMatrix = g_TransformMatrix * Matrix::SymmetryChange(SymmetricOrigin);
		if (g_State.VisualMode == State::vm2DMap)
		{
			g_MapMatrix = g_MapMatrix * Matrix::SymmetryChange(SymmetricOrigin);
		}
		refreshWindow();
		return TRUE;
	case VK_NUMPAD1:
		g_TransformMatrix = g_TransformMatrix * Matrix::SymmetryChange(SymmetricX);
		if (g_State.VisualMode == State::vm2DMap)
		{
			g_MapMatrix = g_MapMatrix * Matrix::SymmetryChange(SymmetricX);
		}
		refreshWindow();
		return TRUE;
	case VK_NUMPAD2:
		g_TransformMatrix = g_TransformMatrix * Matrix::SymmetryChange(SymmetricY);
		if (g_State.VisualMode == State::vm2DMap)
		{
			g_MapMatrix = g_MapMatrix * Matrix::SymmetryChange(SymmetricY);
		}
		refreshWindow();
		return TRUE;
	case VK_NUMPAD3:
		g_TransformMatrix = g_TransformMatrix * Matrix::SymmetryChange(SymmetricXequalY);
		if (g_State.VisualMode == State::vm2DMap)
		{
			g_MapMatrix = g_MapMatrix * Matrix::SymmetryChange(SymmetricXequalY);
		}
		refreshWindow();
		return TRUE;
	case VK_NUMPAD4:
		g_TransformMatrix = g_TransformMatrix * Matrix::SymmetryChange(SymmetricXequa_Y);
		if (g_State.VisualMode == State::vm2DMap)
		{
			g_MapMatrix = g_MapMatrix * Matrix::SymmetryChange(SymmetricXequa_Y);
		}
		refreshWindow();
		return TRUE;
	case VK_NUMPAD5:
		g_TransformMatrix = g_TransformMatrix * Matrix::DisjointChange(50, 50);
		if (g_State.VisualMode == State::vm2DMap)
		{
			g_MapMatrix = g_MapMatrix * Matrix::DisjointChange(50, 50);
		}
		refreshWindow();
		return TRUE;
	}




	return FALSE;
}



LRESULT  handleMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	DPtToLPt(x, y, x, y);//�����豸����תΪ�����߼�����
	static bool dragging = FALSE;
	static bool pointRotateflag = false;
	static PixelPoint pt1;//����ƽ�Ʊ任�ĵ�

	static PixelPoint pt2;//������ת�任

	//��ά�任��Ļ��Χ��ʼ��
	Hiddens::initialization();

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		pt1.x = x;
		pt1.y = y;
		dragging = true;

		pointRotateflag = true; //������ת�ı任
		pt2.x = x;
		pt2.y = y;

	}
	return TRUE;
	case WM_MOUSEMOVE:
	{
		if (dragging && isCtrlKeyPressed())
		{
			setRubberMode(rmNone);
			if (pt1.x != x || pt1.y != y)
			{
				setCursor(csHand);
				g_TransformMatrix = g_TransformMatrix * Matrix::Translation((double)x - pt1.x, ((double)y - pt1.y));
				g_MapMatrix = g_MapMatrix * Matrix::Translation(((double)x - pt1.x) * 50, (((double)y - pt1.y)) * 50);
				pt1.x = x;
				pt1.y = y;
				refreshWindow();
			}
		}
		if (pointRotateflag && isShiftKeyPressed() && x != pt2.x )
		{
			setRubberMode(rmNone);
			setCursor(csHand);
			double alpha = atan((double)y - pt2.y / (double)x - pt2.x);
			g_TransformMatrix = Matrix::FixdePointToRotate((double)x, (double)y , alpha);
			g_MapMatrix = Matrix::FixdePointToRotate((double)x, (double)y, alpha);
		}
		if (pointRotateflag && isShiftKeyPressed() && x == pt2.x)
		{
			setRubberMode(rmNone);
			setCursor(csHand);
			g_TransformMatrix = Matrix::FixdePointToRotate90((double)x, (double)y);
			g_MapMatrix = Matrix::FixdePointToRotate90((double)x, (double)y);
		}
		refreshWindow();
	}
    return TRUE;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	{
		if (getRubberMode() == rmNone) return TRUE;
		if (message == WM_LBUTTONUP && (getRubberMode() == rmLine || getRubberMode() == rmRectangle))
			GeometryContainer::ToGetObject(g_GeometryContainer);  //��ȡ�Ի�ͼ�����꣬������g_GeometryContainer������
		if (message == WM_RBUTTONUP && getRubberMode() != rmLine)
			GeometryContainer::ToGetObject(g_GeometryContainer);  //��ȡ�Ի�ͼ�����꣬������g_GeometryContainer������
		if (dragging)dragging = FALSE;
		pointRotateflag = false;

		//���ֲü����㷨
		if (g_State.OperationType == State::otNone)
		{
			if (getRubberPointCount() < 2) return true;
			PixelPoint cPt1, cPt2; //����ü���
			getRubberPoints(cPt1, cPt2); //��ȡ�����ü������꣬���о��ο������ʾ
			switch (g_State.SlipOperation)
			{
			case State::ocCohen_Sutherland:
				Raster::Cohen_Sutherland(cPt1, cPt2,container); break;
			case State::ocMidpoint_Segmentation:
				Raster::midpointSubdivisionAlgorithm(cPt1, cPt2, container); break;
			case State::ocLiang_Barsky:
				Raster::LBLine(cPt1, cPt2, container); break;
			case State::ocSuth_Hodgeman:
				Raster::Sutherland_Hodgeman(cPt1, cPt2, container); break;
			}
		}	
		
		refreshWindow();	
	}
	return TRUE;
	case WM_MOUSEWHEEL:
	{
		int dx = x;
		int dy = y;
		short det = (short)HIWORD(wParam);
		if (det > 0 && isCtrlKeyPressed())
		{
			g_TransformMatrix = g_TransformMatrix * Matrix::FixdePointToScale(dx, dy, 2);
			g_MapMatrix = g_MapMatrix * Matrix::FixdePointToScale(dx, dy, 2);
		}
			
		else if (det < 0 && isCtrlKeyPressed())
		{
			g_TransformMatrix = g_TransformMatrix * Matrix::FixdePointToScale(dx, dy, 0.5);
			g_MapMatrix = g_MapMatrix * Matrix::FixdePointToScale(dx, dy, 0.5);
		}	
		else
		{
			if (det > 0)
			{
				g_TransformMatrix = g_TransformMatrix * Matrix::ScaleChange(2);
				g_MapMatrix = g_MapMatrix * Matrix::ScaleChange(2);				
			}
				
			if (det <= 0)
			{
				g_TransformMatrix = g_TransformMatrix * Matrix::ScaleChange(0.5);
				g_MapMatrix = g_MapMatrix * Matrix::ScaleChange(0.5);
			}				
		}

		if (g_State.VisualMode == State::vm3DBasic)
		{
			if (det > 0)
			{
				eye.z -= 10;
			}
			else
			{
				eye.z += 10;
			}
		}
		refreshWindow();
	}
	return TRUE;
	}
	return FALSE;
}


LRESULT  handlePaintMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	setYUp(true);//y������
	setOrig(getWindowWidth() / 2, getWindowHeight() / 2);
	
	if (g_dataset.getLayerCount() > 0)
	{
		Layer* pLayer = g_dataset[0];
		double pLayer_width = pLayer->envelop.width();
		double pLayer_height = pLayer->envelop.height();
		double Sx = getWindowWidth() / pLayer_width;
		double Sy = getWindowHeight() / pLayer_height;
		double S = Sx < Sy ? Sx : Sy;
		Matrix lastmatrix = g_MapMatrix * Matrix::ScaleChange(S);
		for (int k = 0; k < g_dataset.getLayerCount(); ++k)
		{
			Layer* pLayer = g_dataset[k];

			for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
			{
				Geometry* pGeometry = (*pLayer)[i];
				switch (pGeometry->getGeomType())
				{
				case gtPoint:
				{
					PointGemetry* pPointGeometry = (PointGemetry*)pGeometry;
					PixelPoint point;				
					point = Matrix_Multiply(vector2D(*pPointGeometry), lastmatrix);
					setPixel(point.x, point.y, BLACK);
				}
				break;
				case gtPolyline:
				{
					PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pGeometry;
					const vector<Point2D>& pts = pPolylineGeometry->getPts();					
					PixelPoint* points = (PixelPoint*)calloc(pts.size(), sizeof(PixelPoint));
					for (int j = 0, size_p = pts.size(); j < size_p; ++j)
					{
						points[j] = Matrix_Multiply(vector2D(pts[j]), lastmatrix);
					}
					Raster::Bresenham_draw_Polyline(points, pts.size(), getPenColor()); 
					free(points);
				}
				break;
				case gtPolygon:
				{
					PolygonGeometry* pPolygonGeometry = (PolygonGeometry*)pGeometry;
					const vector<Point2D>& pts = pPolygonGeometry->getPts();
					PixelPoint* points = (PixelPoint*)calloc(pts.size(), sizeof(PixelPoint));;
					for (int j = 0, size_p = pts.size(); j < size_p; ++j)
					{
						points[j] = Matrix_Multiply(vector2D(pts[j]), lastmatrix);
					}
					Raster::Edge_Fill(points, pts.size() - 1, getPenColor());					
					free(points);
				}
				break;
				}
			}
		}
	}

	if (g_State.Font == State::orfont)
	{
		//g_fontSize = getFontSize();
		//g_fontName = getFontName();
		Raster_byteacher::textOut("�����е�ͼ", -50, getWindowHeight() / 2 - 50, RED);
	}
	
	
	for (int i = 0, size = container.size() ; i < size; ++i)
	{
		StyledGeometry* pStyledGeometry = container[i];
		switch (pStyledGeometry->pGeometry->getGeomType())
		{
		case gtPoint:
		{
			ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
			PointGemetry* pPolypointGeometry = (PointGemetry*)pStyledGeometry->pGeometry;
			int x = pPolypointGeometry->x;
			int y = pPolypointGeometry->y;
			PixelPoint point;
			point.x = x; point.y = y;
		}
		break;
		case gtPolyline:
		{
			ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
			PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pStyledGeometry->pGeometry;
			
			const vector<Point2D>& pts = pPolylineGeometry->getPts();
			vector<PixelPoint> Points(pts.size());
			for (int i = 0; i < pts.size(); i++)
				Points[i] = Matrix_Multiply(vector2D(pts[i]), g_TransformMatrix); //������б任
			switch (pStyle->drawMethod)
			{
			case otDrawRectangle:
				Raster::draw_Rectangle(Points[0], Points[1], pStyle->color); break;
			case otDrawLineDDA:
				Raster::DDA_draw_Line(Points[0], Points[1], pStyle->color); break;
			case otDrawLine_Bresenham:
				Raster::Bresenham_draw_Line(Points[0], Points[1], pStyle->color); break;
			case otDrawCircle:
				Raster::Bresenham_draw_Circle(Points[0], Points[1], pStyle->color); break;
			case otDrawEllipse:
				Raster::Bresenham_draw_ellipse(Points[0], Points[1], pStyle->color); break;
			case otDrawPolyline:
				Raster::Bresenham_draw_Polyline(Points.data(), pts.size(), pStyle->color); break;
			}
		}
		break;
		case gtPolygon:
		{
			ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
			PolygonGeometry* pPolygonGeometry = (PolygonGeometry*)pStyledGeometry->pGeometry;
			
			const vector<Point2D>& pts = pPolygonGeometry->getPts();
			PixelPoint* Points = (PixelPoint*)calloc(pts.size(), sizeof(PixelPoint));
			for (int i = 0; i < pts.size(); i++)
				Points[i] = Matrix_Multiply(vector2D(pts[i]), g_TransformMatrix); //������б任
			switch (pStyle->drawMethod)
			{
			case otDrawPolygon://����λ���  //�޸�
				Raster::Bresenham_draw_Polyline(Points, pts.size(), pStyle->color); break;
			case otFillPolygon://���Ա߱�
				//setOrig(0, getWindowHeight()); //��Ϊ������Ļ�ǽ�ԭ�����������ĵģ�������Ҫ��������ԭ��
				Raster::ActiveEdge_Polygon(Points, pts.size(), pStyle->color); break;
			case otFillPolygon_Border://��Ե���
				Raster::Edge_Fill(Points, pts.size() - 1, pStyle->color); break;			
			case otFillPolygon_FenceFill://դ�����
				Raster::Fence_Fill(Points, pts.size() - 1, pStyle->color); break;				
			case otFillPolygon_EdgeTag://�߱�־�����
				Raster::Raster::Edge_TagMethod(Points, pts.size() - 1, pStyle->color); break;
			}

			free(Points); //����̬���ٵ�Points�Ŀռ��ͷ�
			refreshWindow();
		}
		break;
		case gtSeedFill:
		{
			ColorStyle* pStyle = (ColorStyle*)pStyledGeometry->pStyle;
			SeedFillGeometry* pPolygonGeometry = (SeedFillGeometry*)pStyledGeometry->pGeometry;
			PixelPoint SeedPoint;
			SeedPoint.x = pPolygonGeometry->x;
			SeedPoint.y = pPolygonGeometry->y;
			switch (pStyle->drawMethod)
			{
			case otBoundary_SeedFill: //���õݹ�ķ�ʽ�������
				Raster::Boundary_Fill(SeedPoint.x, SeedPoint.y, BLACK, pStyle->color); break;			
			case StackSeedFill4: //����ѹջ�ķ�ʽ���б߽����
				Raster::SeedBoundaryFill4(SeedPoint, BLACK, pStyle->color); break;
			case SeedLineFill1:  //����ɨ���ߵķ�ʽ�������
				Raster::SeedBoundaryFill4_Line(SeedPoint, BLACK, pStyle->color); break;
			case otFloodFill: //�����
				Raster::FloodFill4(SeedPoint, getPixel(SeedPoint.x, SeedPoint.y), pStyle->color); break;
			}
		}
		break;
		}
	}	
	
	if (g_State.VisualMode == State::vm3DBasic)
	{
		Vec3d z = eye - center;
		ViewMatrix.MakeViewMatrix(eye, center, up);
		//znear = 50,zfar = 500
		ProjectionMatrix.MakePerspectiveMatrix(PI / 3,  getWindowWidth()/ getWindowHeight(), 100, 600);
		VPMatrix = TransformMatrix * ViewMatrix * ProjectionMatrix;
		Matrix4d::GetCoordinates3D(TriangularPyramid, VPMatrix, NewGraph3D);  //NewGraph3DΪ����ͼ��
		
		//������άͼ�οռ��м������ĵ�����
		Vec4d m = Matrix4d::VecMultiplyMatrix(Vec4d(Ocenter), VPMatrix);
		double w = m.h;
		if (w > 0 && m.x >= -w && m.x <= w && m.y >= -w && m.x <= w && m.z >= 0 && m.z <= w)
		{
			m.HomogeneousConvert();
			Ocenter.x = (m.x * 0.5 + 0.5) * getWindowWidth() / 2;
			Ocenter.y = (m.y * 0.5 + 0.5) * getWindowHeight() / 2;
			Ocenter.z = m.z * getWindowHeight() / 2;
		}

		vector<vector<Color>> ColorPixels;  //һ��������е����Ϣ
		//��������ĵ㼯ת��Ϊ�ռ��е�������

		Hiddens::ptsConvertToFace(NewGraph3D, CubeFace);

		
		switch (g_State.BlankMethods)
		{
		case State::tNone:
			break;
		case State::tFrameWork:
			Raster::Draw3DGraph(NewGraph3D);
			break;
		case State::tZ_buffer:
			for (int i = 0; i < 12; ++i)
				Hiddens::Z_Buffer(CubeFace[i], i);  //����z_buffer�㷨ת��
			Hiddens::drawPixels();
			break;
		case State::tScanLine:
			//����ɨ�����㷨			
			Hiddens::buildtable(CubeFace);
			Hiddens::scanLine(ColorPixels);
			Hiddens::draw(ColorPixels);
			break;
		case State::tPinter:
			//Hiddens::DepthSortingMethod(CubeFace);
			break;
		default:
			break;
		}

		//��ʵ����ɫ����
		switch (g_State.realColor)
		{
		case State::tFrameWork:
			RealisticColor::ConstLightIntensity(CubeFace);
			break;
		case State::Gouraud:
			RealisticColor::GroundRealColor(CubeFace);
			break;
		default:
			break;
		}
	}
	
	return TRUE;
}

