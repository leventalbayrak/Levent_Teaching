#pragma once
#include "SDL2-2.0.8/include/SDL.h"
namespace Sprite
{

	struct Instance
	{
		unsigned int *last_frame_change_time;
		int *frame_duration;

		int *current_frame;

		int n_instances;
		int array_size;
	};
	
	struct Data
	{
		int *n_frames;
		SDL_Texture **texture;
		int *frame_pos_x;
		int *frame_pos_y;
		int *frame_w;
		int *frame_h;

		Instance *instances;
		
		int n_sprites;
		int array_size;
	};

}