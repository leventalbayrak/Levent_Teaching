#pragma once
/*
TODO:
use custom allocator
*/
#include "Sprite_data.h"
#include "SDL2-2.0.8/include/SDL.h"
namespace Sprite
{
	void init(Instance *s, int array_size)
	{
		s->array_size = array_size;
		s->n_instances = 0;
		s->current_frame = (int*)malloc(sizeof(int)*s->array_size);
		s->frame_duration = (int*)malloc(sizeof(int)*s->array_size);
		s->last_frame_change_time = (unsigned int*)malloc(sizeof(unsigned int)*s->array_size);
	}

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
		s->instances = (Instance*)malloc(sizeof(Instance)*s->array_size);
		for (int i = 0; i < s->array_size; i++) init(&s->instances[i], 100);
	}

	
	void resize(Data *s)
	{
		int old_size = s->array_size;
		s->array_size += s->array_size >> 1;

		
		s->frame_w = (int*)realloc(s->frame_w, sizeof(int)*s->array_size);
		s->frame_h = (int*)realloc(s->frame_h, sizeof(int)*s->array_size);
		s->frame_pos_x = (int*)realloc(s->frame_pos_x, sizeof(int)*s->array_size);
		s->frame_pos_y = (int*)realloc(s->frame_pos_y, sizeof(int)*s->array_size);
		s->n_frames = (int*)realloc(s->n_frames, sizeof(int)*s->array_size);
		s->texture = (SDL_Texture**)realloc(s->texture, sizeof(SDL_Texture*)*s->array_size);
		s->instances = (Instance*)realloc(s->instances, sizeof(Instance)*s->array_size);
		for (int i = old_size; i < s->array_size; i++)
		{
			init(&s->instances[i], 100);
		}
	}

	void resize(Instance *s)
	{
		s->array_size += s->array_size >> 1;

		s->current_frame = (int*)realloc(s->current_frame, sizeof(int)*s->array_size);
		s->frame_duration = (int*)realloc(s->frame_duration, sizeof(int)*s->array_size);
		s->last_frame_change_time = (unsigned int*)realloc(s->last_frame_change_time, sizeof(unsigned int)*s->array_size);
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

	int make_Instance(int index, Data *d)
	{
		if (d->instances[index].n_instances >= d->instances[index].array_size)
		{
			resize(&d->instances[index]);
		}
		++d->instances[index].n_instances;
		return d->instances[index].n_instances - 1;
	}

	void modify(int index, int instance_index, Data *d, int frame_duration)
	{
		d->instances[index].current_frame[instance_index] = 0;
		d->instances[index].last_frame_change_time[instance_index] = SDL_MAX_UINT32;
		d->instances[index].frame_duration[instance_index] = frame_duration;
	}

	void update(int index, int instance_index, Data *d, unsigned int current_time)
	{
		unsigned int elapsed = current_time - d->instances[index].last_frame_change_time[instance_index];
		if (elapsed >= d->instances[index].frame_duration[instance_index])
		{
			d->instances[index].current_frame[instance_index] = (d->instances[index].current_frame[instance_index] + elapsed / d->instances[index].frame_duration[instance_index]) % d->n_frames[index];
			d->instances[index].last_frame_change_time[instance_index] = current_time;
		}	
	}

	void draw(int index, int instance_index, Data *d, int dest_x, int dest_y, int dest_w, int dest_h, SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = d->frame_pos_x[index] + d->frame_w[index] * d->instances[index].current_frame[instance_index];
		src.y = d->frame_pos_y[index];
		src.w = d->frame_w[index];
		src.h = d->frame_h[index];

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_RenderCopyEx(renderer, d->texture[index], &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}
}