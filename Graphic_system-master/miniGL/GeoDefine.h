#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include "Graphic.h"

#include "State.h"
using namespace std;

#define  PI  3.14159265358979323846

//2D点
template<typename T>
struct _Point2D
{
	_Point2D() { x = y = 0; }
	_Point2D(T x, T y) { this->x = x, this->y = y; }

	void operator = (PixelPoint& pt)
	{
		x = (T)pt.x;
		y = (T)pt.y;
	}

	T x, y;
};

typedef _Point2D<double> Point2D;

//2D包围盒
template<typename T>
struct _Box2D
{
	_Box2D() { valid = false; }
	_Box2D(T xmin, T ymin, T xmax, T ymax)
	{
		setBox(xmin, ymin, xmax, ymax);
	}

	T width() { return _xmax - _xmin; }//包围盒宽度
	T height() { return _ymax - _ymin; }//包围盒高度
	T centerX() { return (_xmin + _xmax) / 2; }//包围盒中心点x坐标
	T centerY() { return (_ymin + _ymax) / 2; }//包围盒中心点y坐标
	T xmin() { return _xmin; }
	T ymin() { return _ymin; }
	T xmax() { return _xmax; }
	T ymax() { return _ymax; }

	void setBox(T xmin, T ymin, T xmax, T ymax)//设置包围盒范围
	{
		this->_xmin = xmin, this->_xmax = xmax, this->_ymin = ymin, this->_ymax = ymax;
		valid = true;
	}

	void setBox(_Box2D& box)
	{
		setBox(box._xmin, box._ymin, box._xmax, box._ymax);
	}

	//根据添加的点扩展包围盒
	void expand(T x, T y)
	{
		if (!valid) {
			_xmin = _xmax = x;
			_ymin = _ymax = y;
			valid = true;
		}
		else
		{
			_xmin = min(_xmin, x);
			_xmax = max(_xmax, x);
			_ymin = min(_ymin, y);
			_ymax = max(_ymax, y);
		}
	}

	//根据添加的包围盒扩展包围盒
	void expand(_Box2D<T>& box) {
		if (box.valid)
		{
			if (!valid)
				setBox(box);
			else
			{
				expand(box.xmin(), box.ymin());
				expand(box.xmax(), box.ymax());
			}
		}
	}

protected:
	T _xmin, _ymin;
	T _xmax, _ymax;
	bool valid;//是否有效
};
typedef _Box2D<double> Box2D;

//图形对象类型
enum GeomType { gtUnkown = 0, gtPoint = 1, gtPolyline = 2, gtPolygon = 3 , gtSeedFill };

//图形对象基类，供继承
struct Geometry
{
	virtual ~Geometry() {}
	virtual GeomType getGeomType() = 0; //获取图形对象类型
	virtual Box2D getEnvelop() = 0;//获取图形对象包围盒
	string label;
};


//点图像对象
struct PointGemetry :Geometry
{
	PointGemetry()
	{
		x = y = 0;
	}
	PointGemetry(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	virtual GeomType getGeomType() { return gtPoint; }
	virtual Box2D getEnvelop() { return Box2D(x, y, x, y); }

	double x, y;
};

//线图形对象
struct PolylineGeometry :Geometry
{
	virtual GeomType getGeomType() { return gtPolyline; }

	//运算符重载，获取第i个点
	Point2D& operator[](int i) { return pts[i]; }

	//添加点
	void addPoint(double x, double y)
	{
		pts.push_back(Point2D(x, y));
		envelop.expand(x, y);
	}

	virtual Box2D getEnvelop() { return envelop; }
	const vector<Point2D>& getPts() const
	{
		return pts;
	}
protected:
	//所有点
	vector<Point2D> pts;
	Box2D envelop;
};



//图层
struct Layer
{
	Layer()
	{
		geomType = gtUnkown;
	}

	Layer(GeomType geomType)
	{
		this->geomType = geomType;
	}

	virtual ~Layer()
	{
		for (size_t i = 0, size = geometrySet.size(); i < size; ++i) delete geometrySet[i];//析构函数删除所有图形对象
	}

	//运算符重载，返回第i个图形对象
	Geometry* operator[](int i) { return geometrySet[i]; }

	//添加图像对象
	void addGeometry(Geometry* pGeometry, bool updateEnvelop = false)
	{
		geometrySet.push_back(pGeometry);
		if (updateEnvelop)
			envelop.expand(pGeometry->getEnvelop());
	}

	//设置图层范围
	void setEnvelop(double xmin, double ymin, double xmax, double ymax)
	{
		envelop.setBox(xmin, ymin, xmax, ymax);
	}

	//获取图层包含的图形对象数量
	int getGeometryCount() { return geometrySet.size(); }

	vector<Geometry*> geometrySet;//图形对象集合
	Box2D envelop;//图层范围对应的包围盒
	GeomType geomType;//图层类型
};

//数据集
struct Dataset
{
	virtual ~Dataset()
	{
		for (size_t i = 0, size = layerSet.size(); i < size; ++i) delete layerSet[i];//析构函数删除图层
	}

	//运算符重载，返回第i个图层
	Layer* operator[](int i) { return layerSet[i]; }

	int indexOf(Layer* pLayer) {
		for (int i = 0; i < layerSet.size(); ++i)
		{
			if (pLayer = layerSet[i]) return i;
		}
		return -1;
	}

	//获取图层数
	int getLayerCount() { return layerSet.size(); }

	//添加图层
	void addLayer(Layer* pLayer)
	{
		layerSet.push_back(pLayer);
	}

	//图层集合
	vector<Layer*> layerSet;
};

//多边形图形对象
struct PolygonGeometry :PolylineGeometry
{
	virtual GeomType getGeomType() { return gtPolygon; }
};
//种子点对象
struct SeedFillGeometry :PointGemetry
{
	SeedFillGeometry(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	virtual GeomType getGeomType() { return gtSeedFill; }
};
//定义风格的种类
enum StyleType { stColor }; 


//定义风格的种类
struct Style
{
	virtual ~Style() {}
	virtual StyleType getStyleType() = 0; //获取图形对象类型
};
//绘制的方式
enum OperationType {
	otNone, otRefreshWindow, otDrawRectangle, otDrawLineDDA, otDrawLine_Bresenham, otDrawPolyline, otDrawPolygon,
	otDrawCircle, otDrawEllipse, otFillPolygon, otFillPolygon_Border, otGridCell, otFillPolygon_FenceFill, otFillPolygon_EdgeTag,
	otBoundary_SeedFill, StackSeedFill4, SeedLineFill1, otFloodFill, otPan
};

struct ColorStyle : public Style    //在风格设置里面增加一个绘制方式的选择
{
	ColorStyle()
	{
		color = BLACK;
		drawMethod = otNone; //add
	}
	ColorStyle(Color color , OperationType drawMethod) //add
	{
		this->color = color;
		this->drawMethod = drawMethod; //add
	}

	virtual StyleType getStyleType()
	{
		return stColor;
	}
	Color color;
	OperationType drawMethod; //add
};


struct StyledGeometry
{
	StyledGeometry(Geometry* pGeometry, Style* pStyle )
	{
		this->pGeometry = pGeometry;
		this->pStyle = pStyle;
	}
	~StyledGeometry()
	{
		delete pGeometry;
		delete pStyle;
	}
	Geometry* pGeometry;
	Style* pStyle;
};

