#include "Raster_byteacher.h"
#include "State.h"
#include <stdlib.h>

#define FAST_TEXT_OUT
void Raster_byteacher::textOut(const char* pTextA , int x, int y , Color color )
{
	//int uLen = MultiByteToWideChar( CP_ACP, 0, pTextA, -1, NULL, 0 );  
	//if ( uLen <= 0 )  return ;

	//wchar_t textw[1024];
	//int nRtn = MultiByteToWideChar( CP_ACP, 0, pTextA, -1, textw, uLen );  
	//if ( nRtn != uLen )  return ;

	int uLen = mbstowcs( 0, pTextA, 0 );//计算实际字符数
	if ( uLen <= 0 )  return ;

	static wchar_t textw[1024] = {0};
	int nRtn = mbstowcs( textw, pTextA, uLen ); //转化为双字节字符串 

	if ( nRtn != uLen )  return ;

	//HFONT hFont = CreateFontA( g_fontHeight, 0, 0/*g_fontDirection*10*/, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
	//	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	//	DEFAULT_PITCH | FF_DONTCARE, g_fontName );

	//HFONT hOldFont = (HFONT)SelectObject( g_hDC, hFont );	

	int x0 = x , y0 = y;
	LPtToDPt( x0, y0, x0, y0 );//输出起点的逻辑坐标转化为屏幕坐标

	int offset_x, offset_y,  gryph_width,  gryph_height,  cell_width,  cell_height;
	for( int c = 0; c < uLen ; ++c )
	{
		wchar_t ch = textw[c];
		byte* pBuf = getWCharGlyph(  ch,  offset_x, offset_y,  gryph_width,  gryph_height,  cell_width,  cell_height );//获取第c个字符位图	

#ifndef FAST_TEXT_OUT
		if( pBuf )
		{
			for( int j = offset_y ; j < offset_y + gryph_height; ++j )
			{
				for( int i = offset_x; i < offset_x + gryph_width; ++i )
				{
					if( getCharGlyphPixel( pBuf, i, j, offset_x, offset_y, gryph_width, gryph_height ))
					{
						int lx =  x0 + i , ly = y0 + j;//输出位置的设备像素坐标
						DPtToLPt( lx, ly, lx, ly );//转化为逻辑坐标
						setPixel( lx, ly, color );
					}
					else
					{
						//setPixel( lx, ly, GREEN );
					}
				}
			}
		}		
#else
		if( pBuf )
		{
			int gryph_width_byte = gryph_width >> 3;//每行字节数
			int dy = y0 + offset_y;//字符外框左上角y坐标+字符y偏移坐标 才是字符位图的输出y坐标
			for (int i = 0; i < gryph_height; i++)
			{
				int dx = x0 + offset_x;//字符外框左上角x坐标+字符x偏移坐标 才是字符位图的输出x坐标
				for (int j = 0; j < gryph_width_byte; j++)//输出一行
				{
					byte btCode = pBuf[j];//获得字符位图行的一个字节==8个像素
					for (int k = 0; k < 8; k++)
					{
						if ( btCode & ( 0x80 >> k ) )//判断对应位是否为1
						{
							int lx =  dx , ly = dy ;
							DPtToLPt( lx, ly, lx, ly );//转化为逻辑坐标
							setPixel( lx, ly, color );
						}
						else
						{
							//setPixel( lx, ly, GREEN );
						}
						dx += 1;//下一像素

					}			
				}
				pBuf += gryph_width_byte;//字符位图行下一字节
				dy += 1;//移到下一行
			}
		}
#endif		

		x0 += cell_width;//下一个字符输出x位置
		y0 += 0;
	}

	//SelectObject(g_hDC,hOldFont);
	//DeleteObject(hFont);
}

void Raster_byteacher::drawCell(int row, int col , unsigned color )
{
	for( int i = row * g_State.GridHeight; i < (row +1)* g_State.GridHeight;++i  )
		for ( int j = col * g_State.GridWidth; j < (col + 1)* g_State.GridWidth; ++j )
		{
			setPixel( j,  i, color);
		}
}

void Raster_byteacher::drawGrid()
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