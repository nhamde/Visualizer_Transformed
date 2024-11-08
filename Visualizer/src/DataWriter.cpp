#include "DataWriter.h"
#include<fstream>
#include<iomanip>
using namespace Geometry;

void DataWriter::Write(const std::string& filename, const Triangulation& triangulation)
{
    std::ofstream outfile(filename);

    if (outfile.is_open())
    {
        for each (Triangle tri in triangulation.Triangles)
        {
            outfile << std::fixed << std::setprecision(6)
                << fomulateText(triangulation, tri.P1())
                << fomulateText(triangulation, tri.P2())
                << fomulateText(triangulation, tri.P3()) << "\n" << "\n";
        }
    }
}

// using const references of parameters to avoid copying
std::string DataWriter::fomulateText(const Triangulation& triangulation, const Point& point)
{
    double x = triangulation.UniqueNumbers[point.X()];
    double y = triangulation.UniqueNumbers[point.Y()];
    double z = triangulation.UniqueNumbers[point.Z()];

    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "\n";    
}
