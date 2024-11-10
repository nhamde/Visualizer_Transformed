#pragma once
#include "Matrix4x4.h"
#include "Transformer.h"
#include <vector>
#include <cmath>
using namespace Geometry;
using namespace Transformation;
using namespace std;

#define PI 3.14

Transformer::Transformer()
{}
Transformer::~Transformer()
{}

Point Transformer::ptTriangulator(double _x, double _y, double _z, map<double, int, customComparator>& uniqueMap, vector<double>& newUV)
{
	double array[3] = { _x, _y, _z };
	int pt[3];
	for (int i = 0; i < 3; i++)
	{
		auto pair = uniqueMap.find(array[i]);
		if (pair == uniqueMap.end())
		{
			newUV.push_back(array[i]);
			uniqueMap[array[i]] = newUV.size() - 1;
			pt[i] = newUV.size() - 1;
		}
		else
		{
			pt[i] = pair->second;
		}
	}
	return Point(pt[0], pt[1], pt[2]);
}

Triangulation Transformer::scaleUniform(const Triangulation& tri, double scaleFactor)
{
	Matrix4x4 matrix;
	matrix.mat = {
		{scaleFactor,0.0,0.0,0.0},
		{0.0,scaleFactor,0.0,0.0},
		{0.0,0.0,scaleFactor,0.0},
		{0.0,0.0,0.0,1.0}
	};

	Triangulation output;
	map<double, int, customComparator> uniqueMap;

	for (auto triangle : tri.Triangles)
	{
		vector<Point> newTrPts;
		newTrPts.reserve(3);
		Point ptArray[4] = { triangle.Normal(), triangle.P1(), triangle.P2(), triangle.P3() };
		for (int p = 1; p < 4; p++)
		{
			vector<double> scaledPt = matrix.multiply(tri.uniqueNumbers[ptArray[p].X()], tri.uniqueNumbers[ptArray[p].Y()], tri.uniqueNumbers[ptArray[p].Z()]);
			Point point = ptTriangulator(scaledPt[0], scaledPt[1], scaledPt[2], uniqueMap, output.uniqueNumbers);
			newTrPts.push_back(point);
		}
		Point newNormal = ptTriangulator(triangle.Normal().X(), triangle.Normal().Y(), triangle.Normal().Z(), uniqueMap, output.uniqueNumbers);
		output.Triangles.push_back(Triangle(newNormal, newTrPts[0], newTrPts[1], newTrPts[2]));
	}
	return output;
}

Triangulation Transformer::scaleNonUniform(const Triangulation& tri, double xScale, double yScale, double zScale)
{
	Matrix4x4 matrix;
	matrix.mat = {
		{xScale,0.0,0.0,0.0},
		{0.0,yScale,0.0,0.0},
		{0.0,0.0,zScale,0.0},
		{0.0,0.0,0.0,1.0}
	};

	Triangulation output;
	map<double, int, customComparator> uniqueMap;

	for (auto triangle : tri.Triangles)
	{
		vector<Point> newTrPts;
		newTrPts.reserve(3);
		Point ptArray[4] = { triangle.Normal(), triangle.P1(), triangle.P2(), triangle.P3() };
		for (int p = 1; p < 4; p++)
		{
			vector<double> scaledPt = matrix.multiply(tri.uniqueNumbers[ptArray[p].X()], tri.uniqueNumbers[ptArray[p].Y()], tri.uniqueNumbers[ptArray[p].Z()]);
			Point point = ptTriangulator(scaledPt[0], scaledPt[1], scaledPt[2], uniqueMap, output.uniqueNumbers);
			newTrPts.push_back(point);
		}
		Point newNormal = ptTriangulator(triangle.Normal().X(), triangle.Normal().Y(), triangle.Normal().Z(), uniqueMap, output.uniqueNumbers);
		output.Triangles.push_back(Triangle(newNormal, newTrPts[0], newTrPts[1], newTrPts[2]));
	}
	return output;
}

Triangulation Transformer::rotateZ(const Triangulation& tri, double theta)
{
	double radians = theta * PI / 180.0;
	Matrix4x4 rotationMatrix;
	rotationMatrix.mat = {
		{cos(radians),-sin(radians),0.0,0.0},
		{sin(radians),cos(radians),0.0,0.0},
		{0.0,0.0,1.0,0.0},
		{0.0,0.0,0.0,1.0}
	};
	Triangulation output;
	map<double, int, customComparator> uniqueMap;

	for (auto triangle : tri.Triangles)
	{
		vector<Point> newTrPts;
		newTrPts.reserve(3);
		Point ptArray[4] = { triangle.Normal(), triangle.P1(), triangle.P2(), triangle.P3() };
		for (int p = 1; p < 4; p++)
		{
			vector<double> scaledPt = rotationMatrix.multiply(tri.uniqueNumbers[ptArray[p].X()], tri.uniqueNumbers[ptArray[p].Y()], tri.uniqueNumbers[ptArray[p].Z()]);
			Point point = ptTriangulator(scaledPt[0], scaledPt[1], scaledPt[2], uniqueMap, output.uniqueNumbers);
			newTrPts.push_back(point);
		}
		auto normalRotated = rotationMatrix.multiply(triangle.Normal().X(), triangle.Normal().Y(), triangle.Normal().Z());
		Point newNormal = Point(normalRotated[0], normalRotated[1], normalRotated[2]);
		output.Triangles.push_back(Triangle(newNormal, newTrPts[0], newTrPts[1], newTrPts[2]));
	}
	return output;
}