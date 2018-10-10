#pragma once
#include "Grid_Camera_data.h"
#include "Grid_data.h"

namespace Grid_Camera
{
	void init(Grid_Camera *c, int screen_width, int screen_height);

	void set_Viewport(Grid_Camera *c, int grid_col, int grid_row, int width_n_grid_cells, int height_n_grid_cells);

	//must perform this if camera width or height changes
	void recalculate_Tile_Position_and_Size(Grid::Active_Index_Range_Out *a, Grid_Camera *c);

	//must perform this if only the camera position changes but not the size
	void recalculate_Tile_Position(Grid::Active_Index_Range_Out *a, Grid_Camera *c);
}

namespace Grid_Camera
{
	void init(Grid_Camera *c, int screen_width,int screen_height)
	{
		*c = {};
		c->init.screen_width = screen_width;
		c->init.screen_height = screen_height;
	}

	void set_Viewport(Grid_Camera *c, int grid_col, int grid_row, int width_n_grid_cells, int height_n_grid_cells)
	{
		c->x = grid_col;
		c->y = grid_row;
		c->w = width_n_grid_cells;
		c->h = height_n_grid_cells;

		Grid::Active_Index_Range_Out a;
		recalculate_Tile_Position_and_Size(&a,c);
	}

	//must perform this if camera width or height changes
	void recalculate_Tile_Position_and_Size(Grid::Active_Index_Range_Out *a, Grid_Camera *c)
	{
		a->x0 = floor(c->x);
		a->y0 = floor(c->y);
		a->x1 = (c->x + c->w);//always keep next cell active for partial drawing, otherwise subtract 1
		a->y1 = (c->y + c->h);

		c->read_only.tile_w = ceil(c->init.screen_width / c->w);
		c->read_only.tile_h = ceil(c->init.screen_height / c->h);

		c->read_only.tile_x = ((float)a->x0 - c->x) * c->read_only.tile_w;
		c->read_only.tile_y = ((float)a->y0 - c->y) * c->read_only.tile_h;
	}

	//must perform this if only the camera position changes but not the size
	void recalculate_Tile_Position(Grid::Active_Index_Range_Out *a, Grid_Camera *c)
	{
		a->x0 = floor(c->x);
		a->y0 = floor(c->y);
		a->x1 = (c->x + c->w);//always keep next cell active for partial drawing, otherwise subtract 1
		a->y1 = (c->y + c->h);

		c->read_only.tile_x = ((float)a->x0 - c->x) * c->read_only.tile_w;
		c->read_only.tile_y = ((float)a->y0 - c->y) * c->read_only.tile_h;
	}
}