#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

struct Vec2D
{
	float x, y;
};

namespace Body
{
	struct Body
	{
		Vec2D force;
		Vec2D pos;
		Vec2D vel;
		float mass;
	};

	void init(Body *b)
	{
		*b = {};
		b->mass = 1.0;
	}

	void add_Force(Body *b,float fx, float fy)
	{
		b->force.x += fx;
		b->force.y += fy;
	}

	void update(Body *b, float dt)
	{
		Vec2D a;
		a.x = b->force.x / b->mass;
		a.y = b->force.y / b->mass;
		b->vel.x += a.x * dt;
		b->vel.y += a.y * dt;
		b->pos.x += b->vel.x * dt;
		b->pos.y += b->vel.y * dt;
	}
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_ShowCursor(SDL_DISABLE);

	int screen_width = 800;
	int screen_height = 600;

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	double dt = 1.0;
	Body::Body box;
	Body::init(&box);
	box.pos.x = screen_width / 2;
	box.pos.y = screen_height - 20;

	float f_gravity = 0.00001;
	float f_poke = 0.0001;

	bool done = false;
	while (!done)
	{
		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}
		unsigned char *keys = (unsigned char*) SDL_GetKeyboardState(NULL);

		/*
		GAME CODE
		*/
		//SUM ALL FORCES
		//add gravity
		box.force = {};
		Body::add_Force(&box, 0.0, f_gravity);
		if (keys[SDL_SCANCODE_W])
		{
			Body::add_Force(&box, 0.0, -f_poke);
		}
		if (keys[SDL_SCANCODE_S])
		{
			Body::add_Force(&box, 0.0, f_poke);
		}
		if (keys[SDL_SCANCODE_A])
		{
			Body::add_Force(&box, -f_poke, 0.0);
		}
		if (keys[SDL_SCANCODE_D])
		{
			Body::add_Force(&box, f_poke, 0.0);
		}

		Body::update(&box, dt);

		box.vel.x *= 0.999;

		if (box.pos.x < 0 || box.pos.x >= screen_width - 20)
		{
			box.vel.x *= -1;
		}
		if (box.pos.y < 0 || box.pos.y >= screen_height - 20)
		{
			box.vel.y *= -1;
		}

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		SDL_Rect rect;
		rect.x = box.pos.x;
		rect.y = box.pos.y;
		rect.w = 20;
		rect.h = 20;

		//draw filled rectangle in the backbuffer
		SDL_RenderFillRect(renderer, &rect);
	

		//flip buffers
		SDL_RenderPresent(renderer);

	}



	return 0;
}