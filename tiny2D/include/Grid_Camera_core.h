#pragma once
#include "Vec_core.h"
#include "Grid_Camera_data.h"
#include "Grid_core.h"

namespace Grid_Camera
{
	void init(Grid_Camera *c, int screen_width, int screen_height);

	void grid_to_Screen(Shape::Rect::Data *screen_coord, const Shape::Rect::Data *world_coord, const Grid_Camera *cam);

	void screen_to_Grid(Vec2D::Vec2D *grid_point, Vec2D::Vec2D *screen_point, const Grid_Camera *c);

	//must perform this if camera width or height changes
	//given a grid region that the camera grid_coord covers, recalculate tile data
	void calibrate(Grid_Camera *cam);
}

namespace Grid_Camera
{
	void init(Grid_Camera *c, int screen_width,int screen_height)
	{
		*c = {};
		c->init.screen_width = screen_width;
		c->init.screen_height = screen_height;
	}

	void grid_to_Screen(Shape::Rect::Data *screen_coord,const Shape::Rect::Data *world_coord, const Grid_Camera *cam)
	{
		screen_coord->x = (world_coord->x - cam->world_coord.x)*cam->calibration.tile_w;
		screen_coord->y = (world_coord->y - cam->world_coord.y)*cam->calibration.tile_h;
		screen_coord->w = world_coord->w*cam->calibration.tile_w;
		screen_coord->h = world_coord->h*cam->calibration.tile_h;
	}

	void screen_to_Grid(Vec2D::Vec2D *grid_point, Vec2D::Vec2D *screen_point, const Grid_Camera *c)
	{
		
		grid_point->x = c->world_coord.x + c->world_coord.w*(screen_point->x / c->init.screen_width);
		grid_point->y = c->world_coord.y + c->world_coord.h*(screen_point->y / c->init.screen_height);
	}

	//must perform this if camera width or height changes
	//given a grid region that the camera grid_coord covers, recalculate tile data
	void calibrate(Grid_Camera *cam)
	{

		int x0 = cam->world_coord.x;
		int y0 = cam->world_coord.y;
		
		cam->calibration.tile_w = ceil(cam->init.screen_width / cam->world_coord.w);
		cam->calibration.tile_h = ceil(cam->init.screen_height / cam->world_coord.h);
		cam->calibration.tile_x = (x0 - cam->world_coord.x) * cam->calibration.tile_w;
		cam->calibration.tile_y = (y0 - cam->world_coord.y) * cam->calibration.tile_h;
	}
}