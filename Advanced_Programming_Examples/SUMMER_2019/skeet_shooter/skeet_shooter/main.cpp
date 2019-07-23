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
int screen_w = 1280;
int screen_h = 800;

#define R() (1.0-2.0*rand()/RAND_MAX)

struct Vec
{
	float x, y;
};

struct Skeet
{
	Vec *pos;
	Vec *vel;
	Vec *accel;
	float *size;
	char *active;

	int n_skeets;
};

void init(Skeet *s)
{
	s->n_skeets = 100;
	s->pos = (Vec*)malloc(sizeof(Vec) * s->n_skeets);
	s->vel = (Vec*)malloc(sizeof(Vec) * s->n_skeets);
	s->accel = (Vec*)malloc(sizeof(Vec) * s->n_skeets);
	s->size = (float*)malloc(sizeof(float) * s->n_skeets);
	s->active = (char*)calloc(s->n_skeets,sizeof(char));
}

int create(Skeet *s)
{
	for (int i = 0; i < s->n_skeets; i++)
	{
		if (s->active[i] == 0)
		{
			s->active[i] = 1;
			return i;
		}
	}
	return -1;
}

void create_Skeet(Skeet *s, float x, float y, float size)
{
	int k = create(s);

	s->vel[k] = { 0 };
	s->accel[k] = { 0 };

	s->pos[k].x = x;
	s->pos[k].y = y;
	s->size[k] = size;
	s->active[k] = 1;
}

void update(Skeet *s, float dt)
{
	for (int i = 0; i < s->n_skeets; i++)
	{
		if (s->active[i] == 1)
		{
			s->vel[i].x += s->accel[i].x * dt;
			s->pos[i].x += s->vel[i].x * dt;

			s->vel[i].y += s->accel[i].y * dt;
			s->pos[i].y += s->vel[i].y * dt;
		}
	}
}

void draw(Skeet *s)
{
	for (int i = 0; i < s->n_skeets; i++)
	{
		if (s->active[i] == 1)
		{
			SDL_Rect rect1 = { s->pos[i].x, s->pos[i].y, s->size[i],s->size[i] };
			SDL_RenderDrawRect(renderer, &rect1);

			SDL_Rect rect2 = { s->pos[i].x + s->size[i]*0.25, s->pos[i].y + s->size[i]*0.75, s->size[i]*0.5,s->size[i]*0.25 };
			SDL_RenderDrawRect(renderer, &rect2);
		}
	}
}

//AABB
int collision(float x, float y, float w, float h, float mx, float my)
{
	if (x + w < mx) return 0;
	if (x > mx) return 0;
	if (y + h < my) return 0;
	if (y > my) return 0;

	return 1;
}

int handle_Click(Skeet *s, int x, int y)
{
	for (int i = 0; i < s->n_skeets; i++)
	{
		if (s->active[i] == 0) continue;

		if (collision(s->pos[i].x, s->pos[i].y, 
			s->size[i], s->size[i], x, y) == 0) continue;
	
		//first one gets modified
		float new_size = s->size[i] * 0.8;
		float explosion_x = x;
		float explosion_y = y;
		float skeet_center_x = s->pos[i].x + 0.5*s->size[i];
		float skeet_center_y = s->pos[i].y + 0.5*s->size[i];
		
		//explosion vector
		float ex = skeet_center_x - explosion_x;
		float ey = skeet_center_y - explosion_y;
		//normalize for unit vector
		float mag = sqrt(ex*ex + ey * ey);
		ex /= mag;
		ey /= mag;

		s->active[i] = 0;

		for (int j = 0; j < 4; j++)
		{
			int si = create(s);//give me next available
			if (si == -1) break;

			s->pos[si].x = explosion_x - 0.5*new_size;
			s->pos[si].y = explosion_y - 0.5*new_size;
			s->vel[si].x = 0.5*ex + 0.1*R();
			s->vel[si].y = 0.5*ey + 0.1*R();
			s->size[si] = new_size;
			s->active[si] = 1;
			s->accel[si] = { 0 };

		}

		return 1;
	}

	return 0;
}


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("conway's game of life", 50, 50, screen_w, screen_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(0));
	Skeet s;
	init(&s);

	create_Skeet(&s, screen_w / 2, screen_h*0.2, 100);

	unsigned int current_mouse_state = 0;
	unsigned int prev_mouse_state = 0;

	float hit_count = 0;
	float loss = 0;

	unsigned int game_start_time = SDL_GetTicks();

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;	
		}

		unsigned int current_time = SDL_GetTicks();

		if (current_time - game_start_time > 30000)
		{
			break;
		}

		int mx, my;
		prev_mouse_state = current_mouse_state;
		current_mouse_state = SDL_GetMouseState(&mx, &my);

		//detects left click
		if ((current_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) == 1 &&
			(prev_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) == 0)
		{
			int r = handle_Click(&s, mx, my);
			if (r == 1) hit_count += 1.0;
		}

		float g = 0.001;
		for (int i = 0; i < s.n_skeets; i++)
		{
			if (s.active[i] == 1)
			{
				s.accel[i].x = 0.0;
				s.accel[i].y = g;
			}
		}

		float dt = 0.1;
		update(&s, dt);

		for (int i = 0; i < s.n_skeets; i++)
		{
			if (s.active[i] == 1)
			{
				if (s.pos[i].x < 0 || s.pos[i].x+s.size[i] > screen_w)
				{
					s.vel[i].x *= -1;
				}
				if (s.pos[i].y > screen_h)
				{
					loss += s.size[i];
					s.active[i] = 0;
				}
			}
		}

		

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		draw(&s);

		SDL_RenderPresent(renderer);
	}


	printf("score: %.2f\n", 1000 * hit_count - loss);
	getchar();

	return 0;
}