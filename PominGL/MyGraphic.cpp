#include <windows.h>
#include <windowsx.h>
#include "MyGraphic.h"
#include "Graphic.h"
#include <vector>
#include "DrawState.h"
#include <algorithm>
#include <stack>
//using namespace State;
using namespace std;
extern Color g_Color; // �߶���ɫ
extern PixelPoint g_Points[100]; // �����Ҫ���Ƶĵ������
extern int g_PointCout;  //��ĸ���

HDC g_HDC = 0;

class RubberPad
{
public:
	POINT pt1;
	vector<POINT> pts;
	Color penColor;
	Color oldPenColor;
	int oldROP2Mode;
	RubberMode rubberMode;
	bool startDrawing;
	bool lineToFirst;

	RubberPad()
	{
		penColor = YELLOW;
		rubberMode = rmNone;
		startDrawing = false;
		lineToFirst = true;
	}

	void mouseDown(UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		//if (wMsg == WM_LBUTTONDOWN)
		//{
		//	if (rubberMode == rmNone) return;

		//	oldROP2Mode = SetROP2(g_hDC, R2_NOT);
		//	SelectObject(g_hDC, GetStockObject(DC_PEN));
		//	oldPenColor = SetDCPenColor(g_hDC, penColor);

		//	pt1.x = LOWORD(lParam);
		//	pt1.y = HIWORD(lParam);

		//	pt2 = pt1;
		//	startDrawing = true;
		//}
	}

	void mouseMove(UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		/*if (rubberMode == rmNone) return;

		if (startDrawing)
		{
			drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);

			if (rubberMode == rmPolygon && pts.size() >= 2)
			{
				if (lineToFirst)
					drawRubberline(g_HDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
				else
					lineToFirst = true;
			}

			pt2.x = LOWORD(lParam);
			pt2.y = HIWORD(lParam);

			drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);

			if (rubberMode == rmPolygon && pts.size() >= 2)
			{
				drawRubberline(g_HDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
			}
		}*/

	}

	void mouseUp(UINT wMsg, WPARAM wParam, LPARAM lParam)
	{
		//if (wMsg == WM_LBUTTONUP)
		//{
		//	if (rubberMode == rmNone) return;

		//	if (!startDrawing)
		//	{
		//		oldROP2Mode = SetROP2(g_HDC, R2_NOT);
		//		SelectObject(g_HDC, GetStockObject(DC_PEN));
		//		oldPenColor = SetDCPenColor(g_HDC, penColor);

		//		pt1.x = LOWORD(lParam);
		//		pt1.y = HIWORD(lParam);

		//		pt2 = pt1;
		//		pts.clear();

		//		startDrawing = true;
		//	}
		//	else
		//	{
		//		if (rubberMode == rmLine || rubberMode == rmRectangle)
		//		{
		//			drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);

		//			pts.push_back(pt2);

		//			DPtToLPt(pts[0].x, pts[0].y, (int&)pts[0].x, (int&)pts[0].y);
		//			DPtToLPt(pts[1].x, pts[1].y, (int&)pts[1].x, (int&)pts[1].y);
		//			//DPtToLPt( pt1.x, pt1.y, (int&)pt1.x, (int&)pt1.y );
		//			//DPtToLPt( pt2.x, pt2.y, (int&)pt2.x, (int&)pt2.y );

		//			SelectObject(g_HDC, GetStockObject(DC_PEN));
		//			oldPenColor = SetDCPenColor(g_HDC, penColor);
		//			SetROP2(g_HDC, oldROP2Mode);
		//			startDrawing = false;

		//			return;
		//		}
		//		else
		//		{
		//			if (rubberMode == rmPolygon && pts.size() == 1)
		//			{
		//				drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);//����
		//				lineToFirst = false;//�����ģʽ�·�ֹ��һ���߱�����
		//			}

		//			pt2.x = LOWORD(lParam);
		//			pt2.y = HIWORD(lParam);

		//			if (rubberMode == rmPolygon && pts.size() == 1)
		//			{
		//				drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);
		//			}

		//			pt1 = pt2;
		//		}
		//	}

		//	pts.push_back(pt1);

		//}
		//else if (wMsg == WM_RBUTTONUP)
		//{
		//	if (startDrawing == false) return;

		//	if (rubberMode == rmPolyline || rubberMode == rmPolygon)
		//	{
		//		drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);	//����P1P2

		//		if (rubberMode == rmPolygon && pts.size() >= 2) // >= 2 --> P2 ������������
		//		{
		//			if (lineToFirst)
		//				drawRubberline(g_HDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
		//			else
		//				lineToFirst = true;	 //����P0P2
		//		}

		//		for (int i = 0; i < (int)pts.size() - 1; ++i)
		//		{
		//			drawRubberline(g_HDC, pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y);
		//		}

		//		for (int i = 0; i < (int)pts.size(); ++i)
		//		{
		//			DPtToLPt(pts[i].x, pts[i].y, (int&)pts[i].x, (int&)pts[i].y);
		//		}

		//		SelectObject(g_HDC, GetStockObject(DC_PEN));
		//		oldPenColor = SetDCPenColor(g_HDC, penColor);
		//		SetROP2(g_HDC, oldROP2Mode);
		//		startDrawing = false;
		//	}
		//}
	}

	/*void drawRubberline(HDC hdc, int x0, int y0, int x1, int y1)
	{

		if (rubberMode != rmRectangle)
		{
			drawLine(hdc, x0, y0, x1, y1);
		}
		else
		{
			drawRectangle(hdc, x0, y0, x1, y1);
		}
	}

	void drawLine(HDC hdc, int x0, int y0, int x1, int y1)
	{
#ifdef DIRECT_DRAW
		DPtToLPt(x0, y0, x0, y0);
		DPtToLPt(x1, y1, x1, y1);
#endif
		MoveToEx(hdc, x0, y0, NULL);
		LineTo(hdc, x1, y1);
	}

	void drawRectangle(HDC hdc, int x0, int y0, int x1, int y1)
	{
#ifdef DIRECT_DRAW
		DPtToLPt(x0, y0, x0, y0);
		DPtToLPt(x1, y1, x1, y1);
#endif
		MoveToEx(hdc, x0, y0, NULL);
		LineTo(hdc, x1, y0);
		LineTo(hdc, x1, y1);
		LineTo(hdc, x0, y1);
		LineTo(hdc, x0, y0);
	}*/
};


int sign(int val)
{
	if (val > 0)
		return 1;
	else if (val < 0)
		return -1;
	else
		return 0;
}
/// <summary>
/// Bresenham�е��㷨
/// </summary>
/// <param name="startp">��ʼ�����������</param>
/// <param name="endp">�����յ�����</param>
void MidPointLine(double x0, double y0, double x1, double y1, unsigned color)
{
	/*
	int dex = abs(startp.x - endp.x);
	int dey = abs(startp.y - endp.y);
	bool tag = false;
	// ��ֱ��б�ʴ���1ʱ
	if (dex < dey)
	{
		std::swap(dex, dey);
		tag = true;
	}
	// x,y����
	int dx = sign(endp.x - startp.x); int dy = sign(endp.y - startp.y);
	int d = dex - dey - dey; // ��ʼ����
	int x = startp.x; int y = startp.y; // ���
	for (int i = 0; i <= dex; ++i)
	{
		setPixel(x, y, g_Color);
		if (d < 0) // d<0ʱ��x,y�ֱ���ϼ�dx,dy
		{
			x += dx; y += dy;
			d += (dex + dex - dey - dey); // ��������2dex-2dey
		}
		else
		{
			// d>0��б�ʴ���1ʱ��y����
			if (tag == true)
				y += dy;
			else // d>0��б�ʲ�����1ʱ��x����
				x += dx;
			d -= (dey + dey); // ��������-2dey
		}
	}
	*/
	int dex = abs(x0 - x1);
	int dey = abs(y0 - y1);
	bool tag = false;
	// ��ֱ��б�ʴ���1ʱ
	if (dex < dey)
	{
		std::swap(dex, dey);
		tag = true;
	}
	// x,y����
	int dx = sign(x1 - x0); int dy = sign(y1 - y0);
	int d = dex - dey - dey; // ��ʼ����
	int x = x0; int y = y0; // ���
	for (int i = 0; i <= dex; ++i)
	{
		setPixel(x, y, color);
		if (d < 0) // d<0ʱ��x,y�ֱ���ϼ�dx,dy
		{
			x += dx; y += dy;
			d += (dex + dex - dey - dey); // ��������2dex-2dey
		}
		else
		{
			// d>0��б�ʴ���1ʱ��y����
			if (tag == true)
				y += dy;
			else // d>0��б�ʲ�����1ʱ��x����
				x += dx;
			d -= (dey + dey); // ��������-2dey
		}
	}

}
/// <summary>
/// DDAֱ���㷨
/// </summary>
/// <param name="startp">��ʼ�����������</param>
/// <param name="endp">�����յ�����</param>
void DDALine(PixelPoint startp, PixelPoint endp)
{
	int x = 0, dex, dey, dexx, deyy;
	float dx, dy, xp, yp;
	dexx = endp.x - startp.x;
	deyy = endp.y - startp.y;
	dex = abs(dexx);
	dey = abs(deyy);
	dx = (float)dexx / max(dex, dey);
	dy = (float)deyy / max(dex, dey);
	xp = startp.x;
	yp = startp.y;
	if (dex < dey)
	{
		std::swap(dex, dey);
	}
	while (x < dex)
	{
		setPixel(round(xp), round(yp), g_Color);
		xp += dx;
		yp += dy;
		x++;
	}

}

/* ��������*/
void DrawPolyline()
{
	// ��ʼ����
	int startp = 0; int endp = 1;
	// ���������д��ڵĵ�
	while (endp < g_PointCout)
	{
		// ÿ������֮�仭һ���߶�
		//MidPointLine(g_Points[startp], g_Points[endp]);
		DDALine(g_Points[startp], g_Points[endp]);
		startp++;
		endp++;
	}
}
/* �����ľ���*/
void DrawRectangle()
{
	int x0 = g_Points[0].x, y0 = g_Points[0].y;
	int x1 = g_Points[1].x, y1 = g_Points[1].y;
	int xmin = min(x0, x1), xmax = max(x0, x1);
	int ymin = min(y0, y1), ymax = max(y0, y1);
	for (int j = ymin; j <= ymax; ++j)
	{
		setPixel(xmin, j, g_Color);
		setPixel(xmax, j, g_Color);
	}
	for (int i = xmin; i <= xmax; ++i)//��ָ����ɫ���ƿ��ľ���
	{
		setPixel(i, ymin, g_Color);
		setPixel(i, ymax, g_Color);
	}
}
/* �����Ķ����*/
void DrawPolygon()
{
	/*
	// �Ȱ������еĵ㰴˳�����λ����߶�
	int startp = 0; int endp = 1;

	while (endp < g_PointCout)
	{
		MidPointLine(g_Points[startp], g_Points[endp]);
		startp++;
		endp++;
	}
	// �������ʼ����յ�֮�仭�ߣ��պ϶����
	MidPointLine(g_Points[0], g_Points[endp - 1]);
	*/
}

/* ��Բ��Ӧ�˸���λ��*/
void CirclePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	setPixel(xCenter + x, yCenter + y, color);
	setPixel(xCenter - x, yCenter + y, color);
	setPixel(xCenter + x, yCenter - y, color);
	setPixel(xCenter - x, yCenter - y, color);
	setPixel(xCenter + y, yCenter + x, color);
	setPixel(xCenter - y, yCenter + x, color);
	setPixel(xCenter + y, yCenter - x, color);
	setPixel(xCenter - y, yCenter - x, color);
}

/* ����Բ��Ӧ�ĸ���λ��*/
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	setPixel(xCenter + x, yCenter + y, color);
	setPixel(xCenter - x, yCenter + y, color);
	setPixel(xCenter + x, yCenter - y, color);
	setPixel(xCenter - x, yCenter - y, color);
}

/*���������դ��*/
void GetYMinMax(PixelPoint* data, int size, int& ymin, int& ymax)
{
	ymin = data[0].y;
	ymax = data[0].y;
	for (int i = 1; i < size; i++)
	{
		if (data[i].y > ymax)
			ymax = data[i].y;
		if (data[i].y < ymin)
			ymin = data[i].y;
	}
}
void InitET(vector<vector<State::tagEDGE>>& etEDGE, PixelPoint* data, int size, int ymin, int ymax)
{
	State::tagEDGE edgeNode;
	// �������е�
	for (int i = 0; i < size; i++)
	{
		PixelPoint ps = data[i]; // ��ǰ�����
		PixelPoint pe = data[(i + 1) % size]; // ��ǰ���յ�
		PixelPoint pss = data[(i - 1 + size) % size]; // ������ڱ����
		PixelPoint pee = data[(i + 2) % size]; // �յ����ڱ��յ�
		// ˮƽ�߲�����
		if (ps.y != pe.y)
		{
			edgeNode.dx = double(pe.x - ps.x) / double(pe.y - ps.y);
			if (pe.y > ps.y) // ȡ��Сֵy�ĵ�x
			{
				edgeNode.x = ps.x;
				edgeNode.ymax = pee.y > pe.y ? pe.y - 1 : pe.y;
				etEDGE[ps.y - ymin].push_back(edgeNode);
			}
			else
			{
				edgeNode.x = pe.x;
				edgeNode.ymax = pss.y > ps.y ? ps.y - 1 : ps.y;
				etEDGE[pe.y - ymin].push_back(edgeNode);
			}
			/*
			edgeNode.dx = double(pe.x - ps.x) / double(pe.y - ps.y);
			if (pe.y > ps.y)
			{
				edgeNode.x = ps.x;
				edgeNode.ymax = pee.y >= pe.y ? (pe.y - 1) : pe.y;
				etEDGE[ps.y - ymin].push_back(edgeNode);
			}
			else
			{
				edgeNode.x = pe.x;
				if (pss.y >= ps.y)
					edgeNode.ymax = ps.y - 1;
				else
					edgeNode.ymax = ps.y;
				etEDGE[pe.y - ymin].push_back(edgeNode);
				*/
		}

	}

}

// ��������
bool LessSort(State::tagEDGE a, State::tagEDGE b)
{
	if (a.x != b.x)
	{
		return a.x < b.x;
	}

	return a.dx < b.dx;
	//return (a.x < b.x ? true : (a.dx < b.dx ? true : false));
}
void InsertAET(vector<State::tagEDGE>& e, vector<State::tagEDGE>& ae)
{
	if (e.size())
	{

		ae.insert(ae.end(), e.begin(), e.end());
		sort(ae.begin(), ae.end(), LessSort);
	}
	return;

}
void ScanFillLine(vector<State::tagEDGE>& aetEDGE, int y, unsigned color)
{
	int size = aetEDGE.size();
	int i = 1;
	while (i < size)
	{
		int x0 = round(aetEDGE[i - 1].x);
		int x1 = round(aetEDGE[i].x);
		for (int x = x0; x <= x1; x++)
		{
			setPixel(x, y, color);
		}

		i += 2;
	}

}
void RemoveAET(vector<State::tagEDGE>& aetEDGE, int y)
{
	auto iter = aetEDGE.begin();
	while (iter != aetEDGE.end())
	{
		if (y == (*iter).ymax)
		{
			iter = aetEDGE.erase(iter);
		}
		else
			iter++;
	}
}

void UpdateAET(vector<State::tagEDGE>& aetEDGE)
{
	for (vector<State::tagEDGE>::iterator iter = aetEDGE.begin(); iter != aetEDGE.end(); iter++)
	{
		(*iter).x += (*iter).dx;
	}
}

void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax, unsigned color)
{
	// ����Сy��ʼ������ɨ�������y
	for (int y = ymin; y <= ymax; y++)
	{
		InsertAET(etEDGE[y - ymin], aetEDGE); // ����ǰ�߱����л��Աߣ��������Ա���
		ScanFillLine(aetEDGE, y, color); // �Ե�ǰ�н������
		RemoveAET(aetEDGE, y);  // ��ǰ���Ա߱����Ƿ��б��Ѿ�ɨ���꣬��Ҫ�Ƴ�
		UpdateAET(aetEDGE); // ����x����
		continue; // ������һ��ɨ��
	}
}
void HorizonEdgeFill(PixelPoint* data, int size, unsigned color)
{
	for (int i = 0; i < size; i++)
	{
		if (data[i].y == data[(i + 1) % size].y)
		{
			for (int x = data[i].x; x < data[(i + 1) % size].x; x++)
			{
				setPixel(x, data[i].y, color);
			}
		}
	}
}

/*��������㷨*/
void PointFill(int x, int y, unsigned newColor)
{
	unsigned oldColor = getPixel(x, y); // ��ȡ�����ڵ�ɵ���ɫ
	PixelPoint startPt = { x, y };
	PixelPoint AdjPt[4];  // �������ͨ�����ܱߵ��ĸ���
	vector<PixelPoint> AdjPtList; // ����һ��ջ���δ�������ɫ�ĵ�
	AdjPtList.push_back(startPt); // �����ӵ�ѹ��ջ

	// ��ջ��ȡ�����������ɫ���ټ���ܱ��ĸ��㣬��δ���ѹ��ջ��,ѭ��ֱ��ջΪ��
	while (!AdjPtList.empty())   
	{
		PixelPoint topPt = AdjPtList.back();
		setPixel(topPt.x, topPt.y, newColor);
		AdjPt[0] = { topPt.x - 1,topPt.y };
		AdjPt[1] = { topPt.x,topPt.y + 1 };
		AdjPt[2] = { topPt.x + 1,topPt.y };
		AdjPt[3] = { topPt.x, topPt.y - 1 };
		AdjPtList.pop_back();
		for (int i = 0; i < 4; i++)
		{
			if (getPixel(AdjPt[i].x, AdjPt[i].y) == oldColor)
			{
				AdjPtList.push_back(AdjPt[i]);
			}
		}

	}
}

void BoundaryFill(int x, int y, unsigned bdrColor, unsigned newColor)
{
	PixelPoint startPt = { x, y };
	PixelPoint AdjPt[4];
	vector<PixelPoint> AdjPtList;
	AdjPtList.push_back(startPt);
	while (!AdjPtList.empty())
	{
		PixelPoint topPt = AdjPtList.back();
		setPixel(topPt.x, topPt.y, newColor);
		AdjPt[0] = { topPt.x - 1,topPt.y };
		AdjPt[1] = { topPt.x,topPt.y + 1 };
		AdjPt[2] = { topPt.x + 1,topPt.y };
		AdjPt[3] = { topPt.x, topPt.y - 1 };
		AdjPtList.pop_back();
		for (int i = 0; i < 4; i++)
		{
			unsigned oldColor = getPixel(AdjPt[i].x, AdjPt[i].y);
			if (oldColor != bdrColor && oldColor != newColor)
			{
				AdjPtList.push_back(AdjPt[i]);
			}
		}

	}
}

/// <summary>
/// ���������������ӵ���ջ�㷨,�����ڿ��������,�����������ұ߽���Ϊ���ӵ���ջ
/// </summary>
/// <param name="slStack">���ÿ��δ�����ջ</param>
/// <param name="xleft">��ǰ��������߽��</param>
/// <param name="xright">��ǰ�������ұ߽��</param>
/// <param name="y">������y����</param>
/// <param name="bdrColor">�߽���ɫ</param>
/// <param name="newColor">�����ɫ</param>
void SearchLinePoint(stack<PixelPoint>& slStack, int xleft, int xright, int y, unsigned bdrColor, unsigned newColor)
{

	bool span_need_fill = false; // �ж�xleft��xright֮���Ƿ��п��������
	PixelPoint Pt;
	// ����߽�㿪ʼֱ���ұ߽�����
	while (xleft <= xright)
	{
		span_need_fill = false;
		//PixelColor = getPixel(xleft, y);
		while ((getPixel(xleft, y) != bdrColor) && (getPixel(xleft, y) != newColor) && (xleft <= xright))
		{
			span_need_fill = true;
			xleft++;
		}
		if (span_need_fill)
		{
			// ������xright��,��Ϊ�����㣬��˵���ջ,��û�дﵽ����
			if ((getPixel(xleft, y) != bdrColor) && (getPixel(xleft, y) != newColor) && (xleft == xright))
			{
				Pt = { xleft,y };
				slStack.push(Pt);
			}
			else
			{
				Pt = { xleft - 1,y };
				slStack.push(Pt);
			}

		}
		//auto pCount = [=](int x, int y, int xright, unsigned bdrColor, unsigned newColor) {while ((x < xright) && (getPixel(x, y) == bdrColor || getPixel(x, y) == newColor)) { x++; }; return x;
	//};
		xleft++;
		//TODO �Թ���Ч��


	}
}

void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor)
{
	const bool toleft = true;
	const bool toright = false;
	int x0, y0, xright, xleft; 
	PixelPoint startPt = { x,y }; // ���ӵ�
	PixelPoint Pt;
	stack<PixelPoint> slStack;// ջ
	slStack.push(startPt);    // ���ӵ���ջ

	// ֱ��ջΪ��
	while (!slStack.empty())
	{
		// ȡ��ջ��Ԫ��
		Pt = slStack.top();
		slStack.pop();

		// ��ջ����x,y��ʼ���Һ������������ɫ��ֱ�������߽�,����������������ұ߽�x����
		x0 = Pt.x;
		y0 = Pt.y;
		auto lrFill = [=](int x, int y, bool direct) {if (direct) { while (getPixel(x, y) != bdrColor) { setPixel(x, y, newColor); x -= 1; } return x + 1; }
		else 
		{
			while (getPixel(x, y) != bdrColor) { setPixel(x, y, newColor); x += 1; }
			return x - 1;
		}};
		xleft = lrFill(x0, y0, toleft); // �������,�������������߽�x����
		xright = lrFill(x0, y0, toright); // �������,������������ұ߽�x����

		// ����е��������н���ɨ���ҵ����ӵ���ջ
		SearchLinePoint(slStack, xleft, xright, y0 - 1, bdrColor, newColor);
		SearchLinePoint(slStack, xleft, xright, y0 + 1, bdrColor, newColor);
	}//��ջ��
}

