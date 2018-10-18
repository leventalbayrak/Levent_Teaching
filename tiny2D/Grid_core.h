#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Grid_data.h"
#include "Table_File_core.h"
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
		g->x0 = (r->x);
		g->y0 = (r->y);
		g->x1 = (r->x + r->w);
		g->y1 = (r->y + r->h);
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
//0 left 1 right
	void handle_Collision_Ugly(int &vert_col, int &vert_row, int &vert_dir, int &hor_col, int &hor_row, int &hor_dir, Shape::Rect *p, Grid *g)
	{
		vert_dir = -1;
		hor_dir = -1;

		Region grid_under_collision_rect;
		get_Region_Under_Shape(&grid_under_collision_rect, p);
		clip_Grid_Region(&grid_under_collision_rect, g->n_cols, g->n_rows);
		for (int i = grid_under_collision_rect.y0; i <= grid_under_collision_rect.y1; i++)
		{
			for (int j = grid_under_collision_rect.x0; j <= grid_under_collision_rect.x1; j++)
			{
				if (g->data[i*g->n_cols + j] == 0) continue;

				if (p->y + p->h > i && p->y < i)
				{
					vert_col = j;
					vert_row = i;
					vert_dir = 0;
				}
				else
				{
					vert_col = j;
					vert_row = i;
					vert_dir = 1;
				}

				if (p->x + p->w > j && p->x < j)
				{
					hor_col = j;
					hor_row = i;
					hor_dir = 1;
				}
				else
				{
					hor_col = j;
					hor_row = i;
					hor_dir = 0;
				}

			}
		}
	}


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