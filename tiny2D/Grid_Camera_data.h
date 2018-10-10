#pragma once

namespace Grid_Camera
{
	struct Grid_Camera
	{
		float x, y;//which grid cell
		float w, h;//width and height in grid cells

		//initialize
		struct
		{
			int screen_width;
			int screen_height;
		} init;

		//read only
		struct {
			int tile_w;
			int tile_h;
			int tile_x;
			int tile_y;
		} read_only;
	};
}