#pragma once
#include "Graphic.h"
#include <math.h>
#include "GeoDefine.h"
#include "Vec.h"
using namespace std;
enum SymmetricTransType {
	None, Translation, ScaleChange, RotationChange, DisjointChange, SymmetricOrigin, SymmetricX, SymmetricY, SymmetricXequalY, SymmetricXequa_Y
};
class Matrix
{
public:
	double matrix[3][3];
	Matrix()
	{
		setmatrix();
	}
	//���Ƚ���������Ϊ��λ����
	void setmatrix() 
	{

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == j)
					matrix[i][j] = 1;
				else
					matrix[i][j] = 0;
			}
		}
			
	}

	//�������� ��+��
	Matrix operator+(Matrix& b)
	{
		Matrix result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.matrix[i][j] = matrix[i][j] + b.matrix[i][j];
		return result;
	}
	//�������� ��-��
	Matrix operator-(Matrix& b)
	{
		Matrix result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.matrix[i][j] = matrix[i][j] - b.matrix[i][j];
		return result;
	}
	//�������� ��*��
	Matrix operator*(Matrix& b)
	{
		Matrix result;
		double sum = 0.0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				sum = 0.0;
				for (int k = 0; k < 3; k++)
					sum += matrix[i][k] * b.matrix[k][j];
				result.matrix[i][j] = sum;		
				
			}
		return result;
	}


	//ƽ�Ʊ任
	Matrix static Translation(double Tx, double Ty)
	{
		Matrix NewMatrix;
		NewMatrix.matrix[2][0] = Tx;
		NewMatrix.matrix[2][1] = Ty;
		return NewMatrix;
	}
	//�����任
	Matrix static ScaleChange(double S)
	{
		Matrix NewMatrix;
		//NewMatrix.matrix[2][2] = S;
		NewMatrix.matrix[0][0] *= S;
		NewMatrix.matrix[1][1] *= S;
		return NewMatrix;
	}
	//��ת�任
	Matrix static RotationChange(double Angle)
	{
		Matrix NewMatrix;
		NewMatrix.matrix[0][0] = cos(Angle / 180 * PI);
		NewMatrix.matrix[0][1] = sin(Angle / 180 * PI);
		NewMatrix.matrix[1][0] = -sin(Angle / 180 * PI);
		NewMatrix.matrix[1][1] = cos(Angle / 180 * PI);
		return NewMatrix;

	}
	//�ԳƱ任
	Matrix static SymmetryChange(SymmetricTransType Type)
	{
		Matrix NewMatrix;
		if (Type == None)
			return NewMatrix;
		if (Type == SymmetricOrigin)
			NewMatrix.matrix[0][0] = NewMatrix.matrix[1][1] = -1;
		if (Type == SymmetricX)
			NewMatrix.matrix[1][1] = -1;
		if (Type == SymmetricY)
			NewMatrix.matrix[0][0] = -1;
		if (Type == SymmetricXequalY)
		{
			NewMatrix.matrix[0][0] = NewMatrix.matrix[1][1] = 0;
			NewMatrix.matrix[0][1] = NewMatrix.matrix[1][0] = 1;
		}
		if (Type == SymmetricXequa_Y)
		{
			NewMatrix.matrix[0][0] = NewMatrix.matrix[1][1] = 0;
			NewMatrix.matrix[0][1] = NewMatrix.matrix[1][0] = -1;
		}
		return NewMatrix;
	}
	//���б任
	Matrix static DisjointChange(double Tb, double Tc)
	{
		Matrix NewMatrix;
		NewMatrix.matrix[0][1] = Tb;
		NewMatrix.matrix[1][0] = Tc;
		return NewMatrix;
	}

	Matrix static FixdePointToScale(double x, double y, double s)
	{
		Matrix NewMatrix;
		NewMatrix.matrix[0][0] = s;
		NewMatrix.matrix[1][1] = s;
		NewMatrix.matrix[2][0] = -x * s + x;
		NewMatrix.matrix[2][1] = -y * s + y;
		return NewMatrix;
	}
	Matrix static FixdePointToRotate(double x, double y, double alpha)
	{
		Matrix NewMatrix;
		NewMatrix.matrix[0][0] = cos(alpha);
		NewMatrix.matrix[0][1] = sin(alpha);
		NewMatrix.matrix[1][0] = -sin(alpha);
		NewMatrix.matrix[1][1] = cos(alpha);
		NewMatrix.matrix[2][0] = x - x * cos(alpha) + y * sin(alpha);
		NewMatrix.matrix[2][1] = y - y * cos(alpha) - x * sin(alpha);
		return NewMatrix;
	}
	Matrix static FixdePointToRotate90(double x, double y)
	{
		Matrix NewMatrix;
		NewMatrix.matrix[0][0] = 0;
		NewMatrix.matrix[0][1] = 1;
		NewMatrix.matrix[1][0] = -1;
		NewMatrix.matrix[1][1] = 0;
		NewMatrix.matrix[2][0] = x + y;
		NewMatrix.matrix[2][1] = y - x;
		return NewMatrix;
	}
};

class Matrix4d
{
public:
	double Matrix[4][4];
	Matrix4d()
	{
		ToSetMatrix();
	}
	~Matrix4d()
	{

	}
	void ToSetMatrix()
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
					Matrix[i][j] = 1;
				else
					Matrix[i][j] = 0;
			}
				
	}
	
	//
	//������ȫ������
	//
	void ClearMatrixToZero()
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				this->Matrix[i][j] = 0;
	}
	//
	//����ά����ƽ�ƾ���
	//
	static Matrix4d TanslateChange(double Tx, double Ty, double Tz)
	{
		Matrix4d NewMatrix;//�µķ��ؾ���
		NewMatrix.Matrix[3][0] = Tx;
		NewMatrix.Matrix[3][1] = Ty;
		NewMatrix.Matrix[3][2] = Tz;
		
		return NewMatrix;
	}
	//
	//��ά�ռ����겻ͬ��������任����
	//
	static Matrix4d ZoomInDifferentDirections(double Tx, double Ty, double Tz)
	{
		Matrix4d NewMatrix;//�µķ��ؾ���
		NewMatrix.Matrix[0][0] = Tx;
		NewMatrix.Matrix[1][1] = Ty;
		NewMatrix.Matrix[2][2] = Tz;
		return NewMatrix;
	}
	//
	//���巽�������
	//
	static Matrix4d ZoomOverallDirections(double Ts)
	{
		Matrix4d NewMatrix;//�µķ��ؾ���
		NewMatrix.Matrix[3][3] = Ts;
		return NewMatrix;
	}
	//
	//��ά�ռ�������ת�任����ѡ��������
	//������Χ��Z�������ת
	//
	static Matrix4d RotateAroundZaxis(double Angle)
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[0][0] = cos(Angle);
		NewMatrix.Matrix[0][1] = sin(Angle);
		NewMatrix.Matrix[1][0] = -sin(Angle);
		NewMatrix.Matrix[1][1] = cos(Angle);
		return NewMatrix;
	}
	//
	//��ά�ռ�������ת�任����ѡ��������
	//������Χ��x�������ת
	//
	static Matrix4d RotateAroundXaxis(double Angle)
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[1][1] = cos(Angle);
		NewMatrix.Matrix[1][2] = sin(Angle);
		NewMatrix.Matrix[2][1] = -sin(Angle);
		NewMatrix.Matrix[2][2] = cos(Angle);
		return NewMatrix;
	}
	//
	//��ά�ռ�������ת�任����ѡ��������
	//������Χ��Y�������ת
	//
	static Matrix4d RotateAroundYaxis(double Angle)
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[0][0] = cos(Angle);
		NewMatrix.Matrix[0][2] = -sin(Angle);
		NewMatrix.Matrix[2][0] = sin(Angle);
		NewMatrix.Matrix[2][2] = cos(Angle);
		return NewMatrix;
	}
	//
	//xoyƽ��ԳƱ任
	//
	static Matrix4d SymmetryByXoyPlane()
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[2][2] = -1;
		return NewMatrix;
	}
	//
	//��ά�ռ����yozƽ��Գ�
	//
	static Matrix4d SymmetryByYozPlane()
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[0][0] = -1;
		return NewMatrix;
	}
	//
	//��ά�ռ����zoxƽ��Գ�
	//
	static Matrix4d SymmetryByZoxPlane()
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[1][1] = -1;
		return NewMatrix;
	}
	//
	//����X����жԳƱ任
	//
	static Matrix4d SymmetryByXaxis()
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[1][1] = -1;
		NewMatrix.Matrix[2][2] = -1;
		return NewMatrix;
	}
	//
	//����Z����жԳƱ任
	//
	static Matrix4d SymmetryByZaxis()
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[0][0] = -1;
		NewMatrix.Matrix[1][1] = -1;
		return NewMatrix;
	}
	//
	//����Y����жԳƱ任
	//
	static Matrix4d SymmetryByYaxis()
	{
		Matrix4d NewMatrix;
		NewMatrix.Matrix[0][0] = -1;
		NewMatrix.Matrix[2][2] = -1;
		return NewMatrix;
	}
	//
	//�����۲�����ϵ
	void MakeViewMatrix(Vec3d& eye, Vec3d& center, Vec3d& up)
	{
		Vec3d u, v;
		Vec3d n = eye - center;
		n.Normalize();
		//��z�����up�õ�X������
		u = up.VectorCrossProduct(n);
		u.Normalize();
		v = n.VectorCrossProduct(u);
		v.Normalize();
		//��������x��
		this->Matrix[0][0] = u.x;
		this->Matrix[1][0] = u.y;
		this->Matrix[2][0] = u.z;

		this->Matrix[0][1] = v.x;
		this->Matrix[1][1] = v.y;
		this->Matrix[2][1] = v.z;

		this->Matrix[0][2] = n.x;
		this->Matrix[1][2] = n.y;
		this->Matrix[2][2] = n.z;

		this->Matrix[3][0] = -(eye * u).x;
		this->Matrix[3][1] = -(eye * v).y;
		this->Matrix[3][2] = -(eye * n).z;
	}
	//
	//ͶӰ���������
	//
	void MakePerspectiveMatrix(double fovy, double aspect, double zNear, double zFar)
	{
		double h = 2 * zNear * tan(fovy / 2);
		double w = 2 * zNear * aspect * tan(fovy / 2);
		this->Matrix[0][0] = 2 * zNear / w;
		this->Matrix[1][1] = 2 * zNear / h;
		this->Matrix[2][2] = -zFar / (zFar - zNear);
		this->Matrix[2][3] = -1;
		this->Matrix[3][2] = -zNear * zFar / (zFar - zNear);
		this->Matrix[3][3] = 0;		
	}
	//
	//���������"*="
	//
	void operator *= (Matrix4d& NextMatrix)
	{
		Matrix4d gNewMatrix;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				gNewMatrix.Matrix[i][j] = 0;
				for (int k = 0; k < 4; ++k)
				{
					gNewMatrix.Matrix[i][j] += Matrix[i][k] * NextMatrix.Matrix[k][j];
				}
				gNewMatrix.Matrix[i][j] = gNewMatrix.Matrix[i][j];
			}
			
		}
		//return gNewMatrix;
	}
	//
	//���������"*"
	//
	Matrix4d operator*(Matrix4d& NextMatrix)
	{
		Matrix4d gNewMatrix;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				gNewMatrix.Matrix[i][j] = 0;
				for (int k = 0; k < 4; ++k)
				{
					gNewMatrix.Matrix[i][j] += Matrix[i][k] * NextMatrix.Matrix[k][j];
				}
			}
		}
		return gNewMatrix;
	}

	//��ά�������ͱ任�������
	static Vec4d VecMultiplyMatrix(Vec4d& vec, Matrix4d matrix);

	//����ͶӰ�任�õ��µ���ά����
	static void GetCoordinates3D(Vec3d (&Vec)[8], Matrix4d& pMatrix, Vec3d(&p)[8]);


};



//���������Ծ���õ����ĵ�����
vector2D Matrix_Multiply( const vector2D& point, Matrix& newmatrix);





