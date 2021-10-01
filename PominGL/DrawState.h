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
	// 多边形填充算法
	enum DrawPolygonMethodType{dpXScanline, dpAET, dpEdgeFill};

	DrawPolygonMethodType DrawPolygonMethod;
	// 边表节点
	struct tagEDGE {
		double x;
		double dx;
		int ymax;
	};

	// 区域填充算法
	enum FillMethodType{seedFill, scanFill};

	FillMethodType FillMethod;
    
};
#endif // !_DRAWSTATE_H
