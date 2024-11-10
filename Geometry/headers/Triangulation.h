#pragma once
#include "Triangle.h"
#include<vector>
namespace Geometry
{
    class Triangulation
    {
    public:
        Triangulation();
        ~Triangulation();

        std::vector<double> uniqueNumbers;
        std::vector<Triangle> Triangles;
    };
}
