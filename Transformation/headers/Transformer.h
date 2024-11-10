#pragma once
#include "Triangulation.h"
#include <map>
#define TOLERANCE 0.0000001
using namespace std;
using namespace Geometry;

namespace Transformation
{
	struct customComparator
	{
		bool operator()(double a, double b) const
		{
			return fabs(a - b) > TOLERANCE ? a < b : false;
		}
	};

	class Transformer
	{
	public:
		Transformer();
		~Transformer();

		Point ptTriangulator(double _x, double _y, double _z, map<double, int, customComparator>& uniqueMap, vector<double>& uniqueValues);
		Triangulation scaleUniform(const Triangulation&, double);
		Triangulation scaleNonUniform(const Triangulation&, double, double, double);
		Triangulation rotateZ(const Triangulation&, double theta);
		Triangulation rotateX(const Triangulation&, double theta);
		Triangulation rotateY(const Triangulation&, double theta);
	};
}

