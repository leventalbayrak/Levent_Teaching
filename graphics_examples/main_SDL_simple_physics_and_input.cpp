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

	void add_Force(Body *b, float fx, float fy)
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

	unsigned char *key_state = (unsigned char*)SDL_GetKeyboardState(NULL);
	static unsigned char last_key_state[256];
	memset(last_key_state, 0, 256);

	double dt = 1.0;
	Body::Body box;
	Body::init(&box);
	box.pos.x = screen_width / 2;
	box.pos.y = screen_height - 20;

	float f_gravity = 0.001;
	float f_move = 0.1;
	float f_jump = f_move * 4;//jump force 4 times stronger than move
	float floor_friction = 0.01;
	float wall_friction = 0.8;
	float max_horizontal_vel = 0.2;

	bool done = false;
	while (!done)
	{
		memcpy(last_key_state, key_state, 256);
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}
		key_state = (unsigned char*)SDL_GetKeyboardState(NULL);

		/*
		GAME CODE
		*/
		//SUM ALL FORCES
		
		box.force = {};

		//add gravity
		Body::add_Force(&box, 0.0, f_gravity);
		
		//allow user input only when the box is on the floor
		if (box.pos.y >= screen_height - 20)
		{
			if (key_state[SDL_SCANCODE_W] == 1 && last_key_state[SDL_SCANCODE_W] == 0)
			{
				Body::add_Force(&box, 0.0, -f_jump);
			}
			if (key_state[SDL_SCANCODE_A])
			{
				Body::add_Force(&box, -f_move, 0.0);
			}
			if (key_state[SDL_SCANCODE_D])
			{
				Body::add_Force(&box, f_move, 0.0);
			}
		}

		Body::update(&box, dt);

		//cap horizontal speed
		if (box.vel.x >= max_horizontal_vel)
		{
			box.vel.x = max_horizontal_vel;
		}
		//cap horizontal speed
		if (box.vel.x <= -max_horizontal_vel)
		{
			box.vel.x = -max_horizontal_vel;
		}

		//do not go thru the floor, set vertical speed to zero if touching the floor
		if (box.pos.y > screen_height - 20)
		{
			box.pos.y = screen_height - 20;
			box.vel.y = 0;
		}

		//apply floor friction only when the box is touching the bottom edge
		if (box.pos.y >= screen_height-20)
		{
			box.vel.x *= 1.0-floor_friction;
		}

		//dont go thru walls
		if (box.pos.x < 0)
		{
			box.vel.x *= -1;
			box.pos.x = 0;
		}
		//dont go thru walls
		if (box.pos.x >= screen_width - 20)
		{
			box.vel.x *= -1;
			box.pos.x = screen_width - 20;
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
