#pragma once

#include "Shape_data.h"
#include "Grid_data.h"

namespace Grid_Camera
{
	

	struct Grid_Camera
	{
		Shape::Rect world_coord;

		//initialize
		struct
		{
			int screen_width;
			int screen_height;
		} init;

		//read only first tile screen position and single tile dimensions
		struct 
		{
			int tile_x;
			int tile_y;
			int tile_w;
			int tile_h;
		} calibration;

		Grid::Region grid_region_covered;

	};
}