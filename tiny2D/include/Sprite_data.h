#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "Spawn_Stack_data.h"
namespace Sprite
{
	namespace Instance
	{
		struct Instance
		{
			unsigned int *last_frame_change_time;
			int *frame_duration;

			int *current_frame;
			int array_size;

			Spawn_Stack::Spawn_Stack spawn_stack;
			
		};
	}

	struct Factory
	{
		struct
		{
			int n_frames;
			SDL_Texture *texture;
			int frame_pos_x;
			int frame_pos_y;
			int frame_w;
			int frame_h;
		} texture_info;

		Instance::Instance instances;
	};
}