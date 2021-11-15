#ifndef _STATE_H
#define _STATE_H

struct State
{
	enum OperationType { otNone, otRefreshWindow,otDrawRectangle, otDrawLineDDA, otDrawLine_Bresenham, otDrawPolyline, otDrawPolygon,
	otDrawCircle, otDrawEllipse,otFillPolygon, otFillPolygon_Border,otGridCell, otFillPolygon_FenceFill, otFillPolygon_EdgeTag,
	otBoundary_SeedFill,StackSeedFill4,SeedLineFill1,otFloodFill,otPan};

	OperationType OperationType;//��ǰ��������
	enum PanMode{orNone,orByMouse};
	PanMode PanMode;
	enum VisualModeType { vm2DBasic, vm2DMap, vm3DBasic, vm3DMap };
	VisualModeType VisualMode = vm2DBasic;

	enum DrawModeType { dmPixel, dmGrid };
	///����ģʽ
	DrawModeType DrawMode;//����ģʽ��0-����ģʽ��1-����ģʽ

	unsigned GridColor1;//����ģʽ����ɫ1
	unsigned GridColor2;//����ģʽ����ɫ2
	unsigned PenColor;//���ʵ���ɫ
	int GridWidth;//�����
	int GridHeight;//�����
	

	//vector<*geometry> g_geometry;


	//����ѡ������ģʽ��������ģʽ
	enum Gridangpixel{};

	enum DrawLineMethodType { dlDDA, dlMidBresenham };//�����ߵķ�ʽ

	///���߷���
	DrawLineMethodType DrawLineMethod;// 0 --DDA , 1 -- MidBresenham

	enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
	///������η���
	DrawPolygonMethodType DrawPolygonMethod; // 0 -- Xɨ���� �� 1 - ���Ա߱� �� 2--��Ե���


	//����ü�������
	enum SlipOperation{ 
		ocNone,ocCohen_Sutherland,ocMidpoint_Segmentation,ocLiang_Barsky, ocSuth_Hodgeman
	};
	SlipOperation SlipOperation;
	enum Font{orNonefont,orfont};
	Font Font;
	//�������������
	char* FontType = "����";//�������ʽ
	int FontSize = 15;//����Ĵ�С

	//�ռ�ͼ��ģʽȷ������ά������ά�ռ�ģʽ
	enum VertexDeclare{ vdXY , vdXYZ };
	VertexDeclare VertexDeclare = vdXY;
	//
	//��ά�ռ�ͶӰģʽ
	//
	enum TdSpaceProjectionMode {oTdNone,
		oTproPerspective,//͸��ͶӰ
		oTproParallel,//ƽ��ͶӰ
		oTproOblique //бͶӰ
	};
	TdSpaceProjectionMode g_Projection;

	//��ά���������㷨����
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
