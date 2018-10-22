#pragma once

#include "Shape_data.h"

namespace Grid_Camera
{
	//read only first tile screen position and single tile dimensions
	struct Calibration
	{
		int tile_x;
		int tile_y;
		int tile_w;
		int tile_h;
	};

	struct Grid_Camera
	{
		Shape::Rect world_coord;

		//initialize
		struct
		{
			int screen_width;
			int screen_height;
		} init;

	};
}