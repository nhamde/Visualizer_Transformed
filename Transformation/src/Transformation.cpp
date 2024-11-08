#pragma once
#include "Matrix4x4.h"
#include "Transformation.h"
#include <vector>
#include "Triangulation.h"
#include "Triangle.h"
#include <map>
using namespace Geometry;
using namespace transformya;

Transformation::Transformation()
{}
Transformation::~Transformation()
{}

Triangulation Transformation::scale(const Triangulation& tri, double scaleFactor)
{
	Matrix4x4 matrix;
	matrix.mat = {
		{scaleFactor,0.0,0.0,0.0},
		{0.0,scaleFactor,0.0,0.0},
		{0.0,0.0,scaleFactor,0.0},
		{0.0,0.0,0.0,1.0}
	};

	Triangulation output;

	for (auto triangle : tri.Triangles)
	{
		vector<Point> outputPts;
		outputPts.reserve(3);
		map<double, int> uniqueMap;
		Point ptArray[3] = { triangle.P1(), triangle.P2(), triangle.P3() };
		for (int p = 0; p < 3; p++)
		{
			int pt[3] = { 0,0,0 };

			vector<double> scaledPt = matrix.multiply(tri.UniqueNumbers[ptArray[p].X()], tri.UniqueNumbers[ptArray[p].Y()], tri.UniqueNumbers[ptArray[p].Z()]);

			for (int i = 0; i < 3; i++)
			{
				auto pair = uniqueMap.find(scaledPt[i]);
				if (pair == uniqueMap.end())
				{
					output.UniqueNumbers.push_back(scaledPt[i]);
					uniqueMap[scaledPt[i]] = output.UniqueNumbers.size() - 1;
					pt[i] = output.UniqueNumbers.size() - 1;
				}
				else
				{
					pt[i] = pair->second;
				}
			}
			outputPts.push_back(Point(pt[0], pt[1], pt[2]));
		}
		
		output.Triangles.push_back(Triangle(triangle.Normal(), outputPts[0], outputPts[1], outputPts[2]));
	}
	return output;
}
