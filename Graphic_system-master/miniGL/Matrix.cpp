#include "Matrix.h"


vector2D Matrix_Multiply( const vector2D& point, Matrix& newmatrix)
{
	vector2D newpoint;
	newpoint.x = point.x * newmatrix.matrix[0][0] + point.y * newmatrix.matrix[1][0] + point.h * newmatrix.matrix[2][0];
	newpoint.y = point.x * newmatrix.matrix[0][1] + point.y * newmatrix.matrix[1][1] + point.h * newmatrix.matrix[2][1];
	newpoint.h = point.x * newmatrix.matrix[0][2] + point.y * newmatrix.matrix[1][2] + point.h * newmatrix.matrix[2][2];
	return newpoint;
}

Vec4d Matrix4d::VecMultiplyMatrix(Vec4d& vec, Matrix4d matrix)
{
	Vec4d NewVec;
	double m[4][4];
	double x = vec.x, y = vec.y, z = vec.z, h = vec.h;
	//先转换为一个单独矩阵
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = matrix.Matrix[i][j];
		}
	}
	NewVec.x = x * m[0][0] + y * m[1][0] + z * m[2][0] + h * m[3][0];
	NewVec.y = x * m[0][1] + y * m[1][1] + z * m[2][1] + h * m[3][1];
	NewVec.z = x * m[0][2] + y * m[1][2] + z * m[2][2] + h * m[3][2];
	NewVec.h = x * m[0][3] + y * m[1][3] + z * m[2][3] + h * m[3][3];
	return NewVec;
}

void Matrix4d::GetCoordinates3D(Vec3d (&Vec)[8], Matrix4d& pMatrix, Vec3d(&p)[8])
{
	for (int i = 0; i < 8; ++i)
	{	
		Vec4d m = Vec[i];
		m = Matrix4d::VecMultiplyMatrix(m, pMatrix);
		double w = m.h;
		if ( w > 0 && m.x >= -w && m.x <= w && m.y >= -w && m.x <= w && m.z >= 0 && m.z <= w)
		{
			m.HomogeneousConvert();
			/*p[i].x = m.x;
			p[i].y = m.y;
			p[i].z = m.z;
			p[i].x = (p[i].x * 0.5 + 0.5) * getWindowWidth()/2;
			p[i].y = (p[i].y * 0.5 + 0.5) * getWindowHeight()/2;*/

			p[i].x = (m.x * 0.5 + 0.5) * getWindowWidth() / 2;
			p[i].y = (m.y * 0.5 + 0.5) * getWindowHeight() / 2;
			p[i].z = m.z * getWindowHeight() / 2;

		}
	}
}

static Vec3d VecArrayMultiplyMatrix(Vec3d(&vec)[8], Matrix4d matrix);


