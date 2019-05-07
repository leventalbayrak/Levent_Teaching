#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

#include "Vec2D_data.h"
SDL_Renderer *renderer = NULL;
namespace Sprite
{
	struct Sprite
	{
		SDL_Texture *tex;
		Vec2D::Vec2D frame_pos;
		Vec2D::Vec2D frame_size;
		unsigned int animation_speed;
		int number_of_frames;


		Vec2D::Vec2D *pos;
		Vec2D::Vec2D *vel;
		Vec2D::Vec2D *size;

		unsigned int *lifetime;
		unsigned int *creation_time;
		unsigned int *last_frame_update;
		unsigned char *current_frame;


		unsigned char *active;

		int array_size;

	};


	void init(Sprite *s, int array_size, Vec2D::Vec2D *frame_pos, Vec2D::Vec2D *frame_size, SDL_Texture *texture, unsigned int animation_speed)
	{
		s->frame_pos = *frame_pos;
		s->frame_size = *frame_size;
		s->animation_speed = animation_speed;
		s->tex = texture;
		s->number_of_frames = 4;
		s->array_size = array_size;
		s->active = (unsigned char*)malloc(sizeof(unsigned char)*array_size);
		s->current_frame = (unsigned char*)malloc(sizeof(unsigned char)*array_size);
		s->creation_time = (unsigned int*)malloc(sizeof(unsigned int)*array_size);
		s->lifetime = (unsigned int*)malloc(sizeof(unsigned int)*array_size);
		s->last_frame_update = (unsigned int*)malloc(sizeof(unsigned int)*array_size);
		s->pos = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*array_size);
		s->vel = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*array_size);
		s->size = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*array_size);

		for (int i = 0; i < array_size; i++) s->active[i] = 0;
	}

	void create(Sprite *s, unsigned int current_time, int how_many)
	{
		for (int i = 0; i < s->array_size; i++)
		{
			if (s->active[i] == 0)
			{
				s->active[i] = 1;
				s->pos[i] = { 0.0, 0.0 };
				s->vel[i] = { (float)(9.0 * (1.0 - 2.0*rand() / RAND_MAX)),(float)(9.0 * (1.0 - 2.0*rand() / RAND_MAX)) };
				s->size[i] = { (float)(8.0 + 32.0*rand() / RAND_MAX),(float)(8.0 + 32.0*rand() / RAND_MAX) };
				s->creation_time[i] = current_time;
				s->lifetime[i] = 2000 + rand() % 10000;

				how_many--;
				if (how_many <= 0) break;
			}
		}
	}

	void update(Sprite *s, const Vec2D::Vec2D *x_bound, const Vec2D::Vec2D *y_bound, unsigned int current_time)
	{
		for (int i = 0; i < s->array_size; i++)
		{
			if (s->active[i] == 1)
			{
				if (current_time - s->creation_time[i] > s->lifetime[i])
				{
					s->active[i] = 0;
					continue;
				}

				s->pos[i].x += s->vel[i].x;
				s->pos[i].y += s->vel[i].y;

				if (s->pos[i].x < x_bound->x || s->pos[i].x > x_bound->y)
				{
					s->vel[i].x *= -1;
				}
				if (s->pos[i].y < y_bound->x || s->pos[i].y > y_bound->y)
				{
					s->vel[i].y *= -1;
				}

				if (current_time - s->last_frame_update[i] > s->animation_speed)
				{
					s->last_frame_update[i] = current_time;
					s->current_frame[i]++;
					s->current_frame[i] %= s->number_of_frames;
				}

			}
		}
	}

	void draw(Sprite *s)
	{
		for (int i = 0; i < s->array_size; i++)
		{
			if (s->active[i] == 1)
			{
				int current_frame_x = s->frame_pos.x + s->current_frame[i] * s->frame_size.x;
				SDL_Rect src = { current_frame_x ,s->frame_pos.y,s->frame_size.x,s->frame_size.y };
				SDL_Rect dest = { s->pos[i].x,s->pos[i].y,s->size[i].x,s->size[i].y };
				SDL_RenderCopyEx(renderer, s->tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
			}
		}
	}
}




int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("james", 50, 50, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load("images/dvd.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	printf("TRYING TO DRAW\n");

	unsigned int frame_counter = 0;
	unsigned int last_frame_time = SDL_GetTicks();
	unsigned int last_time_spawned = 0;


	Sprite::Sprite sprites;
	Vec2D::Vec2D frame_pos = { 0,0 };
	Vec2D::Vec2D frame_size = { 64,64 };
	Sprite::init(&sprites, 100000, &frame_pos, &frame_size, texture, 100);

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;
		}

		unsigned int current_time = SDL_GetTicks();

		if (frame_counter++ > 100)
		{
			frame_counter = 0;
			float elapsed = current_time - last_frame_time;
			printf("frame time: %.2f\n", elapsed / 100.0);

			last_frame_time = current_time;
		}

		if (current_time - last_time_spawned > 1000)
		{
			last_time_spawned = current_time;
			Sprite::create(&sprites, current_time, 1000);
		}

		/*
		update
		*/
		Vec2D::Vec2D x_bound = { 0,800 };
		Vec2D::Vec2D y_bound = { 0,600 };
		Sprite::update(&sprites, &x_bound, &y_bound, current_time);

		/*
		draw
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		Sprite::draw(&sprites);

		SDL_RenderPresent(renderer);
	}

	return 0;
}