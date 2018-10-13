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

	//if this camera was placed on a grid, which cells of the grid would be under its grid_coord?
	void get_Grid_Region_Covered(Grid::Region *a, const Grid_Camera *c)
	{
		Grid::get_Region_Under_Shape(a, &c->grid_coord);
	}

	void grid_Coord_to_Screen_Coord(Shape::Rect *screen_coord,const Shape::Rect *grid_coord, const Grid_Camera *c)
	{
		screen_coord->x = c->init.screen_width*(grid_coord->x - c->grid_coord.x) / c->grid_coord.w;
		screen_coord->y = c->init.screen_height*(grid_coord->y - c->grid_coord.y) / c->grid_coord.h;
		screen_coord->w = c->init.screen_width*grid_coord->w / c->grid_coord.w;
		screen_coord->h = c->init.screen_height*grid_coord->h / c->grid_coord.h;
	}

	//must perform this if camera width or height changes
	//given a grid region that the camera grid_coord covers, recalculate tile data
	void calibrate_Tiles(Grid_Camera *c,const Grid::Region *a)
	{
		c->read_only.tile_w = ceil(c->init.screen_width / c->grid_coord.w);
		c->read_only.tile_h = ceil(c->init.screen_height / c->grid_coord.h);
		c->read_only.tile_x = ((float)a->x0 - c->grid_coord.x) * c->read_only.tile_w;
		c->read_only.tile_y = ((float)a->y0 - c->grid_coord.y) * c->read_only.tile_h;
	}
}