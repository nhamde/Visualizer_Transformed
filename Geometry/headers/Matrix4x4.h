#pragma once
#include <vector>
using namespace std;

namespace Geometry
{
	class Matrix4x4
	{
	public:
		vector<vector<double>> mat;

		Matrix4x4();
		~Matrix4x4();

		vector<double> multiply(double x, double y, double z);
	};
}