#pragma once

#include "Graphic.h"
//#include "Matrix3.h"

struct Geometry;
struct Dataset;
struct Layer;
class Base2DRenderer
{
public:
	Base2DRenderer(void);
	virtual ~Base2DRenderer(void) ;

	void init( Dataset* pDataset );

	//�������ݼ�������ͼ��
	virtual void render( ) ;
	
	////��ԭ
	//virtual void reset();

	////ƽ�ƣ�������Ϊ��λ
	//void translate(double dx, double dy);

	////��ת����ʱ��Ϊ���� �Զ�Ϊ��λ
	//void rotate(double degree);

	////������ת����ʱ��Ϊ���� �Զ�Ϊ��λ
	//void rotate(double x, double y, double degree);

	////���ķ���
	//void scale(double dx, double dy);

	////�������
	//void scale(double x, double y, double dx, double dy);

protected:
	//����һ��ͼ��
	virtual void renderLayer(  Layer* pLayer );
	virtual void renderGeometry( Geometry* pGeometryDef , Color color );
	
	virtual unsigned getLayerColor( Layer* pLayer );
	
	//virtual void renderLayerBBox( Layer* pLayer );	

	Dataset* mpDataset;//���ݼ�
	bool mInited; //�Ƿ��ʼ��

	//Matrix3 mTransformMatrix;//�任����
};

