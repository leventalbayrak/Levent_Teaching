#pragma once

#include "Shape_data.h"

namespace Grid_Camera
{
	struct Grid_Camera
	{
		Shape::Rect canvas;

		//initialize
		struct
		{
			int screen_width;
			int screen_height;
		} init;

		//read only first tile screen position and single tile dimensions
		struct {
			int tile_w;
			int tile_h;
			int tile_x;
			int tile_y;
		} read_only;
	};
}