#ifndef _STATE_H
#define _STATE_H

struct State
{
	//enum VisualModeType{ vm2DBasic, vm2DMap, vm3DBasic, vm3DMap }
	//VisualModeType VisualMode ;
	enum DrawModeType
	{
		dmPixel,
		dmGrid
	};
	///绘制模式
	DrawModeType DrawMode; //绘制模式，0-像素模式，1-网格模式

	unsigned GridColor1; //网格模式下颜色1
	unsigned GridColor2; //网格模式下颜色2
	int GridWidth;		 //网格宽
	int GridHeight;		 //网格高

	enum DrawLineMethodType
	{
		dlDDA,
		dlMidBresenham
	};

	///画线方法
	DrawLineMethodType DrawLineMethod; // 0 --DDA , 1 -- MidBresenham

	enum DrawPolygonMethodType
	{
		dpXScanline,
		dpAET,
		dpEdgeFill
	};
	///画多边形方法
	DrawPolygonMethodType DrawPolygonMethod; // 0 -- X扫描线 ， 1 - 活性边表 ， 2--边缘填充

	State();
};

extern State g_State;

#endif
