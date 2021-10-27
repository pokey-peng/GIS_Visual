#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
using std::string;

#define  PI  3.14159265358979323846

//2D��
template<typename T>
struct _Point2D
{
	_Point2D(){ x = y = 0 ;}
	_Point2D( T x, T y ){ this->x = x, this->y = y; }

	T x,y;
};

typedef _Point2D<double> Point2D;

//2D��Χ��
template<typename T>
struct _Box2D
{
	_Box2D(){ valid = false; }
	_Box2D( T xmin,T ymin , T xmax, T ymax )
	{
		setBox(xmin, ymin, xmax, ymax);
	}

	T width(){ return _xmax - _xmin; }//��Χ�п��
	T height(){ return _ymax - _ymin; }//��Χ�и߶�
	T centerX(){ return (_xmin + _xmax) /2 ; }//��Χ�����ĵ�x����
	T centerY(){ return (_ymin + _ymax) /2 ; }//��Χ�����ĵ�y����
	T xmin(){ return _xmin;}
	T ymin(){ return _ymin;}
	T xmax(){ return _xmax;}
	T ymax(){ return _ymax;}

	void setBox( T xmin,T ymin , T xmax, T ymax )//���ð�Χ�з�Χ
	{
		this->_xmin = xmin, this->_xmax = xmax, this->_ymin = ymin, this->_ymax = ymax ;
		valid = true;
	}

	void setBox( _Box2D& box )
	{
		setBox( box._xmin, box._ymin, box._xmax, box._ymax );
	}

	//������ӵĵ���չ��Χ��
	void expand( T x, T y )
	{
		if( !valid ){
			_xmin = _xmax = x;
			_ymin = _ymax = y;
			valid = true;
		}
		else
		{
			_xmin = min( _xmin, x );
			_xmax = max( _xmax, x );
			_ymin = min( _ymin, y );
			_ymax = max( _ymax, y );
		}
	}

	//������ӵİ�Χ����չ��Χ��
	void expand( _Box2D<T>& box ){
		if (box.valid)
		{
			if ( !valid )
				setBox(box);
			else
			{
				expand(box.xmin(), box.ymin());
				expand(box.xmax(), box.ymax());
			}
		}
	}

protected:
	T _xmin,_ymin;
	T _xmax,_ymax;
	bool valid;//�Ƿ���Ч
};
typedef _Box2D<double> Box2D;

//ͼ�ζ�������
enum GeomType{ gtUnkown = 0, gtPoint = 1, gtPolyline = 2, gtPolygon = 3 , gtCircle , gtEllipse,gtRectangle };

//ͼ�ζ�����࣬���̳�
struct Geometry
{
	virtual ~Geometry(){}
	virtual GeomType getGeomType() =  0; //��ȡͼ�ζ�������
	string label;
};

struct Geometry2D :Geometry
{
	virtual Box2D getEnvelop() = 0;//��ȡͼ�ζ����Χ��
};

//��ͼ�����
struct PointGeometry:Geometry2D
{
	PointGeometry(  )
	{
		x = y = 0;
	}
	PointGeometry( double x, double y )
	{
		this->x= x;
		this->y = y;
	}

	virtual GeomType getGeomType(){ return gtPoint; }
	virtual Box2D getEnvelop(){ return Box2D( x,y,x,y ); }

	double x,y;
};

//��ͼ�ζ���
struct PolylineGeometry:Geometry2D
{
	virtual GeomType getGeomType(){ return gtPolyline; }

	//��������أ���ȡ��i����
	Point2D& operator[]( int i ){ return pts[i]; }

	//��ӵ�
	void addPoint( double x, double y )
	{
		pts.push_back( Point2D( x, y ));
		envelop.expand( x, y );
	}

	virtual Box2D getEnvelop(){ return envelop; }
	const vector<Point2D>& getPts() const 
	{
		return pts;
	}
protected:
	//���е�
	vector<Point2D> pts;
	Box2D envelop;
};

//�����ͼ�ζ���
struct PolygonGeometry:PolylineGeometry
{
	virtual GeomType getGeomType(){ return gtPolygon; }
};

//Բͼ�ζ���
struct CircleGeometry :Geometry2D
{
	CircleGeometry()
	{
		x = y = 0;
		r = 0;
	}
	CircleGeometry(double x, double y, double r )
	{
		this->x = x;
		this->y = y;
		this->r = r;
	}

	virtual GeomType getGeomType() { return gtCircle; }
	virtual Box2D getEnvelop() { return Box2D(x - r, y - r, x + r, y + r ); }

	double x, y;
	double r;
};

//��Բͼ�ζ���
struct EllipseGeometry :Geometry2D
{
	EllipseGeometry()
	{
		x1 = y1 = x2 = y2 = 0;
	}
	EllipseGeometry(double x1, double y1, double x2, double y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}

	virtual GeomType getGeomType() { return gtEllipse; }
	virtual Box2D getEnvelop() { return Box2D(x1, y1, x2, y2); }

	void getCenter(double& x, double& y) { x = (x1 + x2) * 0.5, y = (y1 + y2) * 0.5; }
	double getWidth() { return x2 - x1; }
	double getHeight() { return y2 - y1; }

	double x1, y1;
	double x2, y2;
};



//struct Symbol;
//struct SymbolGeometry: public Geometry
//{
//	SymbolGeometry( Geometry* pGeometry, Symbol* pSymbol )
//	{ 
//		this->pGeometry = pGeometry;
//		this->pSymbol = pSymbol;
//	}
//	virtual GeomType getGeomType(){ return pGeometry->getGeomType(); }
//
//	Geometry* pGeometry ;
//	Symbol* pSymbol;
//};

//ͼ��
struct Layer
{
	Layer()
	{
		geomType = gtUnkown;
	}

	Layer( GeomType geomType )
	{
		this->geomType = geomType;    
	}

	virtual ~Layer()
	{
		for( size_t i = 0 , size = geometrySet.size(); i < size ; ++i ) delete geometrySet[i];//��������ɾ������ͼ�ζ���
	}

	//��������أ����ص�i��ͼ�ζ���
	Geometry2D* operator[]( int i ){ return geometrySet[i]; }

	//���ͼ�����
	void addGeometry(Geometry2D* pGeometry, bool updateEnvelop = false )
	{
		geometrySet.push_back( pGeometry );
		if(updateEnvelop )
			envelop.expand( (Box2D&) pGeometry->getEnvelop());
	}

	//����ͼ�㷶Χ
	void setEnvelop( double xmin,double ymin , double xmax, double ymax  )
	{
		envelop.setBox( xmin, ymin,xmax,ymax );
	}

	//��ȡͼ�������ͼ�ζ�������
	int getGeometryCount(){ return geometrySet.size(); }	

	vector<Geometry2D*> geometrySet;//ͼ�ζ��󼯺�
	Box2D envelop;//ͼ�㷶Χ��Ӧ�İ�Χ��
	GeomType geomType;//ͼ������
};

//���ݼ�
struct Dataset
{
	virtual ~Dataset()
	{
		for( size_t i = 0, size = layerSet.size() ; i < size ; ++i ) delete layerSet[i];//��������ɾ��ͼ��
	}

	//��������أ����ص�i��ͼ��
	Layer* operator[]( int i ){ return layerSet[i]; }

	int indexOf( Layer* pLayer ){
		for ( int i = 0 ; i < layerSet.size(); ++i )
		{
			if( pLayer = layerSet[i]) return i;
		}
		return -1;
	}

	//��ȡͼ����
	int getLayerCount(){ return layerSet.size(); }	

	//���ͼ��
	void addLayer( Layer* pLayer )
	{
		layerSet.push_back( pLayer );
	}

	//ͼ�㼯��
	vector<Layer*> layerSet;
};






