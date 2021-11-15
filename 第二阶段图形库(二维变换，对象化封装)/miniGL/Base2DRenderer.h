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

	//绘制数据集包含的图层
	virtual void render( ) ;
	
	////复原
	//virtual void reset();

	////平移，以像素为单位
	//void translate(double dx, double dy);

	////旋转，逆时针为正， 以度为单位
	//void rotate(double degree);

	////定点旋转，逆时针为正， 以度为单位
	//void rotate(double x, double y, double degree);

	////中心放缩
	//void scale(double dx, double dy);

	////定点放缩
	//void scale(double x, double y, double dx, double dy);

protected:
	//绘制一个图层
	virtual void renderLayer(  Layer* pLayer );
	virtual void renderGeometry( Geometry* pGeometryDef , Color color );
	
	virtual unsigned getLayerColor( Layer* pLayer );
	
	//virtual void renderLayerBBox( Layer* pLayer );	

	Dataset* mpDataset;//数据集
	bool mInited; //是否初始化

	//Matrix3 mTransformMatrix;//变换矩阵
};

