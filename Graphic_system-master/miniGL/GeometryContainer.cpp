#include "GeometryContainer.h"
#include "GeoDefine.h"

StyledGeometry* GeometryContainer::addGeometry(Geometry* pGeometry, Style* pStyle)
{
	StyledGeometry* pStyledGeometry = new StyledGeometry(pGeometry, pStyle);
	mGeometryContainer.push_back(pStyledGeometry);
	return pStyledGeometry;
}

GeometryContainer::~GeometryContainer()
{
	clear();
}

void GeometryContainer::clear()
{
	for (int i = 0; i < mGeometryContainer.size(); ++i)
	{
		delete mGeometryContainer[i];
	}
}


void GeometryContainer::ToGetObject(GeometryContainer& g_GeometryContainer)
{
	int count = getRubberPointCount();
	if (count  < 2)
		return;
	switch (g_State.OperationType)
	{
	case State::otDrawRectangle:
	case State::otDrawLineDDA:
	case State::otDrawLine_Bresenham:
	case State::otDrawCircle:
	case State::otDrawEllipse:
	{
		PixelPoint pt1, pt2;
		getRubberPoints(pt1, pt2);
		PolylineGeometry* pPolylineGeometry = new PolylineGeometry();
		pPolylineGeometry->addPoint(pt1.x, pt1.y);
		pPolylineGeometry->addPoint(pt2.x, pt2.y);
		ColorStyle* pStyle;
		if (g_State.OperationType == State::otDrawRectangle) //矩形
			pStyle = new ColorStyle(getPenColor(), otDrawRectangle);
		else if (g_State.OperationType == State::otDrawLineDDA) //DDA算法
			pStyle = new ColorStyle(getPenColor(), otDrawLineDDA); 
		else if (g_State.OperationType == otDrawLine_Bresenham)
			pStyle = new ColorStyle(getPenColor(), otDrawLine_Bresenham);
		else if (g_State.OperationType == State::otDrawCircle)
			pStyle = new ColorStyle(getPenColor(), otDrawCircle);
		else if (g_State.OperationType == otDrawEllipse)
			pStyle = new ColorStyle(getPenColor(), otDrawEllipse);
		//对加入的图形进行判断
		//vector<StyledGeometry*> container = g_GeometryContainer.mGeometryContainer;
		//if (container.size() > 1)
		//{
		//	
		//	for (int i = 0; i < container.size(); ++i)
		//	{
		//		StyledGeometry* pStyledGeometry = container[i];
		//		if (pStyledGeometry->pGeometry->getGeomType() == gtPolyline)
		//		{
		//			const vector<Point2D>& pts = pPolylineGeometry->getPts();
		//			if (pts[i].x == pt1.x && pts[i].y == pt1.y)
		//				flag = false;
		//		}		
		//	}
		//	if(flag)
		//		g_GeometryContainer.addGeometry(pPolylineGeometry, pStyle);
		//	flag = true;
		//}
		//else 
		////if (flag)
		g_GeometryContainer.addGeometry(pPolylineGeometry, pStyle);
		refreshWindow();
	}
	break;
	case State::otDrawPolyline:
	{
		PixelPoint* points = (PixelPoint*)calloc(getRubberPointCount(), sizeof(PixelPoint));
		getRubberPoints(points);
		PolylineGeometry* pPolylineGeometry = new PolylineGeometry();
		for (int i = 0; i < getRubberPointCount(); ++i)
		{	
			if (points)
					pPolylineGeometry->addPoint(points[i].x, points[i].y);			
		}
		ColorStyle* pStyle = new ColorStyle(getPenColor(), otDrawPolyline);
		g_GeometryContainer.addGeometry(pPolylineGeometry, pStyle);
		free(points);
		refreshWindow();
	}                                                                 
	break;
	case State::otDrawPolygon:
	case State::otFillPolygon_Border:
	case State::otFillPolygon_FenceFill:
	case State::otFillPolygon_EdgeTag:
	case State::otFillPolygon:
	{
		if (count < 3) return;
		vector<PixelPoint> points(count);
		getRubberPoints(points.data());

		PolygonGeometry* pPolygonGeometry = new PolygonGeometry();
		for (int i = 0; i < points.size(); ++i)
		{
			pPolygonGeometry->addPoint(points[i].x, points[i].y);
		}
		if ( g_State.OperationType != State::otFillPolygon)
			pPolygonGeometry->addPoint(points[0].x, points[0].y);	
		ColorStyle* pStyle;
		if (g_State.OperationType == State::otDrawPolygon)
			pStyle = new ColorStyle(getPenColor(), otDrawPolygon);
		else if(g_State.OperationType == State::otFillPolygon_Border)
			pStyle = new ColorStyle(getPenColor(), otFillPolygon_Border);
		else if (g_State.OperationType == State::otFillPolygon_FenceFill)
			pStyle = new ColorStyle(getPenColor(), otFillPolygon_FenceFill);
		else if (g_State.OperationType == State::otFillPolygon_EdgeTag)
			pStyle = new ColorStyle(getPenColor(), otFillPolygon_EdgeTag);
		else if (g_State.OperationType == State::otFillPolygon)
			pStyle = new ColorStyle(getPenColor(), otFillPolygon);
		g_GeometryContainer.addGeometry(pPolygonGeometry, pStyle);

		refreshWindow();
	}
	break;	
	case State::otBoundary_SeedFill:
	case State::StackSeedFill4:
	case State::SeedLineFill1:
	case State::otFloodFill:
	{
		ColorStyle* pStyle;
		PixelPoint pt1, pt2;
		getRubberPoints(pt1, pt2);
		SeedFillGeometry* pPolylineGeometry = new SeedFillGeometry(pt1.x, pt1.y);
		if (g_State.OperationType == otBoundary_SeedFill)
			pStyle = new ColorStyle(getPenColor(), otBoundary_SeedFill);
		if (g_State.OperationType == StackSeedFill4)
			pStyle = new ColorStyle(getPenColor(), StackSeedFill4);
		if (g_State.OperationType == SeedLineFill1)
			pStyle = new ColorStyle(getPenColor(), SeedLineFill1);
		if (g_State.OperationType == otFloodFill)
			pStyle = new ColorStyle(getPenColor(), otFloodFill);
		g_GeometryContainer.addGeometry(pPolylineGeometry, pStyle);
		refreshWindow();
	}
	break;
	}
}
