#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2_image-2.0.3\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

#include <iostream>
#include <fstream>
#include "Table_File_core.h"
using namespace std;


int main(int argc, char **argv)
{
	//SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_ShowCursor(SDL_DISABLE);

	int screen_width = 32 * 32;
	int screen_height = 32 * 16;

	SDL_Window *window = SDL_CreateWindow(
		"tilemap",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load("run.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	int first_frame_pos_x = 0;
	int first_frame_pos_y = 0;
	int frame_w = 108;
	int frame_h = 140;
	int n_frames = 8;
	int frame_duration = 100;
	unsigned int last_frame_change_time = 0;
	int current_frame = 0;
	
	bool done = false;

	while (!done)
	{
		unsigned int current_time = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		if (current_time - last_frame_change_time >= frame_duration)
		{
			current_frame = (current_frame + 1) % n_frames;
			last_frame_change_time = current_time;
		}

		//DRAW
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect src;
		src.x = first_frame_pos_x + current_frame * frame_w;
		src.y = first_frame_pos_y;
		src.w = frame_w;
		src.h = frame_h;

		SDL_Rect dest;
		dest.x = screen_width / 2;
		dest.y = screen_height / 2;
		dest.w = frame_w;
		dest.h = frame_h;
		
		SDL_RenderCopyEx(renderer, texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

		SDL_RenderPresent(renderer);
	}

	return 0;

}