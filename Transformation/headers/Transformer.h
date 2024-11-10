#pragma once
#include "Triangulation.h"
#include <map>
#define TOLERANCE 0.0000001
using namespace std;
using namespace Geometry;

namespace Transformation
{
	class Transformer
	{
	public:
		Transformer();
		~Transformer();

		bool operator()(double a, double b) const;

		Point ptTriangulator(double _x, double _y, double _z, map<double, int, Transformer>& uniqueMap, vector<double>& uniqueValues);
		Triangulation scaleUniform(const Triangulation&, double);
		Triangulation scaleNonUniform(const Triangulation&, double, double, double);
		Triangulation rotateZ(const Triangulation&, double theta);
		Triangulation rotateX(const Triangulation&, double theta);
		Triangulation rotateY(const Triangulation&, double theta);
	};
}

