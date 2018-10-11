#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "Tilemap_core.h"
#include "Grid_Camera_core.h"

namespace Engine
{
	SDL_Renderer *renderer = NULL;
	SDL_Window *window = NULL;
	int screen_width;
	int screen_height;

	Grid_Camera::Grid_Camera camera;
}