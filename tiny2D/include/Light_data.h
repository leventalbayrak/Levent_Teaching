#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "Vec_data.h"
#include "RGBA_data.h"
#include "Spawn_Stack_data.h"
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
		
		Spawn_Stack::Spawn_Stack spawn_stack;
	};

}
