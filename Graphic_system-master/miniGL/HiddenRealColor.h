/*****************************************************
*  《三维图形的消隐算法实现》
*   学号：10180345
*   姓名：魏玉国
*   内容：
*	1.图像空间消隐算法：深度缓存器算法、区间扫描线算法、深度排序算法、区域 细分算法
*	2.景物空间消隐算法：BSP算法、多边形区域排序算法
*	3.介于两者之间算法：深度排序算法（画家算法）、区域细分算法、光线投影算法
*	
******************************************************/
#include "Vec.h"
#include <vector>
#include <math.h>
#include<algorithm>
#include <queue>
using namespace std;
Vec3d NewGraph3D[8];//三维图形 正方体

#define PMAXW 600  //范围宽度
#define PMAXH 600  //范围高度
const int INF = 9999;        //深度值

Vec3d Ocenter(50, 50, 50);  //三维空间中的坐标 
Vec3d eye(50, 50, 400);

//定义缓存器结构
typedef struct
{
	Color color = 99;      //颜色的深度
	double deepth = -INF; //z值的深度

}gPixelBuc;

//定义三角面拓扑结构
//tpsFace == three points faces
struct tpsFace
{
	Vec4d point3D[3];  
	int size = 3;

}CubeFace[12];  //正方体有12个三角形组成的面

gPixelBuc g_PixelBuc[PMAXW][PMAXH];



struct InformPt
{
	Vec3d point;
	Color Light;
};

//区间扫描线算法边表
struct Edge
{
	double x, dx, dy;  //dx和dy在增量算中求解深度的值
	int polygonID;  //指向多边形的指针


	//下面是groud
	InformPt points[2];
};
//区间扫描线算法多边形表
struct Poly
{
	int ploygonID; //多边形的ID
	double A, B, C, D; //空间平面方程系数 Ax + By + Cz +D = 0
	int dy; //扫描线跨越的范围
	Color color;  //颜色
	bool flag; //判断多边形在内在外
};


vector<vector<Edge>> Etable;  //区间扫描线算法边表
vector<vector<Poly>> Ptable;   //区间扫描线算法多边形表
vector<Edge> AET;  //活性边
vector<Poly> APT;  //活性多边形
//定义交点的数据结构
struct cPoint
{
	double zleft, zright;
	int order;
};



//画家算法
struct DpFace :tpsFace
{
	Color color;
	double zmin,zmax;
	//定义三角面的方程系数
	double a, b, c, d;
};


//不同的消隐算法
class Hiddens
{
public:

	/*************************************************——Z_Buffer算法实现——***********************/	//将三维立方体的点集转换为三角面的集合
	static void ptsConvertToFace(Vec3d(&points3D)[8], tpsFace(&faces)[12])
	{
		//底面
		faces[0].point3D[0] = points3D[0]; faces[0].point3D[1] = points3D[1]; faces[0].point3D[2] = points3D[2];
		faces[1].point3D[0] = points3D[0]; faces[1].point3D[1] = points3D[2]; faces[1].point3D[2] = points3D[3];
		//左侧面
		faces[2].point3D[0] = points3D[0]; faces[2].point3D[1] = points3D[1]; faces[2].point3D[2] = points3D[5];
		faces[3].point3D[0] = points3D[0]; faces[3].point3D[1] = points3D[4]; faces[3].point3D[2] = points3D[5];
		//正前面
		faces[4].point3D[0] = points3D[1]; faces[4].point3D[1] = points3D[2]; faces[4].point3D[2] = points3D[6];
		faces[5].point3D[0] = points3D[1]; faces[5].point3D[1] = points3D[5]; faces[5].point3D[2] = points3D[6];
		//右侧面
		faces[6].point3D[0] = points3D[2]; faces[6].point3D[1] = points3D[3]; faces[6].point3D[2] = points3D[7];
		faces[7].point3D[0] = points3D[2]; faces[7].point3D[1] = points3D[6]; faces[7].point3D[2] = points3D[7];
		//正后面
		faces[8].point3D[0] = points3D[0]; faces[8].point3D[1] = points3D[3]; faces[8].point3D[2] = points3D[7];
		faces[9].point3D[0] = points3D[0]; faces[9].point3D[1] = points3D[4]; faces[9].point3D[2] = points3D[7];
		//顶面
		faces[10].point3D[0] = points3D[4]; faces[10].point3D[1] = points3D[5]; faces[10].point3D[2] = points3D[6];
		faces[11].point3D[0] = points3D[4]; faces[11].point3D[1] = points3D[6]; faces[11].point3D[2] = points3D[7];

		
	}

	//获取采样点的的深度z
	static double ToGetDeepthZ(Vec4d pt0, Vec4d pt1, Vec4d pt2, int x, int y)
	{
		//计算原理-向量的叉乘原理
		//| i      j    k   |
		//|x2-x1 y2-y1 z2-z1| //mx my mz
		//|x3-x1 y3-y1 z3-z1| //nx ny nz
		//平面方程
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

		//如果不是
		return (A * x + B * y + D) / (-C);

	}

	//Z_Buffer算法实现
	static void Z_Buffer(tpsFace &pface, Color gcolor)
	{
		Vec4d face[3];
		for (int i = 0; i < 3; ++i)
		{
			face[i] = pface.point3D[i];
		}

		int miny = INF, maxy = -INF;
		int size = pface.size; //一个面的点数
		//进行需要像素（屏幕）排序的范围 以y为准
		for (int i = 0; i < size; ++i)
		{
			if (face[i].y > maxy)
				maxy = face[i].y;
			if (face[i].y < miny)
				miny = face[i].y;
		}

		//对面中的点进行遍历,求解扫描线上的像素信息
		for (int y = miny; y < maxy; ++y)
		{
			//定义一个数组用来存储扫描线交点信息
			vector<int> gvec;
			for (int i = 0; i < size; ++i)
			{
				if (face[i].y == y)
				{

					gvec.push_back(face[i].x);  //等于扫描线的时候
					if ((face[(i + 1) % size].y - face[i].y) * (face[(i - 1 + size) % size].y - face[i].y) > 0)
					{
						gvec.push_back(face[i].x); //基数点相交 “特殊情况”？

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
			//上述“交点“进行排序
			sort(gvec.begin(), gvec.end());
			//进行深度比较-这条线上的
			for (int i = 0; i < gvec.size(); i = i + 2)
			{
				for (int j = gvec[i]; j < gvec[i + 1]; ++j)
				{
					double deepz = ToGetDeepthZ(face[0], face[1], face[2], j, y);
					if (j < PMAXW && y < PMAXH)    //防止数组越界
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

	//屏幕像素初始化
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

	//绘制
	static void drawPixels()
	{
		int indexx = 0;   //测试数据，但是结果有误？63？？？
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
	/*************************************************——Z_Buffer算法实现——***********************/
	/*************************************************——区间扫描线算法实现——********************************/

	//static vector<vector<Edge>> Etable;  //区间扫描线算法边表
	//static vector<vector<Poly>> Ptable;   //区间扫描线算法多边形表
	//static vector<vector<Edge>> AET;  //活性边
	//static vector<vector<Edge>> EPT;  //活性多边形
	//放前面作为全局变量吧？/??
	
	//1.建立表
	static void  buildtable(tpsFace (&faces)[12])
	{
		//创建边的表首先将之前用vector创建的数组尽心清空，防止内存占用
		Etable.clear();
		Ptable.clear();
		Etable.resize(getWindowHeight()/2);
		Ptable.resize(getWindowHeight()/2);
		int facessize = 12;  //定义三角形面的个数

		for (int i = 0; i < facessize; ++i)
		{
			//创建边的表
			Edge edge;
			float miny = INF;
			float maxy = 0;
			//循环每一个面的每一个点
			for (int j = 0; j < faces[i].size; ++j)
			{
				edge.polygonID = i;
				//edge.falg = 0;需要?
				//找到每一条线的顶点坐标
				//活性表表法类似，找到y值较大，并且在上面的点作为起点 
				Vec4d c1 = faces[i].point3D[j];
				Vec4d c2 = faces[i].point3D[(j + 1) % faces[i].size];

				if (c1.y < c2.y)
				{
					edge.x = c2.x;
					//dx 可以是斜率导数的相反数，y增加1 也就是y+1时候的增量
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


			//开始创建多边形表
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
	//获取面方程的A、B、C参数
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

	//更新活边
	static void updateActiveEdge()
	{
		//判断活性多边形，如果扫描完了的话就删除
		int n2 = 0;
		for (vector<Edge>::iterator it = AET.begin(); it != AET.end(); ++it)
		{
			(*it).dy--;
			if ((*it).dy == 0) continue;
			else
			{
				(*it).x += (*it).dx;
				*(AET.begin() + n2) = *(it);
				n2++;  //n2需要+1

			}
		}
		AET.resize(n2);	
	}

	//更新活多边形
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

	//计算区间的颜色
	static void calculateColor(int y, vector<Color> &buffer)
	{
		double left = 0, right;
		//计算多边形的个数
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
			//将背景的颜色值设置为0
			Color pcolor = INF;
			//如果count的值计算结果为0，也就说在（left,right)在这个区间的多边形的个数为0
			if (count == 0)
			{
				//在这个区间的像素值都设置为背景值为0
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = INF;
				}
			}
			//如果这个区间有1个多边形
			else if (count == 1)
			{
				pcolor = pixelByID(count, left, y);
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = pcolor;
				}
			}
			else //区间内有多个多边形的时候
			{
				//计算交点
				//先两两计算，成对
				
				vector<cPoint> points;//申明一个vector用来存储points				
				Poly p1, p2;//列举两个多边形
				vector<double> cross;//交点的x值



				//活性多边形情况进行循环
				//计算区间左右的深度值，把他们所属的多边形在APT表中位置记录下来
				for (int i = 0; i < APT.size(); ++i)
				{
					if (points.size() == count)break;   //出现的多边形个数
					if (APT[i].flag && APT[i].C != 0)
					{
						//每一个多边形算一样的次数
						double zleft = -(APT[i].A * left + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						double zright = -(APT[i].A * right + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						points.push_back({ zleft,zright,i});
					}
				}

				//计算是否交叉，交叉的话求解出交点，并且将交点存入points中
				for (int i = 0; i < points.size(); ++i)
				{
					//左右端点的差值符号相反的时候，代表出现贯穿的现象
					for (int j = i + 1; j < points.size(); ++j)
					{
						if ((points[i].zleft - points[j].zleft) * (points[i].zright - points[j].zright) < 0)
						{

							p1 = APT[points[i].order];
							p2 = APT[points[j].order];
							//计算交点
							//A1x + B1x + C1x + D1 =0
							//A2x + B2x + C2x + D2 =0
							if((p1.C * p2.A - p2.C * p1.A) != 0)
								cross.push_back(((p2.C * p1.B - p1.C * p2.B) * y + p2.C * p1.D - p1.C * p2.D) / (p1.C * p2.A - p2.C * p1.A));
						}						
					}
				}

				//现在将这个区间进行右入栈
				cross.push_back(right);
				sort(cross.begin(), cross.end()); //对其进行排序
				for (int i = 0; i < cross.size(); ++i)
				{
					right = cross[i];
					pcolor = pixelByID(count, left, y);
					for (double pixel = left; left < right; ++left)
					{
						buffer[left] = pcolor;   //这个地方在等于65的时候会发生数组越界
					}
				}
			}
			//区间右边的边所属的多边形标志
			count = updateFlagByID(AET[n].polygonID) ? count + 1 : count - 1;
			left = right;//切换到下一个区间		
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
	//扫描
	static void scanLine(vector<vector<Color>> &buffer)
	{
		AET.clear();
		APT.clear();
		buffer.clear();
		//建立活性边表，从上到下扫描
		//边的指向是指向y减小的方向，所以x+dx就是y-1的值
		buffer.resize(getWindowHeight() / 2);

		//向下扫描循环
		for (int y = getWindowHeight() / 2-1; y >= 0; --y)
		{
			//添加新的活性边
			for (int n = 0; n < Etable[y].size(); ++n)
			{
				AET.push_back(Etable[y][n]);
			}
			//添加新的活的多边形
			for (int n = 0; n < Ptable[y].size(); ++n)
			{
				APT.push_back(Ptable[y][n]);
			}
			//对活性边进行排序
			if (!AET.empty())   //vector非空
				SortActiveEdge();
			calculateColor(y, buffer[y]);

			//对活性表进行更新，产生下一条扫描线
			updateActivePolygon();
			updateActiveEdge();
		}
	}

	//sort函数
	//void sort(** first, ** last, **comp)
	//first： 要排序的数组的起始地址
	//last ： 最后一个数据的地址
	//comp ：是排序的方法的实现
	static void SortActiveEdge()
	{
		sort(AET.begin(), AET.end(),comp);
	}

	static bool comp(const Edge& a, const Edge& b)
	{
		return a.x < b.x;
	}

	
	//计算交叉多边形的z值以及color
	static Color pixelByID(int count,double x,double y)
	{
		Color color = 0;
		y = y - 0.5; //扫描线的中心线

		double maxz = -999, z;
		for (vector<Poly>::iterator it = APT.begin(); count > 0 && it != APT.end(); ++it)
		{
			//线判断进入区间的多边形
			if ((*it).flag)
			{
				//计算比较区间的左边的z值
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
	//进行绘图着色
	static void draw(vector<vector<Color>>& buffer)
	{
		//int indexx = 0;   //indexx最终结果为0
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

	/****************************************——区间扫描线算法——********************************/


	/********************************************** —— 画家算法 ——   *************************/

	

	static bool zType(DpFace& a, DpFace& b)
	{
		return a.zmin < b.zmin;  //按照z的大小进行排序
	}

	static void JudgeOverlap(DpFace& b, DpFace& a)
	{

	}


	/********************************************** —— 画家算法 ——   *************************/
};



/****************************************************************************************************/
//定义恒定光强的多边形绘制下的面的结构体
struct Lightface
{
	int polyID;   //面的ID编号
	double A, B, C, D;  //面方程的系数
	Vec3d spmv;   //面的法向量
	Color color;   //面的颜色也可以的编号
	Vec4d points[3]; //三角面的三个顶点坐标存储

};
Lightface LightFace[12];

//需要重新定义一个vector用来存储新的光照模型下的每个像素的颜色值
vector<vector<Color>> newBuffer;

/*********g光照参数模型*************************/
//选择为RGB模型
//1.定义环境光的光照强度
static double iaR, iaG, iaB;
//2.定义入射光的强度
static double ipR, ipG, ipB;
//3.定义环境光反射系数
static double Kar, Kag, Kab;
//4.定义漫反射系数
static double Kdr, Kdg, Kdb;
//5.定义镜面反射系数
static double Ksr, Ksg, Ksb;

/************************************************/
struct rgb
{
	double r, g, b;
};
//定义每个面的以及角点的信息
struct PtVector
{
	int polyID;   //该顶点所属的多边形
	int indexID;  //点在这个面中的顺序的索引
	Vec3d point;
	Vec3d vec;
	Color light;   //这个点最后的光照强度
	rgb RGB;
};
struct GrdFaces
{
	int polyID;   //面的ID
	double A, B, C, D;  //一个面的方程的系数
	Vec3d spmv;  //每个面的
	Color color;  //每个面的颜色
	PtVector points[3];   //一个面的三个顶点
	int size = 3;  //表示一个面中的点的数量
};
GrdFaces groundface[12];

//真实感着色绘制图形的类
class RealisticColor
{
public:
	RealisticColor();
	
	//恒定光强的多边形绘制
	static void ConstLightIntensity(tpsFace(&faces)[12])
	{
		//对相关的系数进行赋值操作
		iaR = iaG = iaB = 0.5;   //之前设置的是0.8
		ipR = ipG = ipB = 0.8;
		Kar = Kag = Kab = 0.1;
		Kdr = Kdg = Kdb = 0.2;
		Ksr = Ksg = Ksb = 0.5;
		Vec3d Light(-1000,-1000,-990);

		//首先计算每一个面的相关信息
		int size = 12;
		for (int i = 0; i < size; ++i)
		{
			LightFace->polyID = LightFace->color = i;
			Hiddens::GetABC(faces[i], LightFace[i].A, LightFace[i].B, LightFace[i].C, LightFace[i].D);
			//将三角面三个顶点的信息存储到需要研究的光照模型中去
			int pointsize = 3;
			for (int j = 0; j < pointsize; ++j)
			{
				LightFace[i].points[j] = faces[i].point3D[j];
			}

			//存储面的法向量信息
			LightFace[i].spmv.x = LightFace[i].A;
			LightFace[i].spmv.y = LightFace[i].B;
			LightFace[i].spmv.z = LightFace[i].C;

			//重新定义一个向量为图形中心指向一个面的角点的向量
			Vec3d tempVec = Ocenter - LightFace[i].points[0];

			//判断是否指向正方体外面，如果不是，那么返回该法向量的负方向的向量
			if (!SpMVdrector(LightFace[i].spmv, tempVec))
			{
				LightFace[i].spmv.x *= -1;
				LightFace[i].spmv.y *= -1;
				LightFace[i].spmv.z *= -1;
			}
		}


		//运用区间扫描线算法对区域进行运算，得到消隐后的图形
		vector<vector<Color>> ColorPixels;  //定义“全局的像素映射关系
		Hiddens::buildtable(CubeFace); //构建表面表
		Hiddens::scanLine(ColorPixels); //进行扫描线消隐处理

		

		//然后对区域内的像素进行普通光模型建模处理
		int sizey = ColorPixels.size();
		newBuffer.resize(ColorPixels.size());
		for (int y = 0; y < sizey; ++y)
		{
			int sizex = ColorPixels[y].size(); //扫描线上的x范围值

			for (int x = 0; x < sizex; ++x)
			{
				//采用第二种方式，对区域当中的每一个像素进行扫描
				//RGB分开算
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
		//判断2个向量方向是否同向
		//同向 返回为true 否则 返回为false
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

		//如果不是
		return (A * x + B * y + D) / (-C);
	}

	//计算距离
	static double GetDistance(Vec3d& Light, Vec3d& Targrt, Vec3d& eye)
	{
		double d1 = sqrt(pow(Targrt.x - Light.x, 2) + pow(Targrt.y - Light.y, 2) + pow(Targrt.z - Light.z, 2));
		double d2 = sqrt(pow(Targrt.x - eye.x, 2) + pow(Targrt.y - eye.y, 2) + pow(Targrt.z - eye.z, 2));
		return d1 + d2;
	}

	//计算每一个小像素的光照值
	static double ToGetLightFaces(double x, double y, Color& pixel, State::ColorMode ColorMode, Vec3d Light)
	{
		double Ic, Id, Is;
		int n = 5;    //n是与物体表面光滑度有关的一个常数，表面越光滑，n的值就越大
		double Ivalue;
		
		if (pixel == INF)
			return 1;
		else
		{
			//对法向量的方向进行判断

			Vec3d N = LightFace[pixel].spmv;  //面的法向矢量

		//求解点光源到像素坐标点的向量
		//首先求解面上x,y的三维空间向量
			double z = GetZdeep(LightFace[pixel].A, LightFace[pixel].B, LightFace[pixel].C, LightFace[pixel].D, x, y);
			
			//光强衰减
			double fd,c0,c1,c2; //用于光强的衰减计算
			c0 = 0.8;
			c1 = 0.0000001;
			c2 = 0.000000001;
			Vec3d Target(x, y, z);
			//计算广随着距离的衰减
			double distance = GetDistance(Light, Target, eye);
			double disDecay = 1 / (c0 + c1 * distance + c2 * pow(distance, 2));
			fd = min(1, distance);
			//L向量
			Vec3d L(x - Light.x, y - Light.y, z - Light.z);

			//将两个向量进行单位化
			N.Normalize();
			L.Normalize();

			//定义镜面弹舌视线方向的向量性
			Vec3d V(eye.x - x, eye.y - y, eye.z - z);
			V.Normalize();

			Vec3d H; //进行相似拟合向量

			double angle;
			//对这个像素进行判定
			switch (ColorMode)
			{
			case State::cRGB_R:
				//1.求解环境反射光
				Ic = iaR * Kar;

				//2.求解漫反射光
				angle = acos(DotProduct(L,N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipR * Kdr * (DotProduct(L, N));

				//3.求解镜面反射光
				//PS,这里的反射光对称性太复杂了，心累
				//这里用后面的平均量进行替代 &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipR * Ksr * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_G:
				//1.求解环境反射光
				Ic = iaG * Kag;

				//2.求解漫反射光
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipG * Kdg * (DotProduct(L, N));

				//3.求解镜面反射光
				//PS,这里的反射光对称性太复杂了，心累
				//这里用后面的平均量进行替代 &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipG * Ksg * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_B:

				//1.求解环境反射光
				Ic = iaB * Kab;

				//2.求解漫反射光
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipB * Kdb * (DotProduct(L, N));

				//3.求解镜面反射光
				//PS,这里的反射光对称性太复杂了，心累
				//这里用后面的平均量进行替代 &			
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


	//Ground明暗处理
	//定义



	static void GroundRealColor(tpsFace(&faces)[12])
	{

		//对每一个面进行遍历先进行求解面的
		//对相关的系数进行赋值操作
		iaR = iaG = iaB = 3;
		ipR = ipG = ipB = 0.8;
		Kar = Kag = Kab = 0.1;
		Kdr = Kdg = Kdb = 0.2;
		Ksr = Ksg = Ksb = 0.5;
		Vec3d Light(-1000, -1000, -990);

		//首先计算每一个面的相关信息
		int fssize = 12;
		for (int i = 0; i < fssize; ++i)
		{
			groundface->polyID = groundface->color = i;
			Hiddens::GetABC(faces[i], groundface[i].A, groundface[i].B, groundface[i].C, groundface[i].D);
			//将三角面三个顶点的信息存储到需要研究的光照模型中去
			int pointsize = 3;
			for (int j = 0; j < pointsize; ++j)
			{
				groundface[i].points[j].polyID = i;  //每一个顶点所属的多边形
				groundface[i].points[j].indexID = j;   //获取这个点在面中的索引下标
				groundface[i].points[j].point = faces[i].point3D[j];
			}

			//存储面的法向量信息
			groundface[i].spmv.x = groundface[i].A;
			groundface[i].spmv.y = groundface[i].B;
			groundface[i].spmv.z = groundface[i].C;

			//重新定义一个向量为图形中心指向一个面的角点的向量
			Vec3d tempVec = Ocenter - groundface[i].points[0].point;

			//判断是否指向正方体外面，如果不是，那么返回该法向量的负方向的向量
			if (!SpMVdrector(groundface[i].spmv, tempVec))
			{
				groundface[i].spmv.x *= -1;
				groundface[i].spmv.y *= -1;
				groundface[i].spmv.z *= -1;
			}
			for (int j = 0; j < 3; ++j)
			{
				groundface[i].points[j].vec = groundface[i].spmv; //得到每个定点在每一个面的方向性矢量
				groundface[i].points[j].vec.Normalize(); //将顶点处的方向性矢量进行单位化
			}
			
		}
		


		//计算每一个多边形顶点的平均法线矢量
		//将每个顶点用数组的方式进行存储起来
		vector<vector<PtVector>> SamePoints;   //b保存每一个点
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

		//对数组中的每一个点进行求解平均向量
		for (int i = 0; i < SamePoints.size(); ++i)
		{
			Vec3d m(0,0,0); //设置一个空的向量
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


		//不用遍历了，直接用扫描线算法进行求解
		//进行插值运算,运用的是类扫描线方法构造
		//对图像进行消隐变换
		//运用区间扫描线算法对区域进行运算，得到消隐后的图形
		vector<vector<Color>> ColorPixels;  //定义“全局的像素映射关系
		Hiddens::buildtable(CubeFace); //构建表面表
		Hiddens::scanLine(ColorPixels); //进行扫描线消隐处理
		//将已经变换好的重新换算到面当中去
		for (int i = 0; i < ptsize; ++i)
		{
			for (int j = 0; j < psize; ++j) //每一个面中的三个点的信息
			{
				//计算顶点的单位平均方向性矢量
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

				//计算这个点的光照强度
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

		
		
		

		//遍历，查找可以显示的多边形
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


		//对每一个边进行扫描转换
		vector<vector< PtVector>> section;
		section.clear();

		int ymax = getWindowHeight() / 2;
		for (int y = ymax; y > 0; --y)
		{
			//对每一个面的边进行求解交点
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

					//计算中间点的亮度
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
		//构建边
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

		//排序
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

	//Ground算法计算每一个面的三角面的顶点处的光照强度
	static double ToGetGroundFaces(double x, double y, Color pixel, State::ColorMode ColorMode, Vec3d Light,Vec3d& tarv)
	{
		double Ic, Id, Is;
		int n = 5;    //n是与物体表面光滑度有关的一个常数，表面越光滑，n的值就越大
		double Ivalue;

		if (pixel == INF)
			return 1;
		else
		{
			//对法向量的方向进行判断

			Vec3d N = tarv;  //面的法向矢量

		//求解点光源到像素坐标点的向量
		//首先求解面上x,y的三维空间向量
			double z = GetZdeep(groundface[pixel].A, groundface[pixel].B, groundface[pixel].C, groundface[pixel].D, x, y);

			//光强衰减
			double fd, c0, c1, c2; //用于光强的衰减计算
			c0 = 0.8;
			c1 = 0.0000001;
			c2 = 0.000000001;
			Vec3d Target(x, y, z);
			//计算广随着距离的衰减
			double distance = GetDistance(Light, Target, eye);
			double disDecay = 1 / (c0 + c1 * distance + c2 * pow(distance, 2));
			fd = min(1, distance);
			//L向量
			Vec3d L(x - Light.x, y - Light.y, z - Light.z);

			//将两个向量进行单位化
			N.Normalize();
			L.Normalize();

			//定义镜面弹舌视线方向的向量性
			Vec3d V(eye.x - x, eye.y - y, eye.z - z);
			V.Normalize();

			Vec3d H; //进行相似拟合向量

			double angle;
			//对这个像素进行判定
			switch (ColorMode)
			{
			case State::cRGB_R:
				//1.求解环境反射光
				Ic = iaR * Kar;

				//2.求解漫反射光
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipR * Kdr * (DotProduct(L, N));

				//3.求解镜面反射光
				//PS,这里的反射光对称性太复杂了，心累
				//这里用后面的平均量进行替代 &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipR * Ksr * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_G:
				//1.求解环境反射光
				Ic = iaG * Kag;

				//2.求解漫反射光
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipG * Kdg * (DotProduct(L, N));

				//3.求解镜面反射光
				//PS,这里的反射光对称性太复杂了，心累
				//这里用后面的平均量进行替代 &			
				H = (L + V) / 2;
				H.Normalize();

				Is = fd * ipG * Ksg * sqrt((DotProduct(H, N), n));
				Ivalue = Ic + Id + Is;
				return Ivalue;
			case State::cRGB_B:

				//1.求解环境反射光
				Ic = iaB * Kab;

				//2.求解漫反射光
				angle = acos(DotProduct(L, N));
				Id = 0;
				if (angle < 0 || angle > PI / 2)
					Id = 0;
				else
					Id = fd * ipB * Kdb * (DotProduct(L, N));

				//3.求解镜面反射光
				//PS,这里的反射光对称性太复杂了，心累
				//这里用后面的平均量进行替代 &			
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
	
	//判断Vector里面有没有这个颜色
	static bool cheak(Color color, vector<Color>& vec)
	{
		for (int i = 0; i < vec.size(); ++i)
		{
			if (color == vec[i] || color == INF)
				return false;
		}
		return true;
	}
	
	//建立扫描表
	//1.建立表
	static void  buildtableGround(GrdFaces (&faces)[12])
	{
		//创建边的表首先将之前用vector创建的数组尽心清空，防止内存占用
		Etable.clear();
		Ptable.clear();
		Etable.resize(getWindowHeight() / 2);
		Ptable.resize(getWindowHeight() / 2);
		int facessize = 12;  //定义三角形面的个数

		for (int i = 0; i < facessize; ++i)
		{
			//创建边的表
			Edge edge;
			float miny = INF;
			float maxy = 0;
			//循环每一个面的每一个点
			for (int j = 0; j < faces[i].size; ++j)
			{
				edge.polygonID = i;
				//edge.falg = 0;需要?
				//找到每一条线的顶点坐标
				//活性表表法类似，找到y值较大，并且在上面的点作为起点 
				Vec4d c1 = faces[i].points[j].point;
				Vec4d c2 = faces[i].points[(j + 1) % faces[i].size].point;

				if (c1.y < c2.y)
				{
					edge.x = c2.x;
					//dx 可以是斜率导数的相反数，y增加1 也就是y+1时候的增量
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
			//开始创建多边形表
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


	//更新活边
	static void updateActiveEdge()
	{
		//判断活性多边形，如果扫描完了的话就删除
		int n2 = 0;
		for (vector<Edge>::iterator it = AET.begin(); it != AET.end(); ++it)
		{
			(*it).dy--;
			if ((*it).dy == 0) continue;
			else
			{
				(*it).x += (*it).dx;
				*(AET.begin() + n2) = *(it);
				n2++;  //n2需要+1

			}
		}
		AET.resize(n2);
	}

	//更新活多边形
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

	//计算区间的颜色
	static void calculateColor(int y, vector<Color>& buffer)
	{



		double left = 0, right;
		//计算多边形的个数
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
			//将背景的颜色值设置为0
			Color pcolor = INF;
			//如果count的值计算结果为0，也就说在（left,right)在这个区间的多边形的个数为0
			if (count == 0)
			{
				//在这个区间的像素值都设置为背景值为0
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = INF;
				}
			}
			//如果这个区间有1个多边形
			else if (count == 1)
			{
				pcolor = pixelByID(count, left, y);
				for (double pixel = left; left < right; ++left)
				{
					buffer[left] = pcolor;


				}
			}
			else //区间内有多个多边形的时候
			{
				//计算交点
				//先两两计算，成对

				vector<cPoint> points;//申明一个vector用来存储points				
				Poly p1, p2;//列举两个多边形
				vector<double> cross;//交点的x值



				//活性多边形情况进行循环
				//计算区间左右的深度值，把他们所属的多边形在APT表中位置记录下来
				for (int i = 0; i < APT.size(); ++i)
				{
					if (points.size() == count)break;   //出现的多边形个数
					if (APT[i].flag && APT[i].C != 0)
					{
						//每一个多边形算一样的次数
						double zleft = -(APT[i].A * left + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						double zright = -(APT[i].A * right + APT[i].B * (y + 0.5) + APT[i].D) / APT[i].C;
						points.push_back({ zleft,zright,i });
					}
				}

				//计算是否交叉，交叉的话求解出交点，并且将交点存入points中
				for (int i = 0; i < points.size(); ++i)
				{
					//左右端点的差值符号相反的时候，代表出现贯穿的现象
					for (int j = i + 1; j < points.size(); ++j)
					{
						if ((points[i].zleft - points[j].zleft) * (points[i].zright - points[j].zright) < 0)
						{

							p1 = APT[points[i].order];
							p2 = APT[points[j].order];
							//计算交点
							//A1x + B1x + C1x + D1 =0
							//A2x + B2x + C2x + D2 =0
							if ((p1.C * p2.A - p2.C * p1.A) != 0)
								cross.push_back(((p2.C * p1.B - p1.C * p2.B) * y + p2.C * p1.D - p1.C * p2.D) / (p1.C * p2.A - p2.C * p1.A));
						}
					}
				}

				//现在将这个区间进行右入栈
				cross.push_back(right);
				sort(cross.begin(), cross.end()); //对其进行排序
				for (int i = 0; i < cross.size(); ++i)
				{
					right = cross[i];
					pcolor = pixelByID(count, left, y);
					for (double pixel = left; left < right; ++left)
					{
						buffer[left] = pcolor;   //这个地方在等于65的时候会发生数组越界
					}
				}
			}
			//区间右边的边所属的多边形标志
			count = updateFlagByID(AET[n].polygonID) ? count + 1 : count - 1;
			left = right;//切换到下一个区间		
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
	//扫描
	static void scanLineGround(vector<vector<Color>>& buffer)
	{
		AET.clear();
		APT.clear();
		buffer.clear();
		//建立活性边表，从上到下扫描
		//边的指向是指向y减小的方向，所以x+dx就是y-1的值
		buffer.resize(getWindowHeight() / 2);

		//向下扫描循环
		for (int y = getWindowHeight() / 2 - 1; y >= 0; --y)
		{
			//添加新的活性边
			for (int n = 0; n < Etable[y].size(); ++n)
			{
				AET.push_back(Etable[y][n]);
			}
			//添加新的活的多边形
			for (int n = 0; n < Ptable[y].size(); ++n)
			{
				APT.push_back(Ptable[y][n]);
			}
			//对活性边进行排序
			if (!AET.empty())   //vector非空
				SortActiveEdge();
			calculateColor(y, buffer[y]);

			//对活性表进行更新，产生下一条扫描线
			updateActivePolygon();
			updateActiveEdge();
		}
	}

	//sort函数
	//void sort(** first, ** last, **comp)
	//first： 要排序的数组的起始地址
	//last ： 最后一个数据的地址
	//comp ：是排序的方法的实现
	static void SortActiveEdge()
	{
		sort(AET.begin(), AET.end(), comp);
	}

	static bool comp(const Edge& a, const Edge& b)
	{
		return a.x < b.x;
	}


	//计算交叉多边形的z值以及color
	static Color pixelByID(int count, double x, double y)
	{
		Color color = 0;
		y = y - 0.5; //扫描线的中心线

		double maxz = -999, z;
		for (vector<Poly>::iterator it = APT.begin(); count > 0 && it != APT.end(); ++it)
		{
			//线判断进入区间的多边形
			if ((*it).flag)
			{
				//计算比较区间的左边的z值
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