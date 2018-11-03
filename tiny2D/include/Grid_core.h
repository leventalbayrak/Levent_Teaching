#pragma once
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Grid_data.h"
#include "Table_File_core.h"
#include "Shape_data.h"
#include "RGBA_data.h"

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

	void imprint_Set(Grid *g, int value, const Shape::Rect::Data *r)
	{
		int x0 = r->x;
		int y0 = r->y;
		int x1 = r->x + r->w;
		int y1 = r->y + r->h;
		
		for (int y = y0; y <= y1; y++)
		{
			for (int x = x0; x <= x1; x++)
			{
				int index = y * g->n_cols + x;
				g->data[index] = value;
			}
		}
	}

	void imprint_Add(Grid *g, int value, const Shape::Rect::Data *r)
	{
		int x0 = r->x;
		int y0 = r->y;
		int x1 = r->x + r->w;
		int y1 = r->y + r->h;

		for (int y = y0; y <= y1; y++)
		{
			for (int x = x0; x <= x1; x++)
			{
				int index = y * g->n_cols + x;
				g->data[index] += value;
			}
		}
	}

	//returns -1 if no unignored tile is overlapped, tile_id otherwise
	int get_First_Overlapped_Tile(int ignore_tile_id, const Shape::Rect::Data *rect, const Grid *g)
	{
		Region region;
		get_Region_Under_Shape(&region, rect);
		clip_Grid_Region(&region, g->n_cols, g->n_rows);

		for (int y = region.y0; y <= region.y1; y++)
		{
			for (int x = region.x0; x <= region.x1; x++)
			{
				int tile_value = g->data[y*g->n_cols + x];
				if (tile_value != ignore_tile_id)
				{
					return tile_value;
				}
			}
		}

		return -1;
	}

	void draw_Float(float *grid,int n_rows, int n_cols, const Grid_Camera::Grid_Camera *cam, SDL_Renderer *renderer)
	{
		int x0 = cam->world_coord.x;
		int y0 = cam->world_coord.y;
		int x1 = cam->world_coord.x + cam->world_coord.w;
		int y1 = cam->world_coord.y + cam->world_coord.h;

		int ty = cam->calibration.tile_y;
		for (int i = y0; i <= y1; i++)
		{
			int tx = cam->calibration.tile_x;
			for (int j = x0; j <= x1; j++)
			{
				SDL_Rect dest;
				dest.x = tx;
				dest.y = ty;
				dest.w = cam->calibration.tile_w;
				dest.h = cam->calibration.tile_h;

				RGB::RGB color = {};
				
				if (grid[i*n_cols + j] < 0.333333)
				{
					color.r = 255.0*grid[i*n_cols + j] / 0.333333;
				}
				else if (grid[i*n_cols + j] < 0.666666)
				{
					color.g = 255.0*(grid[i*n_cols + j]-0.333333) / 0.333333;
				}
				else
				{
					color.b = 255.0*(grid[i*n_cols + j] - 0.666666) / 0.333333;
				}

				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
				SDL_RenderFillRect(renderer, &dest);
			
				tx += cam->calibration.tile_w;
			}
			ty += cam->calibration.tile_h;
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