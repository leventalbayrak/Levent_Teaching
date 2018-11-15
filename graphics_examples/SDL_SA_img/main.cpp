#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")


int collision(const SDL_Rect *a, const SDL_Rect *b)
{
	if (a->x + a->w < b->x) return 0;
	if (b->x + b->w < a->x) return 0;
	if (a->y + a->h < b->y) return 0;
	if (b->y + b->h < a->y) return 0;
	return 1;
}


namespace SA
{
	namespace internal
	{

		float evaluate_Solution_Fitness(SDL_Rect *boxes, int n_boxes)
		{
			float fitness = 0.0;
			for (int i = 0; i < n_boxes; i++)
			{
				for (int j = i + 1; j < n_boxes; j++)
				{
					int r = collision(&boxes[i], &boxes[j]);
					if (r == 1)
					{
						fitness -= 2;
					}

					if (boxes[i].x < 0 || boxes[i].y < 0) fitness -= 2;
				}
			}

			return fitness;
		}

		void modify_Solution(SDL_Rect *src, int n_boxes)
		{
			int which_box = rand() % n_boxes;
			src[which_box].x += 20.0*(1.0 - 2.0*rand() / RAND_MAX);
			src[which_box].y += 20.0*(1.0 - 2.0*rand() / RAND_MAX);
		}
	}

	void generate_Solution(SDL_Rect *dest, int n_boxes, int max_x, int max_y)
	{
		/*dest[0].x = 20;
		dest[0].y = 20;
		dest[0].w = 200;
		dest[0].h = 200;*/
		for (int i = 0; i < n_boxes; i++)
		{
			dest[i].w = 5 + rand() % 40;
			dest[i].h = 5 + rand() % 40;
			dest[i].x = rand() % (max_x - dest[i].w);
			dest[i].y = rand() % (max_y - dest[i].h);
		}
	}

	float update(SDL_Rect *current_solution, int n_boxes, SDL_Rect *tmp_solution, float temperature)
	{
		memcpy(tmp_solution, current_solution, sizeof(SDL_Rect)*n_boxes);

		internal::modify_Solution(tmp_solution, n_boxes);

		float fitness_current = internal::evaluate_Solution_Fitness(current_solution, n_boxes);
		float fitness_new = internal::evaluate_Solution_Fitness(tmp_solution, n_boxes);

		double p_accept_new_solution = exp((fitness_new - fitness_current) / temperature);
		double p = (double)rand() / RAND_MAX;

		if (p <= p_accept_new_solution)
		{
			memcpy(current_solution, tmp_solution, sizeof(SDL_Rect)*n_boxes);
			return fitness_new;
		}
		return fitness_current;
	}

}


namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;

	float temperature = 2;
	float temperature_decay = 0.98;

	const int n_boxes = 500;
	SDL_Rect boxes[n_boxes];
	SDL_Rect tmp_boxes[n_boxes];
	unsigned int color[n_boxes];

	unsigned char prev_key_state[256];
	unsigned char *keys = NULL;

	bool update_solver = true;

	void init()
	{
		SDL_Init(SDL_INIT_VIDEO);

		prev_key_state[256];
		keys = (unsigned char*)SDL_GetKeyboardState(NULL);

		SDL_Window *window = SDL_CreateWindow(
			"Fortnite",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

		Game::renderer = SDL_CreateRenderer(window,
			-1, SDL_RENDERER_ACCELERATED);

		SA::generate_Solution(boxes, n_boxes, screen_width, screen_height);

		for (int i = 0; i < n_boxes; i++)
		{
			color[i] = 0;
			color[i] = ((rand()%256)<<24) | ((rand() % 256) << 16) | ((rand() % 256) << 8);
		}
	}

	void update()
	{

		memcpy(Game::prev_key_state, Game::keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		if (update_solver)
		{
			float fitness = 0.0;
			for (int i = 0; i < 100; i++)
			{
				fitness = SA::update(boxes, n_boxes, tmp_boxes, temperature);
			}

			printf("fitness: %f temperature: %f\n", fitness, temperature);

			if (fitness >= 0.0)
			{
				update_solver = false;
			}

			temperature *= temperature_decay;
		}

	}

	void draw()
	{
		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		for (int i = 0; i < n_boxes; i++)
		{
			int r = (color[i] & 0xFF000000) >> 24;
			int g = (color[i] & 0x00FF0000) >> 16;
			int b = (color[i] & 0x0000FF00) >> 8;
			SDL_SetRenderDrawColor(Game::renderer, r, g, b, 255);
			SDL_RenderDrawRect(renderer, &boxes[i]);
		}

		//flip buffers
		SDL_RenderPresent(Game::renderer);
	}

}
int main(int argc, char **argv)
{
	

	Game::init();


	for(;;)
	{
		Game::update();
		
		Game::draw();
	}



	return 0;
}