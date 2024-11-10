#include "STLReader.h"
#include "Point.h"
#include "Reader.h"
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<unordered_map>
using namespace Geometry;

#define TOLERANCE 0.0000001

STLReader::STLReader()
{
}

STLReader::~STLReader()
{
}

bool STLReader::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}

void STLReader::read(const std::string& fileName, Triangulation& triangulation)
{
    std::map<double, int, STLReader> uniqueValueMap;
    //std::map<Point, int> uniqueVertexMap;
    //std::map<Point, int> uniqueNormalMap;
    double xyz[3]; // To store 3 coordinates as double
    std::vector<int> pointIndices; // To store indices of unique double values
    triangulation.Triangles.clear();
    triangulation.uniqueNumbers.clear();

    std::ifstream infile(fileName);
    if (infile.is_open())
    {
        std::string line;
        while (getline(infile, line))
        {
            std::stringstream ss(line);
            std::string word;

            while (ss >> word)
            {
                if (word == "vertex" || word == "normal")
                {
                    ss >> xyz[0] >> xyz[1] >> xyz[2];

                    for (int i = 0; i < 3; i++)
                    {
                        auto pair = uniqueValueMap.find(xyz[i]);
                        if (pair == uniqueValueMap.end())
                        {
                            triangulation.uniqueNumbers.push_back(xyz[i]);
                            uniqueValueMap[xyz[i]] = triangulation.uniqueNumbers.size() - 1;
                            pointIndices.push_back(triangulation.uniqueNumbers.size() - 1);
                        }
                        else
                        {
                            pointIndices.push_back(pair->second);
                        }
                    }
                }

                if (pointIndices.size() == 12)
                {
                    Point normal = Point(pointIndices[0], pointIndices[1], pointIndices[2]);
                    Point p1 = Point(pointIndices[3], pointIndices[4], pointIndices[5]);
                    Point p2 = Point(pointIndices[6], pointIndices[7], pointIndices[8]);
                    Point p3 = Point(pointIndices[9], pointIndices[10], pointIndices[11]);
                    Triangle t(normal, p1, p2, p3);
                    triangulation.Triangles.push_back(t);
                    pointIndices.clear();
                }
            }
        }
    }
}