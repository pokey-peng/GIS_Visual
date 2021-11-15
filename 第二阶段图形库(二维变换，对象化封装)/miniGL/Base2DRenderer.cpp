#include "Base2DRenderer.h"
#include "GeoDefine.h"
#include "Graphic.h"
#include "Raster.h"
#include "State.h"

Base2DRenderer::Base2DRenderer(void)
{
}

Base2DRenderer::~Base2DRenderer(void)
{
}

void Base2DRenderer::init(Dataset *pDataset)
{
	mpDataset = pDataset;
	mInited = true;
}

void Base2DRenderer::render()
{
	if (g_State.DrawMode == State::dmGrid)
	{
		Raster::drawGrid();
	}

	if (mInited == false)
		return;

	for (int i = mpDataset->getLayerCount() - 1; i >= 0; --i)
	{
		Layer *pLayer = (*mpDataset)[i];
		renderLayer(pLayer);
	}
}

void Base2DRenderer::renderLayer(Layer *pLayer)
{
	unsigned color = getLayerColor(pLayer);

	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
	{
		renderGeometry((*pLayer)[i], color);
	}
}

void Base2DRenderer::renderGeometry(Geometry *pGeometryDef, Color color)
{
	switch (pGeometryDef->getGeomType())
	{
	case gtPoint:
	{
		PointGeometry *pGeometry = (PointGeometry *)pGeometryDef;
		//Raster::drawPoint( pGeometry->x, pGeometry->y );
		setPixel(pGeometry->x, pGeometry->y, color); //绘制

		//Vec3 v(pGeometry->x, pGeometry->y);//由原始坐标构建齐次坐标
		//Vec3 v2 = v * mTransformMatrix;//变换
		//setPixel(v2.x(), v2.y(), color);//绘制
	}
	break;
	case gtPolyline:
	{
		PolylineGeometry *pGeometry = (PolylineGeometry *)pGeometryDef;
		const vector<Point2D> &pts = pGeometry->getPts();
		static vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());
		for (int j = 0, ptsCount = pts.size(); j < ptsCount; ++j)
		{
			Point2D &pt = (*pGeometry)[j]; //获得线顶点的原始坐标
			setPixel(pt.x, pt.y, color);   //绘制
										   //Raster::drawLine(pts[i].x, pts[i].y, pts[(i + 1)].x, pts[(i + 1)].y,color);

			//Vec3 v(pt.x, pt.y);//由原始坐标构建齐次坐标
			//Vec3 v2 = v * mTransformMatrix;//变换
			//rasterPts[j].x = v2.x, rasterPts[j].y = v2.y;
		}
		//Raster::drawPolyline(rasterPts.data(), rasterPts.size(), color);
	}
	break;
	case gtPolygon:
	{
		PolygonGeometry *pGeometry = (PolygonGeometry *)pGeometryDef;
		const vector<Point2D> &pts = pGeometry->getPts();
		for (int j = 0, ptsCount = pGeometry->getPts().size(); j < ptsCount; ++j)
		{
			Point2D &pt = (*pGeometry)[j]; //获得多边形顶点的原始坐标
			setPixel(pt.x, pt.y, color);   //绘制

			//Vec3 v(pt.x, pt.y);//由原始坐标构建齐次坐标
			//Vec3 v2 = v * mTransformMatrix;//变换
			//setPixel(v2.x(), v2.y(), color);//绘制
		}
		//Raster::drawPolygon(pts2.data(), pts2.size(), color );
	}
	break;
	case gtCircle:
	{
		CircleGeometry *pGeometry = (CircleGeometry *)pGeometryDef;
		//Raster::drawCircle(pGeometry->x, pGeometry->y, pGeometry->r, getPenColor());
	}
	break;

	case gtEllipse:
	{
		EllipseGeometry *pGeometry = (EllipseGeometry *)pGeometryDef;
		double x, y;
		pGeometry->getCenter(x, y);
		//Raster::drawEllipses(x, y, pGeometry->getWidth(), pGeometry->getHeight(), getPenColor());
	}
	break;
	}
}

unsigned Base2DRenderer::getLayerColor(Layer *pLayer)
{
	return getPenColor();
}

//void Base2DRenderer::reset()
//{
//	mTransformMatrix.makeIdentity();
//}
//
//void Base2DRenderer::translate(double dx, double dy)
//{
//	mTransformMatrix *= Matrix3::translate(dx, dy);
//}
//
//void Base2DRenderer::rotate(double degree)
//{
//	double rad = degree * PI / 180;
//	mTransformMatrix *= Matrix3::rotate(rad);
//}
//
//void Base2DRenderer::rotate(double x, double y, double degree)
//{
//	mTransformMatrix *= Matrix3::translate(-x, -y);
//	rotate(degree);
//	mTransformMatrix *= Matrix3::translate(x, y);
//}
//
//void Base2DRenderer::scale(double dx, double dy)
//{
//	mTransformMatrix *= Matrix3::scale(dx, dy);
//}
//
//
//void Base2DRenderer::scale(double x, double y, double dx, double dy)
//{
//	mTransformMatrix *= Matrix3::translate(-x, -y);
//	scale(dx, dy);
//	mTransformMatrix *= Matrix3::translate(x, y);
//}
