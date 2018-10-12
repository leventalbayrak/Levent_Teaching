#pragma once
/*
TODO:
use custom allocator
*/
#include "Sprite_data.h"
#include "SDL2-2.0.8/include/SDL.h"
namespace Sprite
{

	void init(Data *s, int array_size)
	{
		s->array_size = array_size;
		s->n_sprites = 0;
		s->frame_w = (int*)malloc(sizeof(int)*s->array_size);
		s->frame_h = (int*)malloc(sizeof(int)*s->array_size);
		s->frame_pos_x = (int*)malloc(sizeof(int)*s->array_size);
		s->frame_pos_y = (int*)malloc(sizeof(int)*s->array_size);
		s->n_frames = (int*)malloc(sizeof(int)*s->array_size);
		s->texture = (SDL_Texture**)malloc(sizeof(SDL_Texture*)*s->array_size);
	}

	void init(Animation *s, int array_size)
	{
		s->array_size = array_size;
		s->n_animations = 0;
		s->current_frame = (int*)malloc(sizeof(int)*s->array_size);
		s->frame_duration = (int*)malloc(sizeof(int)*s->array_size);
		s->last_frame_change_time = (unsigned int*)malloc(sizeof(unsigned int)*s->array_size);
		s->sprite_data_index = (int*)malloc(sizeof(int)*s->array_size);
	}

	void resize(Data *s)
	{
		s->array_size += s->array_size >> 1;

		
		s->frame_w = (int*)realloc(s->frame_w, sizeof(int)*s->array_size);
		s->frame_h = (int*)realloc(s->frame_h, sizeof(int)*s->array_size);
		s->frame_pos_x = (int*)realloc(s->frame_pos_x, sizeof(int)*s->array_size);
		s->frame_pos_y = (int*)realloc(s->frame_pos_y, sizeof(int)*s->array_size);
		s->n_frames = (int*)realloc(s->n_frames, sizeof(int)*s->array_size);
		s->texture = (SDL_Texture**)realloc(s->texture, sizeof(SDL_Texture*)*s->array_size);
	}

	void resize(Animation *s)
	{
		s->array_size += s->array_size >> 1;

		s->current_frame = (int*)realloc(s->current_frame, sizeof(int)*s->array_size);
		s->frame_duration = (int*)realloc(s->frame_duration, sizeof(int)*s->array_size);
		s->last_frame_change_time = (unsigned int*)realloc(s->last_frame_change_time, sizeof(unsigned int)*s->array_size);
		s->sprite_data_index = (int*)realloc(s->sprite_data_index, sizeof(int)*s->array_size);
	}

	struct Data_Args
	{
		int frame_pos_x;
		int frame_pos_y;
		int frame_w;
		int frame_h;
		int n_frames;
	};

	int make(Data *s)
	{
		if (s->n_sprites >= s->array_size)
		{
			resize(s);
		}
		++s->n_sprites;
		return s->n_sprites - 1;
	}

	void modify(int index,Data *s, const Data_Args *p, const SDL_Texture *texture)
	{
		s->frame_w[index] = p->frame_w;
		s->frame_h[index] = p->frame_h;
		s->n_frames[index] = p->n_frames;
		s->frame_pos_x[index] = p->frame_pos_x;
		s->frame_pos_y[index] = p->frame_pos_y;
		s->texture[index] = (SDL_Texture *)texture;
	}

	int make(Animation *a)
	{
		if (a->n_animations >= a->array_size)
		{
			resize(a);
		}
		++a->n_animations;
		return a->n_animations - 1;
	}

	void modify(int index, Animation *a, int db_index, int sprite_index, int frame_duration)
	{
		a->sprite_database_index[index] = db_index;
		a->sprite_data_index[index] = sprite_index;
		a->current_frame[index] = 0;
		a->last_frame_change_time[index] = SDL_MAX_UINT32;
		a->frame_duration[index] = frame_duration;
	}

	void update(int index, Animation *s, const Data *d, unsigned int current_time)
	{
		unsigned int elapsed = current_time - s->last_frame_change_time[index];
		if (elapsed >= s->frame_duration[index])
		{
			s->current_frame[index] = (s->current_frame[index] + elapsed/s->frame_duration[index]) % d->n_frames[s->sprite_data_index[index]];
			s->last_frame_change_time[index] = current_time;
		}	
	}

	void draw(int index, Animation *a, Data *d, int dest_x, int dest_y, int dest_w, int dest_h, SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = d->frame_pos_x[a->sprite_data_index[index]] + d->frame_w[a->sprite_data_index[index]] * a->current_frame[index];
		src.y = d->frame_pos_y[a->sprite_data_index[index]];
		src.w = d->frame_w[a->sprite_data_index[index]];
		src.h = d->frame_h[a->sprite_data_index[index]];

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_RenderCopyEx(renderer, d->texture[a->sprite_data_index[index]], &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}
}