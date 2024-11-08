#pragma once
#include "Triangulation.h"
using namespace Geometry;

namespace transformya
{
	class Transformation
	{
	public:
		Transformation();
		~Transformation();

		Triangulation scale(const Triangulation& tri, double scaleFactor);
	};
}

