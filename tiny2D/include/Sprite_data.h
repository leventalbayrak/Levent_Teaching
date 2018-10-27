#pragma once
#include "SDL2-2.0.8/include/SDL.h"
namespace Sprite
{
	namespace Instance
	{
		struct Instance
		{
			unsigned int *last_frame_change_time;
			int *frame_duration;

			int *current_frame;

			int n_instances;
			int array_size;
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