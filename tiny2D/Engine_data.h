#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "Tileset_data.h"
#include "Sprite_data.h"
#include "Grid_Camera_core.h"

namespace Engine
{
	SDL_Renderer *renderer = NULL;
	SDL_Window *window = NULL;
	int screen_width;
	int screen_height;

	Tileset::Tileset tileset_db;
	Sprite::Data sprite_db;

}