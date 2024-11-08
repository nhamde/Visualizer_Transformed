#include <QList>
#include <fstream>
#include <iostream>
#include "OBJReader.h"
#include "Triangulation.h"
#include "Point.h"
#include "Triangle.h"
#include <vector>
#include <sstream>
#include <cassert>
#include <string>
#include <QDebug>
#include <map>
#define TOLERANCE 0.0000001
using namespace Geometry;

OBJReader::OBJReader()
{
}
OBJReader::~OBJReader()
{
}
bool OBJReader::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}
Point OBJReader::vectorReader(const QStringList& lineList, std::map<double, int, OBJReader>& uniqueMap, Triangulation& triangulation)
{
    double xyz[3];
    xyz[0] = lineList.value(1).toDouble();
    xyz[1] = lineList.value(2).toDouble();
    xyz[2] = lineList.value(3).toDouble();

    int pt[3];

    for (int i = 0; i < 3; i++)
    {
        auto pair = uniqueMap.find(xyz[i]);
        if (pair == uniqueMap.end())
        {
            triangulation.UniqueNumbers.push_back(xyz[i]);
            uniqueMap[xyz[i]] = triangulation.UniqueNumbers.size() - 1;
            pt[i] = triangulation.UniqueNumbers.size() - 1;
        }
        else
        {
            pt[i] = pair->second;
        }
    }
    return Point(pt[0], pt[1], pt[2]);
}
void OBJReader::read(const std::string& fileName, Triangulation& triangulation)
{
    triangulation.Triangles.clear();
    triangulation.UniqueNumbers.clear();

    std::map<double, int, OBJReader> uniqueMap;
    std::string fLetter;
    std::string str1;
    std::string str2;
    std::string str3;
    std::vector<Point> normals;
    std::vector<Point> vertices;

    std::ifstream infile(fileName);
    assert(infile && "Error: Could not open file");
    if (infile.is_open())
    {
        std::string line;

        while (getline(infile, line))
        {
            std::stringstream ss(line);
            QString _line = QString::fromStdString(line);
            QStringList linelist = _line.split(" ");
            if (linelist.value(0) == "v")
            {
                vertices.push_back(vectorReader(linelist, uniqueMap, triangulation));
            }
            if (linelist.value(0) == "vn")
            {
                normals.push_back(vectorReader(linelist, uniqueMap, triangulation));
            }
            if (linelist.value(0) == "f")
            {
                ss >> fLetter >> str1 >> str2 >> str3;
                QString str_s1 = QString::fromStdString(str1);
                QString str_s2 = QString::fromStdString(str2);
                QString str_s3 = QString::fromStdString(str3);
                QStringList splitList = str_s1.split("/");
                splitList << str_s2.split("/") << str_s3.split("/");
                int normalId = splitList.value(2).toInt() - 1;
                int firstVertexId = splitList.value(0).toInt() - 1;
                int secondVertexId = splitList.value(3).toInt() - 1;
                int thirdVertexId = splitList.value(6).toInt() - 1;

                triangulation.Triangles.push_back(Triangle(normals[normalId], vertices[firstVertexId], vertices[secondVertexId], vertices[thirdVertexId]));
            }
        }
    }
}