#include "State.h"
#include "Graphic.h"
State g_State;

State::State()
{
	DrawMode = dmPixel;
	GridColor1 = RED;
	GridColor2 = BLACK;
	GridWidth = GridHeight = 10;

	DrawLineMethod = dlMidBresenham;
	DrawPolygonMethod = dpXScanline;
}
