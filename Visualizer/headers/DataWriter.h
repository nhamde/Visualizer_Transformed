#pragma once
#include "Writer.h"
#include "Triangulation.h"
using namespace Geometry;

class DataWriter : public Writer
{
public:
    void Write(const std::string& filename, const Triangulation& triangulation);

private:
    // Using const references of Triangulation and Point classes
    std::string fomulateText(const Triangulation& triangulation, const Point& point);
};
