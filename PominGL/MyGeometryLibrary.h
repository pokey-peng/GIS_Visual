#ifndef _MYGEOMETRYLIBRARY_H
#define _MYGEOMETRYLIBRARY_H

#include "MyGeoDefine.h"

struct PixelPoint;

class GeometryLibrary
{
public:
	static Geometry2D* createPointGeometry(double x, double y);
	static Geometry2D* createPolylineGeometry(PixelPoint* pts, int size,unsigned color);
	static Geometry2D* createPolylineGeometry(Point2D* pts, int size);
	static Geometry2D* createPolylineGeometry(PixelPoint& pt1, PixelPoint& pt2,unsigned color);
	static Geometry2D* createPolygonGeometry(PixelPoint* pts, int size);
	static Geometry2D* createPolygonOutlineGeometry(PixelPoint* pts, int size,unsigned color);
	static Geometry2D* createPolygonGeometry(Point2D* pts, int size);
	static Geometry2D* createPolygonOutlineGeometry(Point2D* pts, int size);
	static Geometry2D* createCircleGeometry(double x, double y, double r);
	static Geometry2D* createCircleGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* createCircleGeometry(double x1, double y1, double x2, double y2, unsigned color);
	static Geometry2D* createEllipseGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* createEllipseGeometry(double x1, double y1, double x2, double y2, unsigned color);
	static Geometry2D* creatRectangleGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* creatRectangleOutlineGeometry(double x1, double y1, double x2, double y2);
};

#endif // !_MYGEOMETRY_H