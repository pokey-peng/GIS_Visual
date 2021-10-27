#ifndef _STATE_H
#define _STATE_H

struct State
{
	//enum VisualModeType{ vm2DBasic, vm2DMap, vm3DBasic, vm3DMap }
	//VisualModeType VisualMode ; 
	enum DrawModeType{ dmPixel, dmGrid };
	///����ģʽ
	DrawModeType DrawMode;//����ģʽ��0-����ģʽ��1-����ģʽ

	unsigned GridColor1;//����ģʽ����ɫ1
	unsigned GridColor2;//����ģʽ����ɫ2
	int GridWidth;//�����
	int GridHeight;//�����

	enum DrawLineMethodType{ dlDDA , dlMidBresenham };

	///���߷���
	DrawLineMethodType DrawLineMethod;// 0 --DDA , 1 -- MidBresenham

	enum DrawPolygonMethodType{ dpXScanline , dpAET , dpEdgeFill };
	///������η���
	DrawPolygonMethodType DrawPolygonMethod; // 0 -- Xɨ���� �� 1 - ���Ա߱� �� 2--��Ե���

	State();
};

extern State g_State;

#endif
