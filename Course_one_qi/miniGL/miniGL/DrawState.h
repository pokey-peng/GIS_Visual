#ifndef _DRAWSTATE_H
#define _DRAWSTATE_H

struct State
{
	enum DrawModeType{dmPixel, dmGrid, dmGemotry};
	// 绘制模式
	DrawModeType DrawMode;

	int GridWidth; // 网格宽
	int GridHeight; // 网格高

	enum DrawLineMethodType{dlDDA, dlMidBresenham};

	// 画线方法
	DrawLineMethodType DrawLineMethod;

	enum DrawPolygonMethodType{dpXScanline, dpAET, dpEdgeFill};

	DrawPolygonMethodType DrawPolygonMethod;

};
#endif // !_DRAWSTATE_H
