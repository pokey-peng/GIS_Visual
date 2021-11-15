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
extern Color g_Color; // 线段颜色
extern PixelPoint g_Points[100]; // 存放需要绘制的点的数组
extern int g_PointCout;  //点的个数

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
		//				drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);//擦除
		//				lineToFirst = false;//多边形模式下防止第一条线被擦除
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
		//		drawRubberline(g_HDC, pt1.x, pt1.y, pt2.x, pt2.y);	//擦除P1P2

		//		if (rubberMode == rmPolygon && pts.size() >= 2) // >= 2 --> P2 与两个点相连
		//		{
		//			if (lineToFirst)
		//				drawRubberline(g_HDC, pts[0].x, pts[0].y, pt2.x, pt2.y);
		//			else
		//				lineToFirst = true;	 //擦除P0P2
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
/// Bresenham中点算法
/// </summary>
/// <param name="startp">开始绘制起点坐标</param>
/// <param name="endp">绘制终点坐标</param>
void MidPointLine(double x0, double y0, double x1, double y1, unsigned color)
{
	

	int dex = abs(x0 - x1);
	int dey = abs(y0 - y1);
	bool tag = false;
	// 当直线斜率大于1时
	if (dex < dey)
	{
		std::swap(dex, dey);
		tag = true;
	}
	// x,y增量
	int dx = sign(x1 - x0); int dy = sign(y1 - y0);
	int d = dex - dey - dey; // 初始增量
	int x = x0; int y = y0; // 起点
	for (int i = 0; i <= dex; ++i)
	{
		g_State.drawPixelCB(x, y, color);
		if (d < 0) // d<0时，x,y分别加上加dx,dy
		{
			x += dx; y += dy;
			d += (dex + dex - dey - dey); // 增量更新2dex-2dey
		}
		else
		{
			// d>0且斜率大于1时，y更新
			if (tag == true)
				y += dy;
			else // d>0且斜率不大于1时，x更新
				x += dx;
			d -= (dey + dey); // 增量更新-2dey
		}
	}

}
/// <summary>
/// DDA直线算法
/// </summary>
/// <param name="startp">开始绘制起点坐标</param>
/// <param name="endp">绘制终点坐标</param>
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

/* 绘制折线*/
void DrawPolyline()
{
	// 起始索引
	int startp = 0; int endp = 1;
	// 遍历数组中存在的点
	while (endp < g_PointCout)
	{
		// 每两个点之间画一条线段
		//MidPointLine(g_Points[startp], g_Points[endp]);
		DDALine(g_Points[startp], g_Points[endp]);
		startp++;
		endp++;
	}
}
/* 画空心矩形*/
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
	for (int i = xmin; i <= xmax; ++i)//用指定颜色绘制空心矩形
	{
		setPixel(i, ymin, g_Color);
		setPixel(i, ymax, g_Color);
	}
}
/* 画空心多边形*/
void DrawPolygon()
{
	/*
	// 先把数组中的点按顺序依次画出线段
	int startp = 0; int endp = 1;

	while (endp < g_PointCout)
	{
		MidPointLine(g_Points[startp], g_Points[endp]);
		startp++;
		endp++;
	}
	// 最后在起始点和终点之间画线，闭合多边形
	MidPointLine(g_Points[0], g_Points[endp - 1]);
	*/
}

/* 画圆对应八个点位置*/
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

/* 画椭圆对应四个点位置*/
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	setPixel(xCenter + x, yCenter + y, color);
	setPixel(xCenter - x, yCenter + y, color);
	setPixel(xCenter + x, yCenter - y, color);
	setPixel(xCenter - x, yCenter - y, color);
}

/*多边形填充光栅化*/
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
	// 遍历所有点
	for (int i = 0; i < size; i++)
	{
		PixelPoint ps = data[i]; // 当前边起点
		PixelPoint pe = data[(i + 1) % size]; // 当前边终点
		PixelPoint pss = data[(i - 1 + size) % size]; // 起点相邻边起点
		PixelPoint pee = data[(i + 2) % size]; // 终点相邻边终点
		// 水平线不处理
		if (ps.y != pe.y)
		{
			edgeNode.dx = double(pe.x - ps.x) / double(pe.y - ps.y);
			if (pe.y > ps.y) // 取最小值y的点x
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

// 升序排列
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
	// 从最小y开始，逐行扫描至最大y
	for (int y = ymin; y <= ymax; y++)
	{
		InsertAET(etEDGE[y - ymin], aetEDGE); // 若当前边表中有活性边，则插入活性表中
		ScanFillLine(aetEDGE, y, color); // 对当前行进行填充
		RemoveAET(aetEDGE, y);  // 当前活性边表中是否有边已经扫描完，需要移除
		UpdateAET(aetEDGE); // 更新x坐标
		continue; // 继续下一行扫描
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

/*区域填充算法*/
void PointFill(int x, int y, unsigned newColor)
{
	unsigned oldColor = getPixel(x, y); // 获取区域内点旧的颜色
	PixelPoint startPt = { x, y };
	PixelPoint AdjPt[4];  // 存放四联通区域周边的四个点
	vector<PixelPoint> AdjPtList; // 设置一个栈存放未填充新颜色的点
	AdjPtList.push_back(startPt); // 将种子点压入栈

	// 从栈顶取出点填充新颜色，再检查周边四个点，若未填充压入栈中,循环直至栈为空
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
/// 搜索上下两行种子点入栈算法,若存在可填充区域,将区域内最右边界作为种子点入栈
/// </summary>
/// <param name="slStack">存放每行未填充点的栈</param>
/// <param name="xleft">当前行区域左边界点</param>
/// <param name="xright">当前行区域右边界点</param>
/// <param name="y">上下行y坐标</param>
/// <param name="bdrColor">边界颜色</param>
/// <param name="newColor">填充颜色</param>
void SearchLinePoint(stack<PixelPoint>& slStack, int xleft, int xright, int y, unsigned bdrColor, unsigned newColor)
{

	bool span_need_fill = false; // 判断xleft与xright之间是否有可填充区域
	PixelPoint Pt;
	// 从左边界点开始直到右边界点结束
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
			// 若到达xright点,且为可填充点，则此点入栈,若没有达到，则
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
		//TODO 略过无效点


	}
}

void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor)
{
	const bool toleft = true;
	const bool toright = false;
	int x0, y0, xright, xleft; 
	PixelPoint startPt = { x,y }; // 种子点
	PixelPoint Pt;
	stack<PixelPoint> slStack;// 栈
	slStack.push(startPt);    // 种子点入栈

	// 直至栈为空
	while (!slStack.empty())
	{
		// 取出栈顶元素
		Pt = slStack.top();
		slStack.pop();

		// 从栈顶点x,y开始向右和向左填充新颜色，直至遇到边界,并返回填充区域左右边界x坐标
		x0 = Pt.x;
		y0 = Pt.y;
		auto lrFill = [=](int x, int y, bool direct) {if (direct) { while (getPixel(x, y) != bdrColor) { setPixel(x, y, newColor); x -= 1; } return x + 1; }
		else 
		{
			while (getPixel(x, y) != bdrColor) { setPixel(x, y, newColor); x += 1; }
			return x - 1;
		}};
		xleft = lrFill(x0, y0, toleft); // 向左填充,返回填充区域左边界x坐标
		xright = lrFill(x0, y0, toright); // 向右填充,返回填充区域右边界x坐标

		// 向此行的上下两行进行扫描找到种子点入栈
		SearchLinePoint(slStack, xleft, xright, y0 - 1, bdrColor, newColor);
		SearchLinePoint(slStack, xleft, xright, y0 + 1, bdrColor, newColor);
	}//出栈完
}

