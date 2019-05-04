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

SDL_Renderer *renderer = NULL;

class Sprite
{
	float x, y, w, h;
	float angular_vel;
	float angle;
	float vx, vy;
	unsigned int lifetime;
	unsigned int creation_time;
	
	int active;
	int r, g, b;
	
	int animation_frame;
	int number_of_frames;
	unsigned int last_time_animation_updated;
	unsigned int animation_speed;

	int frame_x;
	int frame_y;
	int frame_w;
	int frame_h;
	SDL_Texture *texture;
public:
	Sprite(SDL_Texture *tex, int _frame_x, int _frame_y,int _frame_w, int _frame_h, int n_frames, unsigned int _creation_time)
	{
		texture = tex;

		frame_x = _frame_x;
		frame_y = _frame_y;
		frame_w = _frame_w;
		frame_h = _frame_h;
		number_of_frames = n_frames;

		x = 0;
		y = 0;
		w = 64;
		h = 64;

		animation_speed = 100;

		creation_time = _creation_time;
		active = 1;
		lifetime = 2000 + rand() % 10000;
	}

	void set_Size(float _w, float _h)
	{
		w = _w;
		h = _h;
	}

	void update(unsigned int current_time)
	{
		if (current_time - creation_time > lifetime)
		{
			active = 0;
			return;
		}

		x += vx;
		y += vy;

		angle += angular_vel;

		if (current_time - last_time_animation_updated > animation_speed)
		{
			last_time_animation_updated = current_time;
			animation_frame++;
			animation_frame %= number_of_frames;
		}

	}

	void draw()
	{
		int current_frame_x = frame_x + animation_frame*frame_w;
		SDL_Rect src = {current_frame_x ,frame_y,frame_w,frame_h };
		SDL_Rect dest = { x,y,w,h };
		SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
	}


	void set_Velocity_and_Angle(float _vx, float _vy, float _angle_v)
	{
		vx = _vx;
		vy = _vy;
		angular_vel = _angle_v;
	}

	int get_Active()
	{
		return active;
	}

	void limit_Pos(float min_x, float max_x, float min_y, float max_y)
	{
		if (x < min_x || x > max_x-w)
		{
			vx *= -1;
		}

		if (y < min_y || y > max_y-h)
		{
			vy *= -1;
		}
	}
};

void spawn(Sprite **sprites, int n_sprites,SDL_Texture *texture, int how_many, unsigned int current_time)
{
	for (int i = 0; i < n_sprites; i++)
	{
		if (sprites[i] == NULL)
		{
			sprites[i] = new Sprite(texture, 0, 0, 64, 64, 4, current_time);
			sprites[i]->set_Size(8.0 + 32.0*rand() / RAND_MAX, 8.0 + 32.0*rand() / RAND_MAX);
			sprites[i]->set_Velocity_and_Angle(9 * (1.0 - 2.0*rand() / RAND_MAX), 9 * (1.0 - 2.0*rand() / RAND_MAX), 0.1*(1.0 - 2.0*rand() / RAND_MAX));
			how_many--;
			if (how_many < 0) return;
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

	int n_sprites = 100000;
	Sprite **sprites = new Sprite *[n_sprites];
	for (int i = 0; i < n_sprites; i++) sprites[i] = NULL;

	printf("TRYING TO DRAW\n");

	unsigned int frame_counter = 0;
	unsigned int last_frame_time = SDL_GetTicks();
	unsigned int last_time_spawned = 0;

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

		/*
		update	
		*/

		if (current_time - last_time_spawned > 1000)
		{
			last_time_spawned = current_time;
			spawn(sprites, n_sprites, texture, 10000,current_time);
		}

		for (int i = 0; i < n_sprites; i++)
		{
			if (sprites[i] == NULL) continue;
		
			sprites[i]->update(current_time);

			if (sprites[i]->get_Active() == 0)
			{
				delete sprites[i];
				sprites[i] = NULL;
				continue;
			}

			sprites[i]->limit_Pos(0, 800, 0, 600);
		}

		/*
		draw
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < n_sprites; i++)
		{
			if (sprites[i] == NULL) continue;
			sprites[i]->draw();
		}


		SDL_RenderPresent(renderer);
	}

	return 0;
}