#ifndef _STATE_H
#define _STATE_H

struct State
{
	enum OperationType { otNone, otRefreshWindow,otDrawRectangle, otDrawLineDDA, otDrawLine_Bresenham, otDrawPolyline, otDrawPolygon,
	otDrawCircle, otDrawEllipse,otFillPolygon, otFillPolygon_Border,otGridCell, otFillPolygon_FenceFill, otFillPolygon_EdgeTag,
	otBoundary_SeedFill,StackSeedFill4,SeedLineFill1,otFloodFill,otPan};

	OperationType OperationType;//当前操作类型
	enum PanMode{orNone,orByMouse};
	PanMode PanMode;
	enum VisualModeType { vm2DBasic, vm2DMap, vm3DBasic, vm3DMap };
	VisualModeType VisualMode = vm2DBasic;

	enum DrawModeType { dmPixel, dmGrid };
	///绘制模式
	DrawModeType DrawMode;//绘制模式，0-像素模式，1-网格模式

	unsigned GridColor1;//网格模式下颜色1
	unsigned GridColor2;//网格模式下颜色2
	unsigned PenColor;//画笔的颜色
	int GridWidth;//网格宽
	int GridHeight;//网格高
	

	//vector<*geometry> g_geometry;


	//定义选择网格模式还是像素模式
	enum Gridangpixel{};

	enum DrawLineMethodType { dlDDA, dlMidBresenham };//绘制线的方式

	///画线方法
	DrawLineMethodType DrawLineMethod;// 0 --DDA , 1 -- MidBresenham

	enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
	///画多边形方法
	DrawPolygonMethodType DrawPolygonMethod; // 0 -- X扫描线 ， 1 - 活性边表 ， 2--边缘填充


	//定义裁剪的类型
	enum SlipOperation{ 
		ocNone,ocCohen_Sutherland,ocMidpoint_Segmentation,ocLiang_Barsky, ocSuth_Hodgeman
	};
	SlipOperation SlipOperation;
	enum Font{orNonefont,orfont};
	Font Font;
	//定义字体的类型
	char* FontType = "宋体";//字体的样式
	int FontSize = 15;//字体的大小

	//空间图形模式确定，二维还是三维空间模式
	enum VertexDeclare{ vdXY , vdXYZ };
	VertexDeclare VertexDeclare = vdXY;
	//
	//三维空间投影模式
	//
	enum TdSpaceProjectionMode {oTdNone,
		oTproPerspective,//透视投影
		oTproParallel,//平行投影
		oTproOblique //斜投影
	};
	TdSpaceProjectionMode g_Projection;

	//三维对象消隐算法种类
	enum BlankMethods
	{
		tNone, tFrameWork,tZ_buffer,tScanLine, tPinter
	};
	BlankMethods BlankMethods;
	enum realColor
	{
		tnone,ConstantLight,Gouraud
	};
	realColor realColor;
	enum ColorMode{cRGB_R,cRGB_G,cRGB_B};
	ColorMode Colormode;
	State();
};

extern State g_State;

#endif
