#pragma once
#include "Grid_Camera_data.h"
#include "Grid_core.h"


namespace Grid_Camera
{
	void init(Grid_Camera *c, int screen_width,int screen_height)
	{
		*c = {};
		c->init.screen_width = screen_width;
		c->init.screen_height = screen_height;
	}

	//if this camera was placed on a grid, which cells of the grid would be under its canvas?
	void get_Grid_Region_Covered_by_Canvas(Grid::Region *a, const Grid_Camera *c)
	{
		Grid::get_Region_Under_Shape(a, &c->canvas);
	}

	//must perform this if camera width or height changes
	//given a grid region that the camera canvas covers, recalculate tile data
	void calibrate_Tiles(Grid_Camera *c,const Grid::Region *a)
	{
		c->read_only.tile_w = ceil(c->init.screen_width / c->canvas.w);
		c->read_only.tile_h = ceil(c->init.screen_height / c->canvas.h);
		c->read_only.tile_x = ((float)a->x0 - c->canvas.x) * c->read_only.tile_w;
		c->read_only.tile_y = ((float)a->y0 - c->canvas.y) * c->read_only.tile_h;
	}
}