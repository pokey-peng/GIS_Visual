/********************************************************************
file base:	Graphic
file ext:	h
author:		����(njnu2016@foxmail.com)
purpose:	ͼ�ο�ӿ�
version:    1.0
*********************************************************************/
#ifndef _GRAPH_H
#define _GRAPH_H

#define BLACK 0
#define WHITE 0xFFFFFF
#define RED 0xFF
#define GREEN 0xFF00
#define BLUE 0xFF0000
#define YELLOW 0x00FFFF

typedef unsigned Color;
typedef unsigned char byte;
typedef unsigned short dword;

#define _RGB(r,g,b)  ((unsigned)(((byte)(r)|((dword)((byte)(g))<<8))|(((dword)(byte)(b))<<16)))

#define _RGBA(r,g,b,a) ((unsigned)(((byte)(r)|((dword)((byte)(g))<<8))|(((dword)(byte)(b))<<16))|(((dword)(byte)(a))<<24))

#define  _R( color ) ((byte)(color))
#define  _G( color ) ((byte)(color >> 8))
#define  _B( color ) ((byte)(color >> 16))
#define  _A( color ) ((byte)(color >> 24))

///���ص�
struct PixelPoint
{
	int x,y;
	double z;
};

/**	��ʼ��
@return �Ƿ��ʼ���ɹ���0-success�� -1 - fail
*/
int init();

/**	��ʼ��
@return �Ƿ��ʼ���ɹ���0-success�� -1 - fail
*/
int init( unsigned hwnd);

/**	ˢ�´���
*/
void refreshWindow();

/**	��ȡ���ڿ��
@return ���ڿ��
*/
int getWindowWidth();

/**	��ȡ���ڸ߶�
@return ���ڸ߶�
*/
int getWindowHeight();

/**	����Y�᷽��
@param  isUp true��ʾ���ϣ���������
*/
void setYUp(bool isUp);

/**	��ȡ�߼�����ϵY�᷽��
@return Y���Ƿ�����
*/
bool isYUp();

/**	�����߼�����ϵԭ��Ϊָ������λ��
@param  x ָ������λ��x����
@param  y ָ������λ��y����
*/
void setOrig(int x, int y);

/**	��ȡ�߼�����ϵԭ������λ��
@param  x ԭ��x����
@param  y ԭ��y����
*/
void getOrig(int& x, int& y);

/**	����ָ���߼�λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setPixel(int x, int y, Color color);

/**	����ָ���߼�λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setPixel(int x, int y, float z, Color color);

/**	����ָ������λ�����ص���ɫ������ϵԭ��λ�����Ͻǣ�x���ң�y����
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setDevicePixel(int x, int y, Color color);

/**	����ָ������λ�����ص���ɫ������ϵԭ��λ�����Ͻǣ�x���ң�y����
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setDevicePixel(int x, int y, float z, Color color);

/**	��ȡָ��λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@return ָ��λ�����ص���ɫ
*/
Color getPixel(int x, int y);

/**	��ȡָ������λ�����ص���ɫ������ϵԭ��λ�����Ͻǣ�x���ң�y����
@param  x ����x����
@param  y  ����y����
@return ָ��λ�����ص���ɫ
*/
Color getDevicePixel(int x, int y);

/**	�豸����תΪ�߼�����
@param  dx �豸x����
@param  dy  �豸y����
@param  lx �߼�x����
@param  ly  �߼�y����
*/
void DPtToLPt(int dx, int dy, int& lx, int& ly);

/**	�߼�����תΪ�豸����
@param  lx �߼�x����
@param  ly  �߼�y����
@param  dx �豸x����
@param  dy  �豸y����
*/
void LPtToDPt( int lx, int ly, int& dx, int& dy );

/**	�ж�Shift���Ƿ���
@return  Shift���Ƿ���
*/
bool isShiftKeyPressed();

/**	�ж�Ctrl���Ƿ���
@return  Ctrl���Ƿ���
*/
bool isCtrlKeyPressed();

/**	��ȡ����ɫ
*/
Color getBackColor();

/**	���ñ���ɫ
@param  color ����ɫ
*/
void setBackColor( Color color );

/**	��ȡ������ɫ
*/
Color getPenColor();

/**	���û���ɫ
@param  color ����ɫ
*/
void setPenColor( Color color );

/**	��ȡ����ʵ�ʴ�С����mmΪ��λ
*/
double getPixelSize();

///��Ƥ��ģʽ, rmNone -- �ر���Ƥ��ģʽ ��rmLine-- ��ģʽ�� rmRectangle -- ����ģʽ , rmPolyline-- ����ģʽ, rmPolygon -- �����ģʽ
enum RubberMode{ rmNone, rmLine, rmRectangle, rmPolyline, rmPolygon, rmCircle, rmEllipse };

/**	���õ�ǰģʽ�Ƿ�Ϊ��Ƥ��ģʽ
@param  value �Ƿ���Ƥ��ģʽ
*/
void setRubberMode(RubberMode mode );

/**	��ȡ��ǰ��Ƥ��ģʽ
@return ��ǰ��Ƥ��ģʽ
*/
RubberMode getRubberMode();

/**	��ȡ��Ƥ�ߵ�������һ��������ǰ����ռ�
@return ��Ƥ�ߵ���
*/
int getRubberPointCount();

/**	��ȡ��Ƥ�������յ㣬��������������Ƥ��ģʽ����������Ƥ��
@param  x1 ���x����
@param  y1  ���y����
@param  x2 �յ�x����
@param  y2  �յ�y����
@return ��Ч��Ƥ�ߵ���
*/
int getRubberPoints(int& x1, int& y1, int& x2, int& y2);

/**	��ȡ��Ƥ�������յ㣬��������������Ƥ��ģʽ����������Ƥ��
@param  pt1 ���
@param  pt2 �յ�
@return ��Ч��Ƥ�ߵ���
*/
int getRubberPoints(PixelPoint& pt1, PixelPoint& pt2 );

/**	��ȡ��Ƥ�ߵ㼯�ϣ�һ������rmPolyline��rmPolygonģʽ
@param  pts ��Ƥ�ߵ㼯��
@return ��Ƥ�ߵ���
*/
int getRubberPoints( PixelPoint pts[] );

///�����ʽ��csArrow--��ͷ ��  csHand--���ͣ� csCross--ʮ��˿�� csSize--����
enum CursorStyle{ csArrow = 32512, csHand = 32649, csCross = 32515, csSize = 32646 };

/**	���ù����ʽ
@param  cursor �����ʽ
*/
void setCursor(CursorStyle cursor);

/**	��ȡ��ǰ�����С
@return ���������С
*/
int getFontSize();

/**	���������С
@param  fontSize �����С
*/
void setFontSize( int fontSize );

/**	�����Сת��Ϊ����߶�
@param  fontSize �����С
@return ��������߶�
*/
int fontSizeToFontHeight( int fontSize);

/**	����߶�ת��Ϊ�����С
@param  fontHeight ����߶�
@return ���������С
*/
int fontHeightToFontSize( int fontHeight );

/**	��ȡ������
@return ����������
*/
const char* getFontName() ;

/**	����������
@param  fontName ������
*/
void setFontName( const char* fontName );

/**	��ȡ���ֽ��ַ�λͼ
@remark λͼ����x����Ϊ����y����Ϊ����ʵ��λͼ����ֻռ�ַ�����һ���֣�ͨ��offset_x��offset_y��gryph_width��gryph_height��ͬȷ��λͼ��Χ
@param  ch ���ֽ��ַ�
@param  offset_x �ַ�λͼ��������ַ�������Ͻ�x����ƫ��
@param  offset_y �ַ�λͼ��������ַ�������Ͻ�y����ƫ�ƣ�����Ϊ��
@param  gryph_width �ַ�λͼ�����
@param  gryph_height �ַ�λͼ������
@param  cell_width �ַ������
@param  cell_width �ַ����߶�
@return ���ص��ַ�λͼ������
*/
byte* getCharGlyph(  char ch , int& offset_x, int& offset_y, int& gryph_width, int& gryph_height, int& cell_width, int& cell_height );

/**	��ȡ˫�ֽ��ַ�λͼ
@remark λͼ����x����Ϊ����y����Ϊ����ʵ��λͼ����ֻռ�ַ�����һ���֣�ͨ��offset_x��offset_y��gryph_width��gryph_height��ͬȷ��λͼ��Χ
@param  ch ˫�ֽ��ַ�
@param  offset_x �ַ�λͼ��������ַ�������Ͻ�x����ƫ��
@param  offset_y �ַ�λͼ��������ַ�������Ͻ�y����ƫ�ƣ�����Ϊ��
@param  gryph_width �ַ�λͼ�����
@param  gryph_height �ַ�λͼ������
@param  cell_width �ַ������
@param  cell_width �ַ����߶�
@return ���ص��ַ�λͼ������
*/
byte* getWCharGlyph(  wchar_t ch , int& offset_x, int& offset_y, int& gryph_width, int& gryph_height, int& cell_width, int& cell_height );

/**	��ȡ�ַ�λͼ����ֵ 0 or 1
@remark �ַ�λͼ����ֵ = 0 ��ʾ�ַ�������ͨ����λ�ã�= 1 ��ʾ�ַ�����ͨ����λ��
@param  pCharGlyph �ַ�λͼ
@param  x ����ˮƽλ�ã�������ַ������߿�
@param  y ���ش�ֱλ�ã�������ַ���򶥱߿�
@param  offset_x �ַ�λͼ��������ַ�������Ͻ�x����ƫ��
@param  offset_y �ַ�λͼ��������ַ�������Ͻ�y����ƫ�ƣ�����Ϊ��
@param  gryph_width �ַ�λͼ�����
@param  gryph_height �ַ�λͼ������
@return ����0 or 1
*/
byte getCharGlyphPixel( byte * pCharGlyph, int x, int y, int offset_x, int offset_y, int gryph_width, int gryph_height );


#endif