#ifndef _DRAWSTATE_H
#define _DRAWSTATE_H

struct State
{
	
	enum DrawModeType{dmPixel, dmGrid, dmGemotry};
	// ����ģʽ
	DrawModeType DrawMode;

	int GridWidth; // �����
	int GridHeight; // �����

	enum DrawLineMethodType{dlDDA, dlMidBresenham};

	// ���߷���
	DrawLineMethodType DrawLineMethod;
	// ���������㷨
	enum DrawPolygonMethodType{dpXScanline, dpAET, dpEdgeFill};

	DrawPolygonMethodType DrawPolygonMethod;
	// �߱�ڵ�
	struct tagEDGE {
		double x;
		double dx;
		int ymax;
	};
};
#endif // !_DRAWSTATE_H
