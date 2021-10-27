#pragma once

#include <vector>
using namespace std;

struct StyledGeometry;
struct Geometry;
struct Style;
class GeometryContainer
{
public:
	StyledGeometry* addGeometry(Geometry* pGeometry, Style* pStyle);
	~GeometryContainer();
	void clear();
	vector<StyledGeometry*> mGeometryContainer;
	static void ToGetObject(GeometryContainer& g_GeometryContainer);
};

