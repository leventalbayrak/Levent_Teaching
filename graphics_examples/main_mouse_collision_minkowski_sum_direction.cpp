

#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

struct Box_Pos
{
	float x, y;
};
struct Box_Size
{
	float w, h;
};
struct Box_Speed
{
	float sx, sy;
};

int collision1(const Box_Pos *pa, const Box_Size *sa, const Box_Pos *pb, const Box_Size *sb)
{
	if (pa->x + sa->w < pb->x) return 0;
	if (pb->x + sb->w < pa->x) return 0;
	if (pa->y + sa->h < pb->y) return 0;
	if (pb->y + sb->h < pa->y) return 0;
	return 1;
}

//minkowski sum
//if collision is a rare event and direction is important, use this
int collision2(const Box_Pos *pa, const Box_Size *sa, const Box_Pos *pb, const Box_Size *sb)
{
	enum { NO_COLLISION = 0, TOP_OF_B, RIGHT_OF_B, BOTTOM_OF_B, LEFT_OF_B};

	float w = 0.5 * (sa->w + sb->w);
	float h = 0.5 * (sa->h + sb->h);
	float dx = pa->x - pb->x + 0.5*(sa->w - sb->w);
	float dy = pa->y - pb->y + 0.5*(sa->h - sb->h);

	if (dx*dx <= w * w && dy*dy <= h * h)
	{
		float wy = w * dy;
		float hx = h * dx;

		if (wy > hx)
		{
			return (wy + hx > 0) ? BOTTOM_OF_B : LEFT_OF_B;
		}
		else
		{
			return (wy + hx > 0) ? RIGHT_OF_B : TOP_OF_B;
		}
	}
	return NO_COLLISION;
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

	Box_Pos pos = { 200,200 };
	Box_Size size = { 50,50 };

	Box_Pos mouse_pos;
	Box_Size mouse_size = { 16,16 };

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

		/*
		GAME CODE
		*/
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		mouse_pos.x = mx - mouse_size.w / 2;
		mouse_pos.y = my - mouse_size.h / 2;

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(renderer);

		//set color to red
		int d = collision2(&mouse_pos, &mouse_size, &pos, &size);
		if (d == 1)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			cout << "TOP" << endl;
		}
		else if (d == 2)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			cout << "RIGHT" << endl;
		}
		else if (d == 3)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			cout << "BOTTOM" << endl;
		}
		else if (d == 4)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			cout << "LEFT" << endl;
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		SDL_Rect rect;
		rect.x = pos.x;
		rect.y = pos.y;
		rect.w = size.w;
		rect.h = size.h;

		//draw filled rectangle in the backbuffer
		SDL_RenderFillRect(renderer, &rect);


		rect.x = mouse_pos.x;
		rect.y = mouse_pos.y;
		rect.w = mouse_size.w;
		rect.h = mouse_size.h;

		//draw filled rectangle in the backbuffer
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		SDL_RenderFillRect(renderer, &rect);


		//flip buffers
		SDL_RenderPresent(renderer);

	}



	return 0;
}
