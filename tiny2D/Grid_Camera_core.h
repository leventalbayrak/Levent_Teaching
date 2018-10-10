#pragma once
#include "Grid_Camera_data.h"
#include "Grid_data.h"
#include "Tilemap_data.h"

namespace Grid_Camera
{

	void camera_to_Screen(Grid::Active_Index_Range_Out *a,Tileset::Tile_Info *tile_info_out, const Grid_Camera *c)
	{
		a->x0 = floor(c->x);
		a->y0 = floor(c->y);
		a->x1 = (c->x + c->w);//always keep next cell active for partial drawing, otherwise subtract 1
		a->y1 = (c->y + c->h);

		tile_info_out->tile_w = ceil(c->screen_width / c->w);
		tile_info_out->tile_h = ceil(c->screen_height / c->h);

		tile_info_out->tile_x = ((float)a->x0 - c->x) * tile_info_out->tile_w;
		tile_info_out->tile_y = ((float)a->y0 - c->y) * tile_info_out->tile_h;
	}
}