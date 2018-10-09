#pragma once
#include "Grid_Camera_data.h"
#include "Grid_data.h"

namespace Grid_Camera
{
	void translate(Grid::Active_Range *a, const Grid_Camera *c)
	{
		a->x0 = floor(c->x);
		a->y0 = floor(c->y);
		a->x1 = ceil(c->x + c->w);
		a->y1 = ceil(c->y + c->h);
	}
}