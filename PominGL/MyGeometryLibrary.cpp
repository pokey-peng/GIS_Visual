#include "MyGeometryLibrary.h";
#include "Graphic.h";

Geometry2D* GeometryLibrary::createPointGeometry(double x, double y)
{
	return new PointGeometry(x, y);
}

Geometry2D* GeometryLibrary::createPolylineGeometry(PixelPoint* pts, int size,unsigned color)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	pGeometry->color = color;
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolylineGeometry(Point2D* pts, int size)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolylineGeometry(PixelPoint& pt1, PixelPoint& pt2,unsigned color)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	pGeometry->addPoint(pt1.x, pt1.y);
	pGeometry->addPoint(pt2.x, pt2.y);
	pGeometry->color = color;
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonGeometry(PixelPoint* pts, int size)
{
	PolygonGeometry* pGeometry = new PolygonGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonOutlineGeometry(PixelPoint* pts, int size,unsigned color)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	pGeometry->addPoint(pts[0].x, pts[0].y);
	pGeometry->color = color;
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonGeometry(Point2D* pts, int size)
{
	PolygonGeometry* pGeometry = new PolygonGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonOutlineGeometry(Point2D* pts, int size)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	pGeometry->addPoint(pts[0].x, pts[0].y);
	return pGeometry;
}

Geometry2D* GeometryLibrary::createCircleGeometry(double x, double y, double r)
{
	return new CircleGeometry(x, y, r);
}

Geometry2D* GeometryLibrary::createCircleGeometry(double x1, double y1, double x2, double y2)
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	return new CircleGeometry(x1, y1, sqrt(dx * dx + dy * dy));
}

Geometry2D* GeometryLibrary::createCircleGeometry(double x1, double y1, double x2, double y2, unsigned color)
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	return new CircleGeometry(x1, y1, sqrt(dx * dx + dy * dy),color);
}

Geometry2D* GeometryLibrary::createEllipseGeometry(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	return new EllipseGeometry(x1, y1, x2, y2);
}

Geometry2D* GeometryLibrary::createEllipseGeometry(double x1, double y1, double x2, double y2, unsigned color)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	return new EllipseGeometry(x1, y1, x2, y2,color);
}

Geometry2D* GeometryLibrary::creatRectangleGeometry(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	PolygonGeometry* pGeometry = new PolygonGeometry();
	pGeometry->addPoint(x1, y1);
	pGeometry->addPoint(x2, y1);
	pGeometry->addPoint(x2, y2);
	pGeometry->addPoint(x1, y2);
	return pGeometry;
}

Geometry2D* GeometryLibrary::creatRectangleOutlineGeometry(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	PolylineGeometry* pGeometry = new PolylineGeometry();
	pGeometry->addPoint(x1, y1);
	pGeometry->addPoint(x2, y1);
	pGeometry->addPoint(x2, y2);
	pGeometry->addPoint(x1, y2);
	pGeometry->addPoint(x1, y1);

	return pGeometry;
}
