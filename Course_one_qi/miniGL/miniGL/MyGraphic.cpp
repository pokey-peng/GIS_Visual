#include <windows.h>
#include "MyGraphic.h"
#include "Graphic.h"
#include <vector>
#include "DrawState.h"
#include <algorithm>
//using namespace State;
using namespace std;
extern Color g_Color; // �߶���ɫ
extern PixelPoint g_Points[100]; // �����Ҫ���Ƶĵ������
extern int g_PointCout;  //��ĸ���

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
void MidPointLine(PixelPoint startp, PixelPoint endp)
{
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
	
}
/// <summary>
/// DDAֱ���㷨
/// </summary>
/// <param name="startp">��ʼ�����������</param>
/// <param name="endp">�����յ�����</param>
void DDALine(PixelPoint startp, PixelPoint endp)
{
	int x = 0,dex,dey,dexx,deyy;
	float dx,dy,xp,yp;
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

// ��������
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
// �����ľ���
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
// �����Ķ����
void DrawPolygon()
{
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
}

// ��Բ��Ӧ�˸���λ��
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

// ��Բ��Ӧ�ĸ���λ��
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	setPixel(xCenter + x, yCenter + y, color);
	setPixel(xCenter - x, yCenter + y, color);
	setPixel(xCenter + x, yCenter - y, color);
	setPixel(xCenter - x, yCenter - y, color);
}

void GetYMinMax(PixelPoint* data, int size,int& ymin, int& ymax)
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
void InitET(vector<vector<State::tagEDGE>>& etEDGE, PixelPoint* data, int size,int ymin,int ymax)
{
	State::tagEDGE edgeNode;
	// �������е�
	for (int i = 0; i < size; i++)
	{
		PixelPoint ps = data[i]; // ��ǰ�����
		PixelPoint pe = data[(i + 1) % size]; // ��ǰ���е�
		PixelPoint pss = data[(i - 1 + size) % size]; // ������ڱ����
		PixelPoint pee = data[(i + 2) % size]; // �յ����ڱ��յ�
		// ˮƽ�߲�����
		if (ps.y != pe.y)
		{
			edgeNode.dx = double(pe.x - ps.y) / double(pe.y - ps.y);
			if (pe.y > ps.y)
			{
				edgeNode.x = ps.x;
				edgeNode.ymax = pee.y >= pe.y ? pe.y - 1 : pe.y;
				etEDGE[ps.y - ymin].push_back(edgeNode);
			}
			else
			{
				edgeNode.x = pe.x;
				if (pss.y >= ps.y)
					edgeNode.ymax = ps.y - 1;
				else
					edgeNode.ymax = ps.y - 1;
				etEDGE[pe.y-ymin].push_back(edgeNode);
			}
		}

	}

}
void InitAET()
{

}
// ��������
bool LessSort(State::tagEDGE a, State::tagEDGE b)
{
	return (a.x < b.x ? true : (a.dx < b.dx ? true : false));
}
void InsertAET(vector<State::tagEDGE>& e,vector<State::tagEDGE>& ae)
{
	if (e.size())
	{
		sort(e.begin(), e.end(), LessSort);
		ae = e;
	}
	return;
	
}
void ScanFillLine(vector<State::tagEDGE>& aetEDGE, int y, unsigned color)
{
	int size = aetEDGE.size();
	int i = 1;
	while (i < size)
	{
		for (int x0 = aetEDGE[i - 1].x; x0 < aetEDGE[i].x; x0++)
		{
			setPixel(x0, y, color);
		}
	}
	
}
void RemoveNonAET(vector<State::tagEDGE>& aetEDGE,int y)
{
	for (vector<State::tagEDGE>::iterator iter=aetEDGE.begin(); iter!=aetEDGE.end(); iter++)
	{
		if (y == (*iter).ymax)
		{
			iter = aetEDGE.erase(iter);
			iter--;
		}
	}
}
void UpdateMethod(State::tagEDGE e)
{
	e.x += e.dx;
}
void UpdateAET(vector<State::tagEDGE>& aetEDGE)
{
	for_each(aetEDGE.begin(), aetEDGE.end(), UpdateMethod);
}
void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax, unsigned color)
{
	for (int y = ymin; y <= ymax; y++)
	{
		InsertAET(etEDGE[y - ymin], aetEDGE);
		ScanFillLine(aetEDGE, y, color);
		RemoveNonAET(aetEDGE,y);
		UpdateAET(aetEDGE);
		continue;
	}
}