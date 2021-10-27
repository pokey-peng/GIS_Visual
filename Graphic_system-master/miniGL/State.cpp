#include "State.h"
#include "Graphic.h"
#include "Matrix.h"
State g_State;

State::State()
{
	OperationType = otNone;
	DrawMode = dmPixel;
	GridColor1 = RED;
	GridColor2 = BLACK;
	PenColor = BLACK;
	GridWidth = GridHeight = 1;
	PanMode = orNone;
	VisualMode = vm2DMap;
	DrawLineMethod = dlMidBresenham;
	DrawPolygonMethod = dpXScanline;
	SlipOperation = ocNone;
	Font = orNonefont;
	g_Projection = oTdNone;
	realColor = tnone;
}
