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

	enum DrawPolygonMethodType{dpXScanline, dpAET, dpEdgeFill};

	DrawPolygonMethodType DrawPolygonMethod;

};
#endif // !_DRAWSTATE_H
