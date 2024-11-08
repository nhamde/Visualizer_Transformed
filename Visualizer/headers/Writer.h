#pragma once
#include<string>
#include<vector>
#include"Triangulation.h"
using namespace Geometry;

class Writer
{
	// Vitual function to write triangle data into file by filename
	virtual void Write(const std::string& filename, const Triangulation& trianglation) = 0;
};

