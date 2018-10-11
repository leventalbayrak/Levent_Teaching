#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Grid_data.h"
#include "Shape_data.h"
namespace Grid
{
	void init(Grid *g, int n_rows, int n_cols)
	{
		g->n_rows = n_rows;
		g->n_cols = n_cols;
		g->data = (int*)malloc(sizeof(int)*n_cols*n_rows);
		memset(g->data, 0, sizeof(int)*n_cols*n_rows);
	}

	void get_Region_Under_Shape(Region *g, const Shape::Rect *r)
	{
		g->x0 = floor(r->x);
		g->y0 = floor(r->y);
		g->x1 = ceil(r->x + r->w);
		g->y1 = ceil(r->y + r->h);
	}

}