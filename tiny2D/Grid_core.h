#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Grid_data.h"
#include "Grid_File_core.h"
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
		/*g->x0 = floor(r->x);
		g->y0 = floor(r->y);
		g->x1 = floor(r->x + r->w + 0.5);
		g->y1 = floor(r->y + r->h + 0.5);
*/
		g->x0 = (r->x);
		g->y0 = (r->y);
		g->x1 = (r->x + r->w);
		g->y1 = (r->y + r->h);
	}

	void clip_Grid_Region(Region *r, const Grid *g)
	{
		if (r->x0 < 0) r->x0 = 0;
		if (r->y0 < 0) r->y0 = 0;
		if (r->x1 >= g->n_cols) r->x1 = g->n_cols - 1;
		if (r->y1 >= g->n_rows) r->y1 = g->n_rows - 1;
	}

	void load(Grid *g, const char *filename)
	{
		Grid_File::Grid_File t;
		Grid_File::read(filename, &t);
		for (int i = 1; i < t.nrows; i++)
		{
			if (t.ncols[i] != t.ncols[i - 1])
			{
				assert(0);
				return;
			}
		}
		init(g, t.nrows, t.ncols[0]);
		for (int i = 0; i < t.nrows; i++)
		{
			for (int j = 0; j < t.ncols[0]; j++)
			{
				g->data[i*g->n_cols + j] = atoi(t.table[i][j]);
			}
		}

		Grid_File::clean(&t);
	}
}