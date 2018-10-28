#pragma once
#include "Vec_data.h"
#include "RGBA_data.h"
#include "SDL2-2.0.8/include/SDL.h"
namespace Light
{

	float ambient;

	namespace internal
	{
		SDL_Texture *lightmap;
		int w, h;
	}

	struct Factory
	{
		Vec2D::Vec2D *pos;
		float *z_height;
		float *intensity;
		RGB::RGB *color;
		int array_size;
		int n_lights;
	};

}
