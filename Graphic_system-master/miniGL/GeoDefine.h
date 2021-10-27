#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include "Graphic.h"

#include "State.h"
using namespace std;

#define  PI  3.14159265358979323846

//2D��
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

//2D��Χ��
template<typename T>
struct _Box2D
{
	_Box2D() { valid = false; }
	_Box2D(T xmin, T ymin, T xmax, T ymax)
	{
		setBox(xmin, ymin, xmax, ymax);
	}

	T width() { return _xmax - _xmin; }//��Χ�п��
	T height() { return _ymax - _ymin; }//��Χ�и߶�
	T centerX() { return (_xmin + _xmax) / 2; }//��Χ�����ĵ�x����
	T centerY() { return (_ymin + _ymax) / 2; }//��Χ�����ĵ�y����
	T xmin() { return _xmin; }
	T ymin() { return _ymin; }
	T xmax() { return _xmax; }
	T ymax() { return _ymax; }

	void setBox(T xmin, T ymin, T xmax, T ymax)//���ð�Χ�з�Χ
	{
		this->_xmin = xmin, this->_xmax = xmax, this->_ymin = ymin, this->_ymax = ymax;
		valid = true;
	}

	void setBox(_Box2D& box)
	{
		setBox(box._xmin, box._ymin, box._xmax, box._ymax);
	}

	//������ӵĵ���չ��Χ��
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

	//������ӵİ�Χ����չ��Χ��
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
	bool valid;//�Ƿ���Ч
};
typedef _Box2D<double> Box2D;

//ͼ�ζ�������
enum GeomType { gtUnkown = 0, gtPoint = 1, gtPolyline = 2, gtPolygon = 3 , gtSeedFill };

//ͼ�ζ�����࣬���̳�
struct Geometry
{
	virtual ~Geometry() {}
	virtual GeomType getGeomType() = 0; //��ȡͼ�ζ�������
	virtual Box2D getEnvelop() = 0;//��ȡͼ�ζ����Χ��
	string label;
};


//��ͼ�����
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

//��ͼ�ζ���
struct PolylineGeometry :Geometry
{
	virtual GeomType getGeomType() { return gtPolyline; }

	//��������أ���ȡ��i����
	Point2D& operator[](int i) { return pts[i]; }

	//��ӵ�
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
	//���е�
	vector<Point2D> pts;
	Box2D envelop;
};



//ͼ��
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
		for (size_t i = 0, size = geometrySet.size(); i < size; ++i) delete geometrySet[i];//��������ɾ������ͼ�ζ���
	}

	//��������أ����ص�i��ͼ�ζ���
	Geometry* operator[](int i) { return geometrySet[i]; }

	//���ͼ�����
	void addGeometry(Geometry* pGeometry, bool updateEnvelop = false)
	{
		geometrySet.push_back(pGeometry);
		if (updateEnvelop)
			envelop.expand(pGeometry->getEnvelop());
	}

	//����ͼ�㷶Χ
	void setEnvelop(double xmin, double ymin, double xmax, double ymax)
	{
		envelop.setBox(xmin, ymin, xmax, ymax);
	}

	//��ȡͼ�������ͼ�ζ�������
	int getGeometryCount() { return geometrySet.size(); }

	vector<Geometry*> geometrySet;//ͼ�ζ��󼯺�
	Box2D envelop;//ͼ�㷶Χ��Ӧ�İ�Χ��
	GeomType geomType;//ͼ������
};

//���ݼ�
struct Dataset
{
	virtual ~Dataset()
	{
		for (size_t i = 0, size = layerSet.size(); i < size; ++i) delete layerSet[i];//��������ɾ��ͼ��
	}

	//��������أ����ص�i��ͼ��
	Layer* operator[](int i) { return layerSet[i]; }

	int indexOf(Layer* pLayer) {
		for (int i = 0; i < layerSet.size(); ++i)
		{
			if (pLayer = layerSet[i]) return i;
		}
		return -1;
	}

	//��ȡͼ����
	int getLayerCount() { return layerSet.size(); }

	//���ͼ��
	void addLayer(Layer* pLayer)
	{
		layerSet.push_back(pLayer);
	}

	//ͼ�㼯��
	vector<Layer*> layerSet;
};

//�����ͼ�ζ���
struct PolygonGeometry :PolylineGeometry
{
	virtual GeomType getGeomType() { return gtPolygon; }
};
//���ӵ����
struct SeedFillGeometry :PointGemetry
{
	SeedFillGeometry(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	virtual GeomType getGeomType() { return gtSeedFill; }
};
//�����������
enum StyleType { stColor }; 


//�����������
struct Style
{
	virtual ~Style() {}
	virtual StyleType getStyleType() = 0; //��ȡͼ�ζ�������
};
//���Ƶķ�ʽ
enum OperationType {
	otNone, otRefreshWindow, otDrawRectangle, otDrawLineDDA, otDrawLine_Bresenham, otDrawPolyline, otDrawPolygon,
	otDrawCircle, otDrawEllipse, otFillPolygon, otFillPolygon_Border, otGridCell, otFillPolygon_FenceFill, otFillPolygon_EdgeTag,
	otBoundary_SeedFill, StackSeedFill4, SeedLineFill1, otFloodFill, otPan
};

struct ColorStyle : public Style    //�ڷ��������������һ�����Ʒ�ʽ��ѡ��
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

