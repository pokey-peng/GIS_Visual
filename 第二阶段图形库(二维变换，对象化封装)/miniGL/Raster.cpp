#include "Raster.h"
#include "State.h"
#include "Graphic.h"

void Raster::drawCell(int row, int col , unsigned color )
{
	for( int i = row * g_State.GridHeight; i < (row +1)* g_State.GridHeight;++i  )
		for ( int j = col * g_State.GridWidth; j < (col + 1)* g_State.GridWidth; ++j )
		{
			setPixel( j,  i, color);
		}
}

void Raster::drawGrid()
{
	int orig_x, orig_y;
	getOrig( orig_x, orig_y );

	int left = (int)((float)orig_x / g_State.GridWidth + 0.5);
	int right = (int)((float)( getWindowWidth() - orig_x) / g_State.GridWidth + 0.5);
	int top = (int)((float)orig_y / g_State.GridHeight + 0.5);
	int bottom = (int)((float)( getWindowHeight() - orig_y) / g_State.GridHeight + 0.5);

	int ymin,ymax;
	if( isYUp())
	{
		ymin = - bottom;
		ymax = top;
	}
	else
	{
		ymin = -top;
		ymax = bottom;
	}
	for( int col = -left ; col <= right; ++col )
		for ( int row = ymin ; row <= ymax; ++row )
		{
			drawCell(row, col , (row + col)%2 ? g_State.GridColor1:g_State.GridColor2  );
		}
}