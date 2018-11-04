#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

struct Pos
{
	float x;
	float y;
};
struct Size
{
	float w, h;
};

struct Speed
{
	float x, y;
};

struct RGB
{
	unsigned char r, g, b;
};

namespace Collision
{
	enum { NO_COLLISION = 0, TOP_OF_1, RIGHT_OF_1, BOTTOM_OF_1, LEFT_OF_1 };
	
	int minkowski(float x0, float y0, float w0, float h0, float x1, float y1, float w1, float h1)
	{
		float w = 0.5 * (w0 + w1);
		float h = 0.5 * (h0 + h1);
		float dx = x0 - x1 + 0.5*(w0 - w1);
		float dy = y0 - y1 + 0.5*(h0 - h1);

		if (dx*dx <= w * w && dy*dy <= h * h)
		{
			float wy = w * dy;
			float hx = h * dx;

			if (wy > hx)
			{
				return (wy + hx > 0) ? BOTTOM_OF_1 : LEFT_OF_1;
			}
			else
			{
				return (wy + hx > 0) ? RIGHT_OF_1 : TOP_OF_1;
			}
		}
		return NO_COLLISION;
	}
}

namespace Game
{
	SDL_Renderer *renderer = NULL;

	int screen_width;
	int screen_height;
	int n_bricks;

	Pos ball_pos;
	Speed ball_speed;
	Size ball_size;
	RGB ball_color;

	Pos *brick_pos;
	int *brick_state;
	Size brick_size;
	RGB brick_color;

	void init()
	{
		//initialize
		n_bricks = 100;
		screen_width = 800;
		screen_height = 600;

		ball_pos.x = screen_width / 2;
		ball_pos.y = screen_height / 2;
		ball_size.w = 8;
		ball_size.h = 8;
		ball_speed.x = 0.1;
		ball_speed.y = 0.1;
		ball_color.r = 0;
		ball_color.g = 120;
		ball_color.b = 255;

		brick_size.w = 64;
		brick_size.h = 16;
		brick_color.r = 180;
		brick_color.g = 80;
		brick_color.b = 0;

		brick_pos = new Pos[n_bricks];
		brick_state = new int[n_bricks];
		for (int i = 0; i < n_bricks; i++)
		{
			brick_pos[i].x = rand() % screen_width;
			brick_pos[i].y = rand() % screen_height;
			brick_state[i] = 1;
		}
	}

	void update()
	{
		ball_pos.x += ball_speed.x;
		ball_pos.y += ball_speed.y;

		if (ball_pos.x < 0 || ball_pos.x + ball_size.w > screen_width)
		{
			ball_speed.x *= -1;
		}
		if (ball_pos.y < 0 || ball_pos.y + ball_size.h > screen_height)
		{
			ball_speed.y *= -1;
		}

		for (int i = 0; i < n_bricks; i++)
		{
			if (brick_state[i] == 0) continue;

			int result = Collision::minkowski(
				ball_pos.x, ball_pos.y, ball_size.w, ball_size.h,
				brick_pos[i].x, brick_pos[i].y, brick_size.w, brick_size.h);
			
			if (result == Collision::LEFT_OF_1 || result == Collision::RIGHT_OF_1)
			{
				ball_speed.x *= -1;
				brick_state[i] = 0;
			}
			else if (result == Collision::BOTTOM_OF_1 || result == Collision::TOP_OF_1)
			{
				ball_speed.y *= -1;
				brick_state[i] = 0;
			}
		}

	}

	void draw_Ball()
	{
		SDL_SetRenderDrawColor(renderer, ball_color.r, ball_color.g, ball_color.b, 255);
		SDL_Rect rect = { ball_pos.x,ball_pos.y,ball_size.w,ball_size.h };
		SDL_RenderFillRect(renderer, &rect);
	}

	void draw_Fancy_Ball()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_Rect rect0 = { ball_pos.x,ball_pos.y,ball_size.w,ball_size.h };
		SDL_RenderFillRect(renderer, &rect0);

		SDL_SetRenderDrawColor(renderer, ball_color.r, ball_color.g, ball_color.b, 255);
		SDL_Rect rect1 = { ball_pos.x+2,ball_pos.y+2,ball_size.w-4,ball_size.h-4 };
		SDL_RenderFillRect(renderer, &rect1);
	}

	void draw_Bricks()
	{
		SDL_SetRenderDrawColor(renderer, brick_color.r, brick_color.g, brick_color.b, 255);

		for (int i = 0; i < n_bricks; i++)
		{
			if (brick_state[i] == 0) continue;

			SDL_Rect rect = { brick_pos[i].x,brick_pos[i].y,brick_size.w,brick_size.h };
			SDL_RenderFillRect(renderer, &rect);
		}

	}

	void draw_Fancy_Bricks()
	{
		
		for (int i = 0; i < n_bricks; i++)
		{
			if (brick_state[i] == 0) continue;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Rect rect0 = { brick_pos[i].x,brick_pos[i].y,brick_size.w,brick_size.h };
			SDL_RenderFillRect(renderer, &rect0);

			SDL_SetRenderDrawColor(renderer, brick_color.r, brick_color.g, brick_color.b, 255);
			SDL_Rect rect1 = { brick_pos[i].x + 2,brick_pos[i].y + 2,brick_size.w - 4,brick_size.h - 4 };
			SDL_RenderFillRect(renderer, &rect1);
		}
	}
}
int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);

	Game::init();

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

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

		Game::update();
		
		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		Game::draw_Fancy_Ball();
		Game::draw_Fancy_Bricks();

		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}