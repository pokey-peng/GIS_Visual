#pragma once
#include "Graphic.h"
#include <math.h>
#include "GeoDefine.h"
class vector2D
{
public:
	vector2D()
	{
		x = y = h = 0;
	}
	vector2D(const Point2D& pt)
	{
		this->x = pt.x;
		this->y = pt.y;
		this->h = 1;
	}
	vector2D(PointGemetry& pt)
	{
		this->x = pt.x;
		this->y = pt.y;
		this->h = 1;
	}
	
	
	operator PixelPoint()
	{
		PixelPoint pt;
		pt.x = (int)x, pt.y = (int)y;
		return pt;
	}
	double x, y, h;
};


class Vec3d
{
public:
	double x, y, z;
	Vec3d()
	{
		x = y = z = 999;
	}
	~Vec3d() {}
	Vec3d(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	//向量的加法
	Vec3d operator + (Vec3d& Point3d)
	{
		Vec3d NewPoint3d;
		NewPoint3d.x = x + Point3d.x;
		NewPoint3d.y = y + Point3d.y;
		NewPoint3d.z = z + Point3d.z;
		return NewPoint3d;
	}
	//
	//向量的"-"
	Vec3d operator - (Vec3d& Point3d)
	{
		Vec3d NewPoint3d;
		NewPoint3d.x = x - Point3d.x;
		NewPoint3d.y = y - Point3d.y;
		NewPoint3d.z = z - Point3d.z;
		return NewPoint3d;
	}
	
	Vec3d operator  * (Vec3d& Point3d)
	{
		Vec3d NewPoint3d;
		NewPoint3d.x = x * Point3d.x;
		NewPoint3d.y = y * Point3d.y;
		NewPoint3d.z = z * Point3d.z;
		return NewPoint3d;
	}

	//向量的除法
	Vec3d operator / (double num)
	{
		Vec3d NewPoint3d;
		NewPoint3d.x = x / num;
		NewPoint3d.y = y / num;
		NewPoint3d.z = z / num;
		return NewPoint3d;
	}
	//
	//向量的叉乘
	//
	Vec3d VectorCrossProduct(Vec3d& Point3d)
	{
		Vec3d NewVector;
		double aX = x, aY = y, aZ = z, bX = Point3d.x, bY = Point3d.y, bZ = Point3d.z;
		NewVector.x = (aY * bZ) - (aZ * bY);
		NewVector.y = (aZ * bX) - (aX * bZ);
		NewVector.z = (aX * bY) - (aY * bX);
		return NewVector;
	}

	//向量的等于符号的
	/*Vec3d operator = (Vec3d point3D)
	{
		Vec3d NewVector;
		NewVector.x = point3D.x;
		NewVector.y = point3D.y;
		NewVector.z = point3D.z;
		return NewVector;

	}*/
	//
	//向量的单位化
	//
	void Normalize()
	{
		double LengthVector = sqrt(x * x + y * y + z * z);
		if (LengthVector != 0)
		{
			this->x = x / LengthVector;
			this->y = y / LengthVector;
			this->z = z / LengthVector;
		}
	}

	vector<Vec3d> CubeFace;  //三角形表面
	//将三维空间中的点集转换成带索引的面
	void ConvertToPloygon(Vec3d (&points)[], int n)
	{
		for (int i = 0; i < n; ++i)
		{
			//
		}
	}
	
};

class Vec4d : public Vec3d
{
public:
	int h; //构建齐次坐标最后的一位，默认为1
	Vec4d() 
	{
		this->h = 1;

	}
	~Vec4d(){}

	Vec4d(int x, int y, int z, int h)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->h = h;
	}
	Vec4d(Vec3d& point)
	{
		this->x = point.x;
		this->y = point.y;
		this->z = point.z;
		this->h = 1;
	}
	Vec4d operator += (const Vec4d& Pointz)
	{
		this->x = x + Pointz.x;
		this->y = y + Pointz.y;
		this->z = z + Pointz.z;
		this->h = h + Pointz.h;
		return *this;
	}
	Vec4d operator -= (const Vec4d& Pointz)
	{
		this->x = x - Pointz.x;
		this->y = y - Pointz.y;
		this->z = z - Pointz.z;
		this->h = h - Pointz.h;
		return *this;
	}
	Vec4d operator *= (const Vec4d& Pointz)
	{
		this->x = x * Pointz.x;
		this->y = y * Pointz.y;
		this->z = z * Pointz.z;
		this->h = h * Pointz.h;
		return *this;
	}
	Vec4d operator + (const Vec4d& Pointz)
	{
		Vec4d NewPointz;
		NewPointz.x = x + Pointz.x;
		NewPointz.y = y + Pointz.y;
		NewPointz.z = z + Pointz.z;
		NewPointz.h = h + Pointz.h;
		return NewPointz;
	}
	void HomogeneousConvert()
	{
		this->x = x / h;
		this->y = y / h;
		this->z = z / h;
		this->h = h / h;
	}
};





