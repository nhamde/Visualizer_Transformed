#include "Matrix4x4.h"

using namespace Geometry;

Matrix4x4::Matrix4x4()
{
	mat = { {1.0, 0.0, 0.0, 0.0},
			{0.0, 1.0, 0.0, 0.0},
			{0.0, 0.0, 1.0, 0.0},
			{0.0, 0.0, 0.0, 1.0} };
}

Matrix4x4::~Matrix4x4()
{
}

vector<double> Matrix4x4::multiply(double x, double y, double z)
{
	vector<double> inVector = { x, y, z, 1 };
	vector <double> outVector(4, 0.0);

	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat[i].size(); j++)
		{
			outVector[i] += inVector[j] * mat[i][j];
		}
	}
	return outVector;
}