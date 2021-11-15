#pragma once

#include "Matrix3.h"
#include "Base2DRenderer.h"

struct Dataset;
struct Layer;
class Map2DRenderer : public Base2DRenderer
{
public:
	Map2DRenderer( );

	void init( Dataset* pDataset, double xOrig_map, double yOrig_map, 
		double width_map, double height_map , int width_wnd, int height_wnd );

	void reset();

	void scale( double d );

	void scale(double x, double y, double d);

protected:

	double _calcScale();

	int mWndWidth , mWndHeight;//���ڿ��
	double mXOrig_map, mYOrig_map;//��Ӧ����ԭ��ĵ�ͼ��
	double mMapWidth, mMapHeight;//��ͼ���

};

