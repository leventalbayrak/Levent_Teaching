
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("james", 50, 50, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load("images/werewolf.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	int current_frame = 0;
	unsigned int last_time_frame_changed = 0;

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;
		}

		unsigned int current_time = SDL_GetTicks();

		/*
		update	
		*/

		if (current_time - last_time_frame_changed > 66)
		{

			last_time_frame_changed = current_time;

			current_frame++;
			if (current_frame >= 4) current_frame = 0;
		}

		/*
		draw
		*/
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderClear(renderer);

		int x_of_werewolf_frame = current_frame * 32;
		SDL_Rect src = { x_of_werewolf_frame,0,32,32 };
		SDL_Rect dest = { 50,50,64,64 };
		SDL_RenderCopyEx(renderer, texture, &src, &dest,0,NULL, SDL_FLIP_NONE);

		SDL_RenderPresent(renderer);
	}

	return 0;
}