#include "DrawState.h"
#include "Graphic.h"

State g_State;

State::State()
{
	DrawMode = dmPixel;
	g_OperationType = otNone;
	GridWidth = GridHeight = 10;
	/*
	GridColor1 = RED;
	GridColor2 = BLACK;
	PenColor = BLACK;*/
	
	/*PanMode = orNone;
	VisualMode = vm2DMap;
	DrawLineMethod = dlMidBresenham;
	DrawPolygonMethod = dpXScanline;
	SlipOperation = ocNone;
	Font = orNonefont;
	g_Projection = oTdNone;
	realColor = tnone;*/
}
