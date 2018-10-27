#pragma once
/*
TODO:
use custom allocator
*/
#include "Sprite_data.h"
#include "Texture_core.h"
#include "SDL2-2.0.8/include/SDL.h"
namespace Sprite
{
	namespace Instance
	{
		void init(Instance *s, int array_size)
		{
			s->array_size = array_size;
			s->n_instances = 0;
			s->current_frame = (int*)malloc(sizeof(int)*s->array_size);
			s->frame_duration = (int*)malloc(sizeof(int)*s->array_size);
			s->last_frame_change_time = (unsigned int*)malloc(sizeof(unsigned int)*s->array_size);
		}

		void resize(Instance *s)
		{
			s->array_size += s->array_size >> 1;

			s->current_frame = (int*)realloc(s->current_frame, sizeof(int)*s->array_size);
			s->frame_duration = (int*)realloc(s->frame_duration, sizeof(int)*s->array_size);
			s->last_frame_change_time = (unsigned int*)realloc(s->last_frame_change_time, sizeof(unsigned int)*s->array_size);
		}
	}

	int make_Instance(Factory *d)
	{
		if (d->instances.n_instances >= d->instances.array_size)
		{
			resize(&d->instances);
		}
		++d->instances.n_instances;
		return d->instances.n_instances - 1;
	}

	void modify(int instance_index, Factory *d, int frame_duration)
	{
		d->instances.current_frame[instance_index] = 0;
		d->instances.last_frame_change_time[instance_index] = SDL_MAX_UINT32;
		d->instances.frame_duration[instance_index] = frame_duration;
	}

	void update(int instance_index, Factory *d, unsigned int current_time)
	{
		unsigned int elapsed = current_time - d->instances.last_frame_change_time[instance_index];
		if (elapsed >= d->instances.frame_duration[instance_index])
		{
			d->instances.current_frame[instance_index] = (d->instances.current_frame[instance_index] + elapsed / d->instances.frame_duration[instance_index]) % d->texture_info.n_frames;
			d->instances.last_frame_change_time[instance_index] = current_time;
		}	
	}

	void draw(int instance_index, Factory *d, int dest_x, int dest_y, int dest_w, int dest_h, SDL_Renderer *renderer, int flip, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		SDL_Rect src;
		src.x = d->texture_info.frame_pos_x + d->texture_info.frame_w * d->instances.current_frame[instance_index];
		src.y = d->texture_info.frame_pos_y;
		src.w = d->texture_info.frame_w;
		src.h = d->texture_info.frame_h;

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_SetTextureAlphaMod(d->texture_info.texture, a);
		SDL_SetTextureColorMod(d->texture_info.texture, r, g, b);

		if (flip == 0)
		{
			SDL_RenderCopyEx(renderer, d->texture_info.texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);
		}
		else
		{
			SDL_RenderCopyEx(renderer, d->texture_info.texture, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
	}

	namespace File
	{
		void add(Factory *sprite_database, const char *filename, SDL_Renderer *renderer)
		{
			sprite_database->texture_info.texture = NULL;
			
			char img_filename[128];
			FILE *f = fopen(filename, "r");
			int r = fscanf(f, 
					"img=%s frame_x=%d frame_y=%d frame_w=%d frame_h=%d n_frames=%d\n", 
					img_filename,
					&sprite_database->texture_info.frame_pos_x, 
					&sprite_database->texture_info.frame_pos_y, 
					&sprite_database->texture_info.frame_w, 
					&sprite_database->texture_info.frame_h, 
					&sprite_database->texture_info.n_frames);
			if (r != 6) return;

			sprite_database->texture_info.texture = Texture::load(img_filename, renderer);
		}
	}
}