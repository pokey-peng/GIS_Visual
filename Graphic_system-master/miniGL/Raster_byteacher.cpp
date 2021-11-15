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

	int uLen = mbstowcs( 0, pTextA, 0 );//����ʵ���ַ���
	if ( uLen <= 0 )  return ;

	static wchar_t textw[1024] = {0};
	int nRtn = mbstowcs( textw, pTextA, uLen ); //ת��Ϊ˫�ֽ��ַ��� 

	if ( nRtn != uLen )  return ;

	//HFONT hFont = CreateFontA( g_fontHeight, 0, 0/*g_fontDirection*10*/, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
	//	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	//	DEFAULT_PITCH | FF_DONTCARE, g_fontName );

	//HFONT hOldFont = (HFONT)SelectObject( g_hDC, hFont );	

	int x0 = x , y0 = y;
	LPtToDPt( x0, y0, x0, y0 );//��������߼�����ת��Ϊ��Ļ����

	int offset_x, offset_y,  gryph_width,  gryph_height,  cell_width,  cell_height;
	for( int c = 0; c < uLen ; ++c )
	{
		wchar_t ch = textw[c];
		byte* pBuf = getWCharGlyph(  ch,  offset_x, offset_y,  gryph_width,  gryph_height,  cell_width,  cell_height );//��ȡ��c���ַ�λͼ	

#ifndef FAST_TEXT_OUT
		if( pBuf )
		{
			for( int j = offset_y ; j < offset_y + gryph_height; ++j )
			{
				for( int i = offset_x; i < offset_x + gryph_width; ++i )
				{
					if( getCharGlyphPixel( pBuf, i, j, offset_x, offset_y, gryph_width, gryph_height ))
					{
						int lx =  x0 + i , ly = y0 + j;//���λ�õ��豸��������
						DPtToLPt( lx, ly, lx, ly );//ת��Ϊ�߼�����
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
			int gryph_width_byte = gryph_width >> 3;//ÿ���ֽ���
			int dy = y0 + offset_y;//�ַ�������Ͻ�y����+�ַ�yƫ������ �����ַ�λͼ�����y����
			for (int i = 0; i < gryph_height; i++)
			{
				int dx = x0 + offset_x;//�ַ�������Ͻ�x����+�ַ�xƫ������ �����ַ�λͼ�����x����
				for (int j = 0; j < gryph_width_byte; j++)//���һ��
				{
					byte btCode = pBuf[j];//����ַ�λͼ�е�һ���ֽ�==8������
					for (int k = 0; k < 8; k++)
					{
						if ( btCode & ( 0x80 >> k ) )//�ж϶�Ӧλ�Ƿ�Ϊ1
						{
							int lx =  dx , ly = dy ;
							DPtToLPt( lx, ly, lx, ly );//ת��Ϊ�߼�����
							setPixel( lx, ly, color );
						}
						else
						{
							//setPixel( lx, ly, GREEN );
						}
						dx += 1;//��һ����

					}			
				}
				pBuf += gryph_width_byte;//�ַ�λͼ����һ�ֽ�
				dy += 1;//�Ƶ���һ��
			}
		}
#endif		

		x0 += cell_width;//��һ���ַ����xλ��
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