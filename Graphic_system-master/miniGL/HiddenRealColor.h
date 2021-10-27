/*****************************************************
*  ����άͼ�ε������㷨ʵ�֡�
*   ѧ�ţ�10180345
*   ������κ���
*   ���ݣ�
*	1.ͼ��ռ������㷨����Ȼ������㷨������ɨ�����㷨����������㷨������ ϸ���㷨
*	2.����ռ������㷨��BSP�㷨����������������㷨
*	3.��������֮���㷨����������㷨�������㷨��������ϸ���㷨������ͶӰ�㷨
*	
******************************************************/
#include "Vec.h"
#include <vector>
#include <math.h>
#include<algorithm>
#include <queue>
using namespace std;
Vec3d NewGraph3D[8];//��άͼ�� ������

#define PMAXW 600  //��Χ���
#define PMAXH 600  //��Χ�߶�
const int INF = 9999;        //���ֵ

Vec3d Ocenter(50, 50, 50);  //��ά�ռ��е����� 
Vec3d eye(50, 50, 400);

//���建�����ṹ
typedef struct
{
	Color color = 99;      //��ɫ�����
	double deepth = -INF; //zֵ�����

}gPixelBuc;

//�������������˽ṹ
//tpsFace == three points faces
struct tpsFace
{
	Vec4d point3D[3];  
	int size = 3;

}CubeFace[12];  //��������12����������ɵ���

gPixelBuc g_PixelBuc[PMAXW][PMAXH];



struct InformPt
{
	Vec3d point;
	Color Light;
};

//����ɨ�����㷨�߱�
struct Edge
{
	double x, dx, dy;  //dx��dy���������������ȵ�ֵ
	int polygonID;  //ָ�����ε�ָ��


	//������groud
	InformPt points[2];
};
//����ɨ�����㷨����α�
struct Poly
{
	int ploygonID; //����ε�ID
	double A, B, C, D; //�ռ�ƽ�淽��ϵ�� Ax + By + Cz +D = 0
	int dy; //ɨ���߿�Խ�ķ�Χ
	Color color;  //��ɫ
	bool flag; //�ж϶������������
};


vector<vector<Edge>> Etable;  //����ɨ�����㷨�߱�
vector<vector<Poly>> Ptable;   //����ɨ�����㷨����α�
vector<Edge> AET;  //���Ա�
vector<Poly> APT;  //���Զ����
//���彻������ݽṹ
struct cPoint
{
	double zleft, zright;
	int order;
};



//�����㷨
struct DpFace :tpsFace
{
	Color color;
	double zmin,zmax;
	//����������ķ���ϵ��
	double a, b, c, d;
};


//��ͬ�������㷨
class Hiddens
{
public:

	/*************************************************����Z_Buffer�㷨ʵ�֡���***********************/	//����ά������ĵ㼯ת��Ϊ������ļ���
	static void ptsConvertToFace(Vec3d(&points3D)[8], tpsFace(&faces)[12])
	{
		//����
		faces[0].point3D[0] = points3D[0]; faces[0].point3D[1] = points3D[1]; faces[0].point3D[2] = points3D[2];
		faces[1].point3D[0] = points3D[0]; faces[1].point3D[1] = points3D[2]; faces[1].point3D[2] = points3D[3];
		//�����
		faces[2].point3D[0] = points3D[0]; faces[2].point3D[1] = points3D[1]; faces[2].point3D[2] = points3D[5];
		faces[3].point3D[0] = points3D[0]; faces[3].point3D[1] = points3D[4]; faces[3].point3D[2] = points3D[5];
		//��ǰ��
		faces[4].point3D[0] = points3D[1]; faces[4].point3D[1] = points3D[2]; faces[4].point3D[2] = points3D[6];
		faces[5].point3D[0] = points3D[1]; faces[5].point3D[1] = points3D[5]; faces[5].point3D[2] = points3D[6];
		//�Ҳ���
		faces[6].point3D[0] = points3D[2]; faces[6].point3D[1] = points3D[3]; faces[6].point3D[2] = points3D[7];
		faces[7].point3D[0] = points3D[2]; faces[7].point3D[1] = points3D[6]; faces[7].point3D[2] = points3D[7];
		//������
		faces[8].point3D[0] = points3D[0]; faces[8].point3D[1] = points3D[3]; faces[8].point3D[2] = points3D[7];
		faces[9].point3D[0] = points3D[0]; faces[9].point3D[1] = points3D[4]; faces[9].point3D[2] = points3D[7];
		//����
		faces[10].point3D[0] = points3D[4]; faces[10].point3D[1] = points3D[5]; faces[10].point3D[2] = points3D[6];
		faces[11].point3D[0] = points3D[4]; faces[11].point3D[1] = points3D[6]; faces[11].point3D[2] = points3D[7];

		
	}

	//��ȡ������ĵ����z
	static double ToGetDeepthZ(Vec4d pt0, Vec4d pt1, Vec4d pt2, int x, int y)
	{
		//����ԭ��-�����Ĳ��ԭ��
		//| i      j    k   |
		//|x2-x1 y2-y1 z2-z1| //mx my mz
		//|x3-x1 y3-y1 z3-z1| //nx ny nz
		//ƽ�淽��
		//Ax + By + Cz +D = 0

		double mx = pt1.x - pt0.x;
		double my = pt1.y - pt0.y;
		double mz = pt1.z - pt0.z;

		double nx = pt2.x - pt0.x;
		double ny = pt2.y - pt0.y;
		double nz = pt2.z - pt0.z;

		double A = my * nz - ny * mz;
		double B = nx * mz - mx * nz;
		double C = mx * ny - nx * my;
		double D = -(A * pt0.x + B * pt0.y + C * pt0.z);

		if (C == 0)
		{
			return -INF;
		}

		//�������
		return (A * x + B * y + D) / (-C);

	}

	//Z_Buffer�㷨ʵ��
	static void Z_Buffer(tpsFace &pface, Color gcolor)
	{
		Vec4d face[3];
		for (int i = 0; i < 3; ++i)
		{
			face[i] = pface.point3D[i];
		}

		int miny = INF, maxy = -INF;
		int size = pface.size; //һ����ĵ���
		//������Ҫ���أ���Ļ������ķ�Χ ��yΪ׼
		for (int i = 0; i < size; ++i)
		{
			if (face[i].y > maxy)
				maxy = face[i].y;
			if (face[i].y < miny)
				miny = face[i].y;
		}

		//�����еĵ���б���,���ɨ�����ϵ�������Ϣ
		for (int y = miny; y < maxy; ++y)
		{
			//����һ�����������洢ɨ���߽�����Ϣ
			vector<int> gvec;
			for (int i = 0; i < size; ++i)
			{
				if (face[i].y == y)
				{

					gvec.push_back(face[i].x);  //����ɨ���ߵ�ʱ��
					if ((face[(i + 1) % size].y - face[i].y) * (face[(i - 1 + size) % size].y - face[i].y) > 0)
					{
						gvec.push_back(face[i].x); //�������ཻ �������������

					}
				}
				if ((face[i].y < y && face[(i + 1) % size].y > y) || (face[i].y > y && face[(i + 1) % size].y < y))
				{
					if (face[i].y - face[(i + 1) % size].y != 0)
					{
						int x = face[i].x + (y - face[i].y) * (face[(i + 1) % size].x - face[i].x) / (face[(i + 1) % size].y - face[i].y);
						gvec.push_back(x);
					}
				}
			}
			//���������㡰��������
			sort(gvec.begin(), gvec.end());
			//������ȱȽ�-�������ϵ�
			for (int i = 0; i < gvec.size(); i = i + 2)
			{
				for (int j = gvec[i]; j < gvec[i + 1]; ++j)
				{
					double deepz = ToGetDeepthZ(face[0], face[1], face[2], j, y);
					if (j < PMAXW && y < PMAXH)    //��ֹ����Խ��
					{
						if (deepz > g_PixelBuc[j][y].deepth)
						{
							g_PixelBuc[j][y].deepth = deepz;
							g_PixelBuc[j][y].color = gcolor;
						}
					}					
				}
			}
		}
	}

	//��Ļ���س�ʼ��
	static void initialization()
	{
		for (int i = 0; i < PMAXW; ++i)
		{
			for (int j = 0; j < PMAXH; ++j)
			{
				g_PixelBuc[i][j].color = INF;
				g_PixelBuc[i][j].deepth = -INF;
			}
		}
	}

	//����
	static void drawPixels()
	{
		int indexx = 0;   //�������ݣ����ǽ������63������
		for (int x = 0; x < PMAXW; ++x)
		{
			for (int y = 0; y < PMAXH; ++y)
			{
				switch (g_PixelBuc[x][y].color)
				{
				case INF:
					setPixel(x, y, _RGB(255, 255, 255)); break;
				case 0:
				case 1:
					setPixel(x, y, _RGB(0, 255, 255)); break;
				case 2:
				case 3:
					setPixel(x, y, _RGB(255, 0, 255)); break;
				case 4:
				case 5:
					setPixel(x, y, _RGB(255, 255, 0)); break;
				case 6:
				case 7:
					setPixel(x, y, _RGB(255, 0, 255)); break;
				case 8:
				case 9:
					setPixel(x, y, _RGB(255, 255, 0)); break;
				case 10:
				case 11:
					indexx++;
					setPixel(x, y, _RGB(0, 0, 0)); break;
				default:
					setPixel(x, y, _RGB(255, 255, 255)); break;
				}
			}
		}
	}
	/*************************************************����Z_Buffer�㷨ʵ�֡���***********************/
	/*************************************************��������ɨ�����㷨ʵ�֡���********************************/

	//static vector<vector<Edge>> Etable;  //����ɨ�����㷨�߱�
	//static vector<vector<Poly>> Ptable;   //����ɨ�����㷨����α�
	//static vector<vector<Edge>> AET;  //���Ա�
	//static vector<vector<Edge>> EPT;  //���Զ����
	//��ǰ����Ϊȫ�ֱ����ɣ�/??
	
	//1.������
	static void  buildtable(tpsFace (&faces)[12])
	{
		//�����ߵı����Ƚ�֮ǰ��vector���������龡����գ���ֹ�ڴ�ռ��
		Etable.clear();
		Ptable.clear();
		Etable.resize(getWindowHeight()/2);
		Ptable.resize(getWindowHeight()/2);
		int facessize = 12;  //������������ĸ���

		for (int i = 0; i < facessize; ++i)
		{
			//�����ߵı�
			Edge edge;
			float miny = INF;
			float maxy = 0;
			//ѭ��ÿһ�����ÿһ����
			for (int j = 0; j < faces[i].size; ++j)
			{
				edge.polygonID = i;
				//edge.falg = 0;��Ҫ?
				//�ҵ�ÿһ���ߵĶ�������
				//���Ա�����ƣ��ҵ�yֵ�ϴ󣬲���������ĵ���Ϊ��� 
				Vec4d c1 = faces[i].point3D[j];
				Vec4d c2 = faces[i].point3D[(j + 1) % faces[i].size];

				if (c1.y < c2.y)
				{
					edge.x = c2.x;
					//dx ������б�ʵ������෴����y����1 Ҳ����y+1ʱ�������
					edge.dy = ceil(c2.y) - ceil(c1.y);
					if (edge.dy == 0)
						continue;
					edge.dx = (c1.x - c2.x) / edge.dy;
					Etable[ceil(c2.y)].push_back(edge);
				}
				else   
				{
					edge.x = c1.x;
					edge.dy = ceil(c1.y) - ceil(c2.y);
					if (edge.dy == 0)continue;
					edge.dx = (c2.x - c1.x) / edge.dy;
					Etable[ceil(c1.y)].push_back(edge);
				}
				miny = miny < c1.y ? miny : c1.y;
				maxy = maxy > c1.y ? maxy : c1.y;
			}


			//��ʼ��������α�
			if (maxy == 0) continue;
			
			Poly poly;
			poly.ploygonID = i;
			poly.dy = ceil(maxy) - ceil(miny);
			GetABC(faces[i], poly.A, poly.B, poly.C,poly.D);
			poly.color = i;
			poly.flag = 0;
			Ptable[ceil(maxy)].push_back(poly);
		}	
	}
	//��ȡ�淽�̵�A��B��C����
	static void GetABC(tpsFace& face, double& a, double& b, double& c,double& d)
	{
		Vec4d pt0, pt1, pt2;
		pt0 = face.point3D[0];
		pt1 = face.point3D[1];
		pt2 = face.point3D[2];
		double mx = pt1.x - pt0.x;
		double my = pt1.y - pt0.y;
		double mz = pt1.z - pt0.z;

		double nx = pt2.x - pt0.x;
		double ny = pt2.y - pt0.y;
		double nz = pt2.z - pt0.z;

        a = my * nz - ny * mz;
		b = nx * mz - mx * nz;
		c = mx * ny - nx * my;

		d = -(a * pt0.x + b * pt0.y + c * pt0.z);
	}

	//���»��
	static void updateActiveEdge()
	{
		//�жϻ��Զ���Σ����ɨ�����˵Ļ���ɾ��
		int n2 = 0;
		for (vector<Edge>::iterator it = AET.begin(); it != AET.end(); ++it)
		{
			(*it).dy--;
			if ((*it).dy == 0) continue;
			else
			{
				(*it).x += (*it).dx;
				*(AET.begin() + n2) = *(it);
				n2++;  //n2��Ҫ+1

			}
		}
		AET.resize(n2);	
	}

	//���»�����
	static void updateActivePolygon()
	{
		int n2 = 0;
		for (vector<Poly>::iterator it = APT.begin(); it != APT.end(); ++it)
		{
			(*it).dy--;
			if ((*it).dy == 0)continue;
			else
			{
				*(APT.begin() + n2) = *(it);
				n2++;
			}
		}
		APT.resize(n2);
	}

	//�����������ɫ
	static void calculateColor(int y, vector<Color> &buffer)
	{
		double left = 0, right;
		//�������εĸ���
		int count = 0;
		buffer.resize(getWindowWidth() / 2);

		for (int n = 0; n < AET.size(); ++n)
		{
			right = AET[n].x;
			if (left >= right)
			{
				count = updateFlagByID(AET[n].polygonID) ? count + 1 : count - 1;
				continue;
			}
			//����������ɫֵ����Ϊ0
			Color pcolor = INF;
			//���count��ֵ������Ϊ0��Ҳ��˵�ڣ�left,right)���������Ķ���εĸ���Ϊ0
			if (count == 0)
			{
				//��������������ֵ������Ϊ����ֵΪ0
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = INF;
				}
			}
			//������������1�������
			else if (count == 1)
			{
				pcolor = pixelByID(count, left, y);
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = pcolor;
				}
			}
			else //�������ж������ε�ʱ��
			{
				//���㽻��
				//���������㣬�ɶ�
				
				vector<cPoint> points;//����һ��vector�����洢points				
				Poly p1, p2;//�о����������
				vector<double> cross;//�����xֵ



				//���Զ�����������ѭ��
				//�����������ҵ����ֵ�������������Ķ������APT����λ�ü�¼����
				for (int i = 0; i < APT.size(); ++i)
				{
					if (points.size() == count)break;   //���ֵĶ���θ���
					if (APT[i].flag && APT[i].C != 0)
					{
						//ÿһ���������һ���Ĵ���
						double zleft = -(APT[i].A * left + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						double zright = -(APT[i].A * right + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						points.push_back({ zleft,zright,i});
					}
				}

				//�����Ƿ񽻲棬����Ļ��������㣬���ҽ��������points��
				for (int i = 0; i < points.size(); ++i)
				{
					//���Ҷ˵�Ĳ�ֵ�����෴��ʱ�򣬴�����ֹᴩ������
					for (int j = i + 1; j < points.size(); ++j)
					{
						if ((points[i].zleft - points[j].zleft) * (points[i].zright - points[j].zright) < 0)
						{

							p1 = APT[points[i].order];
							p2 = APT[points[j].order];
							//���㽻��
							//A1x + B1x + C1x + D1 =0
							//A2x + B2x + C2x + D2 =0
							if((p1.C * p2.A - p2.C * p1.A) != 0)
								cross.push_back(((p2.C * p1.B - p1.C * p2.B) * y + p2.C * p1.D - p1.C * p2.D) / (p1.C * p2.A - p2.C * p1.A));
						}						
					}
				}

				//���ڽ���������������ջ
				cross.push_back(right);
				sort(cross.begin(), cross.end()); //�����������
				for (int i = 0; i < cross.size(); ++i)
				{
					right = cross[i];
					pcolor = pixelByID(count, left, y);
					for (double pixel = left; left < right; ++left)
					{
						buffer[left] = pcolor;   //����ط��ڵ���65��ʱ��ᷢ������Խ��
					}
				}
			}
			//�����ұߵı������Ķ���α�־
			count = updateFlagByID(AET[n].polygonID) ? count + 1 : count - 1;
			left = right;//�л�����һ������		
		}
		for (double pixel = left; left < getWindowWidth()/2 -1; ++left)
		{
			buffer[left] = INF;
		}
	}

	static int updateFlagByID(int id)
	{
		for (vector<Poly>::iterator it = APT.begin(); it != APT.end(); ++it)
		{
			if ((*it).ploygonID == id)
			{
				(*it).flag = !(*it).flag;
				return (*it).flag;
			}
		}
	}
	//ɨ��
	static void scanLine(vector<vector<Color>> &buffer)
	{
		AET.clear();
		APT.clear();
		buffer.clear();
		//�������Ա߱����ϵ���ɨ��
		//�ߵ�ָ����ָ��y��С�ķ�������x+dx����y-1��ֵ
		buffer.resize(getWindowHeight() / 2);

		//����ɨ��ѭ��
		for (int y = getWindowHeight() / 2-1; y >= 0; --y)
		{
			//����µĻ��Ա�
			for (int n = 0; n < Etable[y].size(); ++n)
			{
				AET.push_back(Etable[y][n]);
			}
			//����µĻ�Ķ����
			for (int n = 0; n < Ptable[y].size(); ++n)
			{
				APT.push_back(Ptable[y][n]);
			}
			//�Ի��Ա߽�������
			if (!AET.empty())   //vector�ǿ�
				SortActiveEdge();
			calculateColor(y, buffer[y]);

			//�Ի��Ա���и��£�������һ��ɨ����
			updateActivePolygon();
			updateActiveEdge();
		}
	}

	//sort����
	//void sort(** first, ** last, **comp)
	//first�� Ҫ������������ʼ��ַ
	//last �� ���һ�����ݵĵ�ַ
	//comp ��������ķ�����ʵ��
	static void SortActiveEdge()
	{
		sort(AET.begin(), AET.end(),comp);
	}

	static bool comp(const Edge& a, const Edge& b)
	{
		return a.x < b.x;
	}

	
	//���㽻�����ε�zֵ�Լ�color
	static Color pixelByID(int count,double x,double y)
	{
		Color color = 0;
		y = y - 0.5; //ɨ���ߵ�������

		double maxz = -999, z;
		for (vector<Poly>::iterator it = APT.begin(); count > 0 && it != APT.end(); ++it)
		{
			//���жϽ�������Ķ����
			if ((*it).flag)
			{
				//����Ƚ��������ߵ�zֵ
				z = -((*it).A * x + (*it).B * y + (*it).D) / (*it).C;
				if (z > maxz)
				{
					maxz = z;
					color = (*it).color;
				}
				count--;
			}
		}
		return color;
	}


	//static int indexx = 0;
	//���л�ͼ��ɫ
	static void draw(vector<vector<Color>>& buffer)
	{
		//int indexx = 0;   //indexx���ս��Ϊ0
		for (int y = 0; y < buffer.size(); ++y)
		{
			for (int x = 0; x < buffer[y].size(); ++x)
			{
				switch (buffer[y][x])
				{
				case INF:
					setPixel(x, y, _RGB(255, 255, 255)); break;
				case 0:
				case 1:
					setPixel(x, y, _RGB(0, 255, 255)); break;
				case 2:
				case 3:
					setPixel(x, y, _RGB(255, 0, 255)); break;
				case 4:
				case 5:
					setPixel(x, y, _RGB(0, 255, 100)); break;
				case 6:
				case 7:
					setPixel(x, y, _RGB(0, 100, 255)); break;
				case 8:
				case 9:
					
					setPixel(x, y, _RGB(0, 255, 0)); break;
				case 10:
				case 11:
					
					setPixel(x, y, _RGB(0, 0, 255)); break;
				default:
					setPixel(x, y, _RGB(0, 0, 0)); break;
				}
			}
		}
	}

	/****************************************��������ɨ�����㷨����********************************/


	/********************************************** ���� �����㷨 ����   *************************/

	

	static bool zType(DpFace& a, DpFace& b)
	{
		return a.zmin < b.zmin;  //����z�Ĵ�С��������
	}

	static void JudgeOverlap(DpFace& b, DpFace& a)
	{

	}


	/********************************************** ���� �����㷨 ����   *************************/
};



/****************************************************************************************************/
//����㶨��ǿ�Ķ���λ����µ���Ľṹ��
struct Lightface
{
	int polyID;   //���ID���
	double A, B, C, D;  //�淽�̵�ϵ��
	Vec3d spmv;   //��ķ�����
	Color color;   //�����ɫҲ���Եı��
	Vec4d points[3]; //�������������������洢

};
Lightface LightFace[12];

//��Ҫ���¶���һ��vector�����洢�µĹ���ģ���µ�ÿ�����ص���ɫֵ
vector<vector<Color>> newBuffer;

/*********g���ղ���ģ��*************************/
//ѡ��ΪRGBģ��
//1.���廷����Ĺ���ǿ��
static double iaR, iaG, iaB;
//2.����������ǿ��
static double ipR, ipG, ipB;
//3.���廷���ⷴ��ϵ��
static double Kar, Kag, Kab;
//4.����������ϵ��
static double Kdr, Kdg, Kdb;
//5.���徵�淴��ϵ��
static double Ksr, Ksg, Ksb;

/************************************************/
struct rgb
{
	double r, g, b;
};
//����ÿ������Լ��ǵ����Ϣ
struct PtVector
{
	int polyID;   //�ö��������Ķ����
	int indexID;  //����������е�˳�������
	Vec3d point;
	Vec3d vec;
	Color light;   //��������Ĺ���ǿ��
	rgb RGB;
};
struct GrdFaces
{
	int polyID;   //���ID
	double A, B, C, D;  //һ����ķ��̵�ϵ��
	Vec3d spmv;  //ÿ�����
	Color color;  //ÿ�������ɫ
	PtVector points[3];   //һ�������������
	int size = 3;  //��ʾһ�����еĵ������
};
GrdFaces groundface[12];

//��ʵ����ɫ����ͼ�ε���
class RealisticColor
{
public:
	RealisticColor();
	
	//�㶨��ǿ�Ķ���λ���
	static void ConstLightIntensity(tpsFace(&faces)[12])
	{
		//����ص�ϵ�����и�ֵ����
		iaR = iaG = iaB = 0.5;   //֮ǰ���õ���0.8
		ipR = ipG = ipB = 0.8;
		Kar = Kag = Kab = 0.1;
		Kdr = Kdg = Kdb = 0.2;
		Ksr = Ksg = Ksb = 0.5;
		Vec3d Light(-1000,-1000,-990);

		//���ȼ���ÿһ����������Ϣ
		int size = 12;
		for (int i = 0; i < size; ++i)
		{
			LightFace->polyID = LightFace->color = i;
			Hiddens::GetABC(faces[i], LightFace[i].A, LightFace[i].B, LightFace[i].C, LightFace[i].D);
			//�������������������Ϣ�洢����Ҫ�о��Ĺ���ģ����ȥ
			int pointsize = 3;
			for (int j = 0; j < pointsize; ++j)
			{
				LightFace[i].points[j] = faces[i].point3D[j];
			}

			//�洢��ķ�������Ϣ
			LightFace[i].spmv.x = LightFace[i].A;
			LightFace[i].spmv.y = LightFace[i].B;
			LightFace[i].spmv.z = LightFace[i].C;

			//���¶���һ������Ϊͼ������ָ��һ����Ľǵ������
			Vec3d tempVec = Ocenter - LightFace[i].points[0];

			//�ж��Ƿ�ָ�����������棬������ǣ���ô���ظ÷������ĸ����������
			if (!SpMVdrector(LightFace[i].spmv, tempVec))
			{
				LightFace[i].spmv.x *= -1;
				LightFace[i].spmv.y *= -1;
				LightFace[i].spmv.z *= -1;
			}
		}


		//��������ɨ�����㷨������������㣬�õ��������ͼ��
		vector<vector<Color>> ColorPixels;  //���塰ȫ�ֵ�����ӳ���ϵ
		Hiddens::buildtable(CubeFace); //���������
		Hiddens::scanLine(ColorPixels); //����ɨ������������

		

		//Ȼ��������ڵ����ؽ�����ͨ��ģ�ͽ�ģ����
		int sizey = ColorPixels.size();
		newBuffer.resize(ColorPixels.size());
		for (int y = 0; y < sizey; ++y)
		{
			int sizex = ColorPixels[y].size(); //ɨ�����ϵ�x��Χֵ

			for (int x = 0; x < sizex; ++x)
			{
				//���õڶ��ַ�ʽ���������е�ÿһ�����ؽ���ɨ��
				//RGB�ֿ���
				double IR = ToGetLightFaces(x, y, ColorPixels[y][x],State::cRGB_R, Light);
				double IG = ToGetLightFaces(x, y, ColorPixels[y][x], State::cRGB_G, Light);
				double IB = ToGetLightFaces(x, y, ColorPixels[y][x], State::cRGB_B, Light);
				newBuffer[y].push_back(RGB(IR, IG, IB));	
				setPixel(x, y, RGB(IR*255, IG*255, IB*255));
			}
		}

	}

	static double DotProduct(Vec3d m, Vec3d n)
	{
		return m.x * n.x + m.y * n.y + m.z * n.z;
	}
	
	static bool SpMVdrector(Vec3d m, Vec3d n)
	{
		//�ж�2�����������Ƿ�ͬ��
		//ͬ�� ����Ϊtrue ���� ����Ϊfalse
		if ((m.x * n.x + m.y * n.y + m.z * n.z) > 0)
			return true;
		return false;
	}

	static double GetZdeep(double A, double B, double C, double D, double x, double y)
	{
		if (C == 0)
		{
			return -INF;
		}

		//�������
		return (A * x + B * y + D) / (-C);
	}

	//�������
	static double GetDistance(Vec3d& Light, Vec3d& Targrt, Vec3d& eye)
	{
		double d1 = sqrt(pow(Targrt.x - Light.x, 2) + pow(Targrt.y - Light.y, 2) + pow(Targrt.z - Light.z, 2));
		double d2 = sqrt(pow(Targrt.x - eye.x, 2) + pow(Targrt.y - eye.y, 2) + pow(Targrt.z - eye.z, 2));
		return d1 + d2;
	}

	//����ÿһ��С���صĹ���ֵ
	static double ToGetLightFaces(double x, double y, Color& pixel, State::ColorMode ColorMode, Vec3d Light)
	{
		double Ic, Id, Is;
		int n = 5;    //n�����������⻬���йص�һ������������Խ�⻬��n��ֵ��Խ��
		double Ivalue;
		
		if (pixel == INF)
			return 1;
		else
		{
			//�Է������ķ�������ж�

			Vec3d N = LightFace[pixel].spmv;  //��ķ���ʸ��

		//�����Դ����������������
		//�����������x,y����ά�ռ�����
			double z = GetZdeep(LightFace[pixel].A, LightFace[pixel].B, LightFace[pixel].C, LightFace[pixel].D, x, y);
			
			//��ǿ˥��
			double fd,c0,c1,c2; //���ڹ�ǿ��˥������
			c0 = 0.8;
			c1 = 0.0000001;
			c2 = 0.000000001;
			Vec3d Target(x, y, z);
			//��������ž����˥��
			double distance = GetDistance(Light, Target, eye);
			double disDecay = 1 / (c0 + c1 * distance + c2 * pow(distance, 2));
			fd = min(1, distance);
			//L����
			Vec3d L(x - Light.x, y - Light.y, z - Light.z);

			//�������������е�λ��
			N.Normalize();
			L.Normalize();

			//���徵�浯�����߷����������
			Vec3d V(eye.x - x, eye.y - y, eye.z - z);
			V.Normalize();

			Vec3d H; //���������������

			double angle;
			//��������ؽ����ж�
			switch (ColorMode)
			{
			case State::cRGB_R:
				//1.��⻷�������
				Ic = iaR * Kar;

				//2.����������
				angle = acos(DotProduct(L,N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipR * Kdr * (DotProduct(L, N));

				//3.��⾵�淴���
				//PS,����ķ����Գ���̫�����ˣ�����
				//�����ú����ƽ����������� &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipR * Ksr * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_G:
				//1.��⻷�������
				Ic = iaG * Kag;

				//2.����������
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipG * Kdg * (DotProduct(L, N));

				//3.��⾵�淴���
				//PS,����ķ����Գ���̫�����ˣ�����
				//�����ú����ƽ����������� &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipG * Ksg * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_B:

				//1.��⻷�������
				Ic = iaB * Kab;

				//2.����������
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipB * Kdb * (DotProduct(L, N));

				//3.��⾵�淴���
				//PS,����ķ����Գ���̫�����ˣ�����
				//�����ú����ƽ����������� &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipB * Ksb * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			default:
				return 1;
				break;
			}
		}
	}


	//Ground��������
	//����



	static void GroundRealColor(tpsFace(&faces)[12])
	{

		//��ÿһ������б����Ƚ���������
		//����ص�ϵ�����и�ֵ����
		iaR = iaG = iaB = 3;
		ipR = ipG = ipB = 0.8;
		Kar = Kag = Kab = 0.1;
		Kdr = Kdg = Kdb = 0.2;
		Ksr = Ksg = Ksb = 0.5;
		Vec3d Light(-1000, -1000, -990);

		//���ȼ���ÿһ����������Ϣ
		int fssize = 12;
		for (int i = 0; i < fssize; ++i)
		{
			groundface->polyID = groundface->color = i;
			Hiddens::GetABC(faces[i], groundface[i].A, groundface[i].B, groundface[i].C, groundface[i].D);
			//�������������������Ϣ�洢����Ҫ�о��Ĺ���ģ����ȥ
			int pointsize = 3;
			for (int j = 0; j < pointsize; ++j)
			{
				groundface[i].points[j].polyID = i;  //ÿһ�����������Ķ����
				groundface[i].points[j].indexID = j;   //��ȡ����������е������±�
				groundface[i].points[j].point = faces[i].point3D[j];
			}

			//�洢��ķ�������Ϣ
			groundface[i].spmv.x = groundface[i].A;
			groundface[i].spmv.y = groundface[i].B;
			groundface[i].spmv.z = groundface[i].C;

			//���¶���һ������Ϊͼ������ָ��һ����Ľǵ������
			Vec3d tempVec = Ocenter - groundface[i].points[0].point;

			//�ж��Ƿ�ָ�����������棬������ǣ���ô���ظ÷������ĸ����������
			if (!SpMVdrector(groundface[i].spmv, tempVec))
			{
				groundface[i].spmv.x *= -1;
				groundface[i].spmv.y *= -1;
				groundface[i].spmv.z *= -1;
			}
			for (int j = 0; j < 3; ++j)
			{
				groundface[i].points[j].vec = groundface[i].spmv; //�õ�ÿ��������ÿһ����ķ�����ʸ��
				groundface[i].points[j].vec.Normalize(); //�����㴦�ķ�����ʸ�����е�λ��
			}
			
		}
		


		//����ÿһ������ζ����ƽ������ʸ��
		//��ÿ������������ķ�ʽ���д洢����
		vector<vector<PtVector>> SamePoints;   //b����ÿһ����
		int ptsize = 8, psize = 3;
		SamePoints.clear();	
		SamePoints.resize(ptsize);
		for (int i = 0; i < ptsize; ++i)
		{
			SamePoints[i].clear();
		}
		for (int i = 0; i < ptsize; ++i)
		{
			for (int j = 0; j < fssize; ++j)
			{
				for (int k = 0; k < psize; ++k)
				{
					Vec3d tempv = groundface[j].points[k].point;
					double tx = tempv.x, ty = tempv.y, tz = tempv.z;
					if ((NewGraph3D[i].x == tx)&& (NewGraph3D[i].y == ty)&& (NewGraph3D[i].z == tz))
					{
						SamePoints[i].push_back(groundface[j].points[k]);
					}
				}
			}
		}

		//�������е�ÿһ����������ƽ������
		for (int i = 0; i < SamePoints.size(); ++i)
		{
			Vec3d m(0,0,0); //����һ���յ�����
			for (int j = 0; j < SamePoints[i].size(); ++j)
			{
				m = m + SamePoints[i][j].vec;
			}
			m = m / SamePoints[i].size();

			for (int j = 0; j < SamePoints[i].size(); ++j)
			{
				SamePoints[i][j].vec = m;
			}
		}


		//���ñ����ˣ�ֱ����ɨ�����㷨�������
		//���в�ֵ����,���õ�����ɨ���߷�������
		//��ͼ����������任
		//��������ɨ�����㷨������������㣬�õ��������ͼ��
		vector<vector<Color>> ColorPixels;  //���塰ȫ�ֵ�����ӳ���ϵ
		Hiddens::buildtable(CubeFace); //���������
		Hiddens::scanLine(ColorPixels); //����ɨ������������
		//���Ѿ��任�õ����»��㵽�浱��ȥ
		for (int i = 0; i < ptsize; ++i)
		{
			for (int j = 0; j < psize; ++j) //ÿһ�����е����������Ϣ
			{
				//���㶥��ĵ�λƽ��������ʸ��
				for (int m = 0; m < SamePoints.size(); ++m)
				{
					for (int n = 0; n < SamePoints[m].size(); ++n)
					{
						if (SamePoints[m][n].polyID == i && SamePoints[m][n].indexID == j)
						{
							groundface[i].points[j].vec = SamePoints[m][n].vec;
							groundface[i].points[j].vec.Normalize();
						}
					}
				}

				//���������Ĺ���ǿ��
				Vec3d tempoint = groundface[i].points[j].point;
				double x = tempoint.x, y = tempoint.y;
				double IR = ToGetGroundFaces(x, y, groundface[i].color, State::cRGB_R, Light, groundface[i].points[j].vec);
				double IG = ToGetGroundFaces(x, y, groundface[i].color, State::cRGB_G, Light, groundface[i].points[j].vec);
				double IB = ToGetGroundFaces(x, y, groundface[i].color, State::cRGB_B, Light, groundface[i].points[j].vec);
				groundface[i].points[j].RGB.r = IR;
				groundface[i].points[j].RGB.g = IG;
				groundface[i].points[j].RGB.b = IB;
			}
		}

		
		
		

		//���������ҿ�����ʾ�Ķ����
		vector<Color> hpolyID;
		for (int i = 0; i < ColorPixels.size(); ++i)
		{
			for (int j = 0; j < ColorPixels[i].size(); ++j)
			{
				if (cheak(ColorPixels[i][j], hpolyID))
				{
					hpolyID.push_back(ColorPixels[i][j]);
				}
			}
		}


		//��ÿһ���߽���ɨ��ת��
		vector<vector< PtVector>> section;
		section.clear();

		int ymax = getWindowHeight() / 2;
		for (int y = ymax; y > 0; --y)
		{
			//��ÿһ����ı߽�����⽻��
			for (int i = 0; i < fssize; ++i)
			{
				vector< PtVector> sec;
				double xleft, xright;
				Color Is, It;
				gettwolight(groundface[i], y, sec);
				if (sec.size() != 0)
					section.push_back(sec);
			}
		}

		for (int i = 0; i < section.size(); ++i)
		{
			for (int j = 0; j < section[i].size(); ++j)
			{
				for (int x = section[i][j].point.x; x < section[i][section[i].size() - 1].point.x; x++)
				{
					double tisR = section[i][j].RGB.r,titR = section[i][section[i].size() - 1].RGB.r;
					double tisG = section[i][j].RGB.g, titG = section[i][section[i].size() - 1].RGB.g;
					double tisB = section[i][j].RGB.b, titB = section[i][section[i].size() - 1].RGB.b;

					//�����м�������
					Vec3d stratpt = section[i][j].point, endpt = section[i][section[i].size() - 1].point;
					double IpR = 0, IpG = 0, IpB = 0;
					if (endpt.x - stratpt.x != 0)
					{
						 IpR = (endpt.x - x) / (endpt.x - stratpt.x) * tisR + (x - stratpt.x) / (endpt.x - stratpt.x) * titR;
						 IpG = (endpt.x - x) / (endpt.x - stratpt.x) * tisG + (x - stratpt.x) / (endpt.x - stratpt.x) * titG;
						 IpB = (endpt.x - x) / (endpt.x - stratpt.x) * tisB + (x - stratpt.x) / (endpt.x - stratpt.x) * titB;
					}					
					setPixel(x, stratpt.y, RGB(IpR * 255, IpG *255, IpB*255));
				}				
			}			
		}
	}
	
	static void gettwolight(GrdFaces face, double y, vector< PtVector>& InfoPL)
	{
		//������
		//static void Z_Buffer(tpsFace &pface, Color gcolor)

		
		InfoPL.clear();

		for (int i = 0; i < face.size; ++i)
		{
			Vec3d p1 = face.points[i].point, p2 = face.points[(i+1)%face.size].point;
			if ((p1.y > y && p2.y < y) || (p1.y < y && p2.y > y))
			{
				PtVector target;
				if (p1.x == p2.x)
				{
					target.point = Vec3d(p1.x, y, 0);
					target.RGB.r = (target.point.y - p2.y) / (p1.y - p2.y) * face.points[i].RGB.r
						+ (p1.y - target.point.y) / (p1.y - p2.y) * face.points[(i + 1) % face.size].RGB.r;
					target.RGB.g = (target.point.y - p2.y) / (p1.y - p2.y) * face.points[i].RGB.g
						+ (p1.y - target.point.y) / (p1.y - p2.y) * face.points[(i + 1) % face.size].RGB.g;
					target.RGB.b = (target.point.y - p2.y) / (p1.y - p2.y) * face.points[i].RGB.b
						+ (p1.y - target.point.y) / (p1.y - p2.y) * face.points[(i + 1) % face.size].RGB.b;
					InfoPL.push_back(target);
				}
				else
				{
					double k = (p2.y - p1.y) / (p2.x - p1.x);
					double x = (y - p1.y) / k + p1.x;
					target.point = Vec3d(x, y, 0);
					target.RGB.r = (target.point.y - p2.y) / (p1.y - p2.y) * face.points[i].RGB.r
						+ (p1.y - target.point.y) / (p1.y - p2.y) * face.points[(i + 1) % face.size].RGB.r;
					target.RGB.g = (target.point.y - p2.y) / (p1.y - p2.y) * face.points[i].RGB.g
						+ (p1.y - target.point.y) / (p1.y - p2.y) * face.points[(i + 1) % face.size].RGB.g;
					target.RGB.b = (target.point.y - p2.y) / (p1.y - p2.y) * face.points[i].RGB.b
						+ (p1.y - target.point.y) / (p1.y - p2.y) * face.points[(i + 1) % face.size].RGB.b;
					InfoPL.push_back(target);

				}
			}
		}

		//����
		if (InfoPL.size() == 2)
		{
			if (InfoPL[0].point.x > InfoPL[1].point.x)
			{
				PtVector tem;
				tem = InfoPL[0];
				InfoPL[0] = InfoPL[1];
				InfoPL[1] = tem;

			}
		}
		
	}

	//Ground�㷨����ÿһ�����������Ķ��㴦�Ĺ���ǿ��
	static double ToGetGroundFaces(double x, double y, Color pixel, State::ColorMode ColorMode, Vec3d Light,Vec3d& tarv)
	{
		double Ic, Id, Is;
		int n = 5;    //n�����������⻬���йص�һ������������Խ�⻬��n��ֵ��Խ��
		double Ivalue;

		if (pixel == INF)
			return 1;
		else
		{
			//�Է������ķ�������ж�

			Vec3d N = tarv;  //��ķ���ʸ��

		//�����Դ����������������
		//�����������x,y����ά�ռ�����
			double z = GetZdeep(groundface[pixel].A, groundface[pixel].B, groundface[pixel].C, groundface[pixel].D, x, y);

			//��ǿ˥��
			double fd, c0, c1, c2; //���ڹ�ǿ��˥������
			c0 = 0.8;
			c1 = 0.0000001;
			c2 = 0.000000001;
			Vec3d Target(x, y, z);
			//��������ž����˥��
			double distance = GetDistance(Light, Target, eye);
			double disDecay = 1 / (c0 + c1 * distance + c2 * pow(distance, 2));
			fd = min(1, distance);
			//L����
			Vec3d L(x - Light.x, y - Light.y, z - Light.z);

			//�������������е�λ��
			N.Normalize();
			L.Normalize();

			//���徵�浯�����߷����������
			Vec3d V(eye.x - x, eye.y - y, eye.z - z);
			V.Normalize();

			Vec3d H; //���������������

			double angle;
			//��������ؽ����ж�
			switch (ColorMode)
			{
			case State::cRGB_R:
				//1.��⻷�������
				Ic = iaR * Kar;

				//2.����������
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipR * Kdr * (DotProduct(L, N));

				//3.��⾵�淴���
				//PS,����ķ����Գ���̫�����ˣ�����
				//�����ú����ƽ����������� &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipR * Ksr * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_G:
				//1.��⻷�������
				Ic = iaG * Kag;

				//2.����������
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipG * Kdg * (DotProduct(L, N));

				//3.��⾵�淴���
				//PS,����ķ����Գ���̫�����ˣ�����
				//�����ú����ƽ����������� &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipG * Ksg * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_B:

				//1.��⻷�������
				Ic = iaB * Kab;

				//2.����������
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipB * Kdb * (DotProduct(L, N));

				//3.��⾵�淴���
				//PS,����ķ����Գ���̫�����ˣ�����
				//�����ú����ƽ����������� &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipB * Ksb * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			default:
				return 1;
				break;
			}
		}
	}
	
	//�ж�Vector������û�������ɫ
	static bool cheak(Color color, vector<Color>& vec)
	{
		for (int i = 0; i < vec.size(); ++i)
		{
			if (color == vec[i] || color == INF)
				return false;
		}
		return true;
	}
	
	//����ɨ���
	//1.������
	static void  buildtableGround(GrdFaces (&faces)[12])
	{
		//�����ߵı����Ƚ�֮ǰ��vector���������龡����գ���ֹ�ڴ�ռ��
		Etable.clear();
		Ptable.clear();
		Etable.resize(getWindowHeight() / 2);
		Ptable.resize(getWindowHeight() / 2);
		int facessize = 12;  //������������ĸ���

		for (int i = 0; i < facessize; ++i)
		{
			//�����ߵı�
			Edge edge;
			float miny = INF;
			float maxy = 0;
			//ѭ��ÿһ�����ÿһ����
			for (int j = 0; j < faces[i].size; ++j)
			{
				edge.polygonID = i;
				//edge.falg = 0;��Ҫ?
				//�ҵ�ÿһ���ߵĶ�������
				//���Ա�����ƣ��ҵ�yֵ�ϴ󣬲���������ĵ���Ϊ��� 
				Vec4d c1 = faces[i].points[j].point;
				Vec4d c2 = faces[i].points[(j + 1) % faces[i].size].point;

				if (c1.y < c2.y)
				{
					edge.x = c2.x;
					//dx ������б�ʵ������෴����y����1 Ҳ����y+1ʱ�������
					edge.dy = ceil(c2.y) - ceil(c1.y);
					if (edge.dy == 0)
						continue;
					edge.dx = (c1.x - c2.x) / edge.dy;
					edge.points[0].point = c1;
					edge.points[1].point = c2;
					edge.points[0].Light = faces[i].points[j].light;
					edge.points[1].Light = faces[i].points[(j + 1) % faces[i].size].light;
					Etable[ceil(c2.y)].push_back(edge);
				}
				else
				{
					edge.x = c1.x;
					edge.dy = ceil(c1.y) - ceil(c2.y);
					if (edge.dy == 0)continue;
					edge.dx = (c2.x - c1.x) / edge.dy;
					edge.points[1].point = c1;
					edge.points[0].point = c2;
					edge.points[1].Light = faces[i].points[j].light;
					edge.points[0].Light = faces[i].points[(j + 1) % faces[i].size].light;
					Etable[ceil(c1.y)].push_back(edge);
				}
				miny = miny < c1.y ? miny : c1.y;
				maxy = maxy > c1.y ? maxy : c1.y;
			}
			//��ʼ��������α�
			if (maxy == 0) continue;

			Poly poly;
			poly.ploygonID = i;
			poly.dy = ceil(maxy) - ceil(miny);
			/*GetABC(faces[i], poly.A, poly.B, poly.C, poly.D);*/

			poly.A = groundface[i].A;
			poly.B = groundface[i].B;
			poly.C = groundface[i].C;
			poly.D = groundface[i].D;

			poly.color = i;
			poly.flag = 0;
			Ptable[ceil(maxy)].push_back(poly);
		}
	}


	//���»��
	static void updateActiveEdge()
	{
		//�жϻ��Զ���Σ����ɨ�����˵Ļ���ɾ��
		int n2 = 0;
		for (vector<Edge>::iterator it = AET.begin(); it != AET.end(); ++it)
		{
			(*it).dy--;
			if ((*it).dy == 0) continue;
			else
			{
				(*it).x += (*it).dx;
				*(AET.begin() + n2) = *(it);
				n2++;  //n2��Ҫ+1

			}
		}
		AET.resize(n2);
	}

	//���»�����
	static void updateActivePolygon()
	{
		int n2 = 0;
		for (vector<Poly>::iterator it = APT.begin(); it != APT.end(); ++it)
		{
			(*it).dy--;
			if ((*it).dy == 0)continue;
			else
			{
				*(APT.begin() + n2) = *(it);
				n2++;
			}
		}
		APT.resize(n2);
	}

	//�����������ɫ
	static void calculateColor(int y, vector<Color>& buffer)
	{



		double left = 0, right;
		//�������εĸ���
		int count = 0;
		buffer.resize(getWindowWidth() / 2);

		for (int n = 0; n < AET.size(); ++n)
		{
			right = AET[n].x;
			if (left >= right)
			{
				count = updateFlagByID(AET[n].polygonID) ? count + 1 : count - 1;
				continue;
			}
			//����������ɫֵ����Ϊ0
			Color pcolor = INF;
			//���count��ֵ������Ϊ0��Ҳ��˵�ڣ�left,right)���������Ķ���εĸ���Ϊ0
			if (count == 0)
			{
				//��������������ֵ������Ϊ����ֵΪ0
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = INF;
				}
			}
			//������������1�������
			else if (count == 1)
			{
				pcolor = pixelByID(count, left, y);
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = pcolor;


				}
			}
			else //�������ж������ε�ʱ��
			{
				//���㽻��
				//���������㣬�ɶ�

				vector<cPoint> points;//����һ��vector�����洢points				
				Poly p1, p2;//�о����������
				vector<double> cross;//�����xֵ



				//���Զ�����������ѭ��
				//�����������ҵ����ֵ�������������Ķ������APT����λ�ü�¼����
				for (int i = 0; i < APT.size(); ++i)
				{
					if (points.size() == count)break;   //���ֵĶ���θ���
					if (APT[i].flag && APT[i].C != 0)
					{
						//ÿһ���������һ���Ĵ���
						double zleft = -(APT[i].A * left + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						double zright = -(APT[i].A * right + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						points.push_back({ zleft,zright,i });
					}
				}

				//�����Ƿ񽻲棬����Ļ��������㣬���ҽ��������points��
				for (int i = 0; i < points.size(); ++i)
				{
					//���Ҷ˵�Ĳ�ֵ�����෴��ʱ�򣬴�����ֹᴩ������
					for (int j = i + 1; j < points.size(); ++j)
					{
						if ((points[i].zleft - points[j].zleft) * (points[i].zright - points[j].zright) < 0)
						{

							p1 = APT[points[i].order];
							p2 = APT[points[j].order];
							//���㽻��
							//A1x + B1x + C1x + D1 =0
							//A2x + B2x + C2x + D2 =0
							if ((p1.C * p2.A - p2.C * p1.A) != 0)
								cross.push_back(((p2.C * p1.B - p1.C * p2.B) * y + p2.C * p1.D - p1.C * p2.D) / (p1.C * p2.A - p2.C * p1.A));
						}
					}
				}

				//���ڽ���������������ջ
				cross.push_back(right);
				sort(cross.begin(), cross.end()); //�����������
				for (int i = 0; i < cross.size(); ++i)
				{
					right = cross[i];
					pcolor = pixelByID(count, left, y);
					for (double pixel = left; left < right; ++left)
					{
						buffer[left] = pcolor;   //����ط��ڵ���65��ʱ��ᷢ������Խ��
					}
				}
			}
			//�����ұߵı������Ķ���α�־
			count = updateFlagByID(AET[n].polygonID) ? count + 1 : count - 1;
			left = right;//�л�����һ������		
		}
		for (double pixel = left; left < getWindowWidth() / 2 - 1; ++left)
		{
			buffer[left] = INF;
		}
	}

	static int updateFlagByID(int id)
	{
		for (vector<Poly>::iterator it = APT.begin(); it != APT.end(); ++it)
		{
			if ((*it).ploygonID == id)
			{
				(*it).flag = !(*it).flag;
				return (*it).flag;
			}
		}
	}
	//ɨ��
	static void scanLineGround(vector<vector<Color>>& buffer)
	{
		AET.clear();
		APT.clear();
		buffer.clear();
		//�������Ա߱����ϵ���ɨ��
		//�ߵ�ָ����ָ��y��С�ķ�������x+dx����y-1��ֵ
		buffer.resize(getWindowHeight() / 2);

		//����ɨ��ѭ��
		for (int y = getWindowHeight() / 2 - 1; y >= 0; --y)
		{
			//����µĻ��Ա�
			for (int n = 0; n < Etable[y].size(); ++n)
			{
				AET.push_back(Etable[y][n]);
			}
			//����µĻ�Ķ����
			for (int n = 0; n < Ptable[y].size(); ++n)
			{
				APT.push_back(Ptable[y][n]);
			}
			//�Ի��Ա߽�������
			if (!AET.empty())   //vector�ǿ�
				SortActiveEdge();
			calculateColor(y, buffer[y]);

			//�Ի��Ա���и��£�������һ��ɨ����
			updateActivePolygon();
			updateActiveEdge();
		}
	}

	//sort����
	//void sort(** first, ** last, **comp)
	//first�� Ҫ������������ʼ��ַ
	//last �� ���һ�����ݵĵ�ַ
	//comp ��������ķ�����ʵ��
	static void SortActiveEdge()
	{
		sort(AET.begin(), AET.end(), comp);
	}

	static bool comp(const Edge& a, const Edge& b)
	{
		return a.x < b.x;
	}


	//���㽻�����ε�zֵ�Լ�color
	static Color pixelByID(int count, double x, double y)
	{
		Color color = 0;
		y = y - 0.5; //ɨ���ߵ�������

		double maxz = -999, z;
		for (vector<Poly>::iterator it = APT.begin(); count > 0 && it != APT.end(); ++it)
		{
			//���жϽ�������Ķ����
			if ((*it).flag)
			{
				//����Ƚ��������ߵ�zֵ
				z = -((*it).A * x + (*it).B * y + (*it).D) / (*it).C;
				if (z > maxz)
				{
					maxz = z;
					color = (*it).color;
				}
				count--;
			}
		}
		return color;



	}



};