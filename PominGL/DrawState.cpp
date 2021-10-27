#include "DrawState.h"
#include "Graphic.h"

State g_State;

State::State()
{
	DrawMode = dmPixel;
	g_OperationType = otNone;
	GridWidth = GridHeight = 10;
}
