#pragma once
#include "SDL2-2.0.8/include/SDL.h"
namespace Sprite
{
	
	struct Data
	{
		int *n_frames;
		SDL_Texture **texture;
		int *frame_pos_x;
		int *frame_pos_y;
		int *frame_w;
		int *frame_h;
		
		int n_sprites;
		int array_size;
	};

	struct Animation
	{
		unsigned int *last_frame_change_time;
		int *frame_duration;

		int *current_frame;

		int *sprite_data_index;
		int *sprite_database_index;

		int n_animations;
		int array_size;
	};

}