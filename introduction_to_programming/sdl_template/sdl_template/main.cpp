//SDL header
#include "SDL2-2.0.9/include/SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("Tetris Effect VR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float x = 50;
	float y = 50;
	float speed_x = 0.2;
	float speed_y = 0.1;

	//infinite loop
	for (;;)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				//terminate the process
				exit(0);
			}
		}

		//GAME CODE BEGIN

		//UPDATE CODE
		x += speed_x;
		y += speed_y;

		if (x < 0 || x + 40 > 800)
		{
			speed_x *= -1;
		}

		if (y < 0 || y + 40 > 600)
		{
			speed_y *= -1;
		}


		//DRAWING CODE
		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		SDL_RenderClear(renderer);

		SDL_Rect rectangle;
		rectangle.x = x;
		rectangle.y = y;
		rectangle.w = 40;
		rectangle.h = 40;

		SDL_SetRenderDrawColor(renderer, 255, 0, 50, 255);
		SDL_RenderFillRect(renderer, &rectangle);


		//swap buffers
		SDL_RenderPresent(renderer);
		//GAME CODE END
	}










	return 0;
}

