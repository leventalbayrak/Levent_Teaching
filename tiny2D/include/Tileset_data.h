#pragma once

#include "SDL2-2.0.8\include\SDL.h"

namespace Tileset
{
	struct Tileset
	{
		SDL_Texture **tex;
		int *tile_w;
		int *tile_h;
		int *n_rows;
		int *n_cols;
		int n_tilesets;
		int array_size;
	};

}