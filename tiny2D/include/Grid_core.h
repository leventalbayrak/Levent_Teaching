#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Grid_data.h"
#include "Table_File_core.h"
#include "Shape_data.h"

namespace Grid
{
	void init(Grid *g, int n_rows, int n_cols);

	inline void Vec2D_to_Grid(Point *gp, Vec2D::Vec2D *p);

	inline void get_Region_Under_Shape(Region *g, const Shape::Rect::Data *r);

	inline int tile(int x, int y, Grid *g);

	inline int tile(Point *p, Grid *g);

	inline int tile(Vec2D::Vec2D *p, Grid *g);

	void clip_Grid_Region(Region *r, int n_cols, int n_rows);

	void load(Grid *g, const char *filename);
}

namespace Grid
{
	void init(Grid *g, int n_rows, int n_cols)
	{
		g->n_rows = n_rows;
		g->n_cols = n_cols;
		g->data = (int*)malloc(sizeof(int)*n_cols*n_rows);
		memset(g->data, 0, sizeof(int)*n_cols*n_rows);
	}


	inline void Vec2D_to_Grid(Point *gp, Vec2D::Vec2D *p)
	{
		gp->x = p->x;
		gp->y = p->y;
	}

	inline void get_Region_Under_Shape(Region *g, const Shape::Rect::Data *r)
	{
		g->x0 = (r->x);
		g->y0 = (r->y);
		g->x1 = (r->x + r->w);
		g->y1 = (r->y + r->h);
	}

	inline int tile(int x, int y, Grid *g)
	{
		return g->data[y*g->n_cols + x];
	}

	inline int tile(Point *p, Grid *g)
	{
		return g->data[p->y*g->n_cols + p->x];
	}

	inline int tile(Vec2D::Vec2D *p, Grid *g)
	{
		return g->data[(int)p->y*g->n_cols + (int)p->x];
	}

	void clip_Grid_Region(Region *r, int n_cols, int n_rows)
	{
		if (r->x0 < 0) r->x0 = 0;
		if (r->y0 < 0) r->y0 = 0;
		if (r->x1 >= n_cols) r->x1 = n_cols - 1;
		if (r->y1 >= n_rows) r->y1 = n_rows - 1;
	}

	//returns rightmost or bottommost collision tile coords
//0 bottom 1 top

	void load(Grid *g, const char *filename)
	{
		Table_File::Table_File t;
		Table_File::read(filename, &t);
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

		Table_File::clean(&t);
	}
}