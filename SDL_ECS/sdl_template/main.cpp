#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//SDL header
#include "SDL2-2.0.9/include/SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")

#include "ECS.h"
#include "System_Render_Color.h"
#include "System_Motion.h"
#include "system_Bounce_off_walls.h"
#include "System_Pawn_Input.h"

SDL_Renderer *renderer = NULL;

int spawn_Player(ECS* e)
{
	int playerid = create_Entity(e);

	add_Component(playerid, _Position_Component, e);
	e->pos.px[playerid] = 50.0f;
	e->pos.py[playerid] = 50.0f;

	add_Component(playerid, _Color_Component, e);
	e->color.r[playerid] = 50;
	e->color.g[playerid] = 50;
	e->color.b[playerid] = 50;

	add_Component(playerid, _Size_Component, e);
	e->size.w[playerid] = 50;
	e->size.h[playerid] = 50;

	add_Component(playerid, _Motion_Component, e);
	e->motion.ax[playerid] = 0.0f;
	e->motion.ay[playerid] = 0.0f;
	e->motion.vx[playerid] = 0.0f;
	e->motion.vy[playerid] = 0.0f;

	add_Tag(playerid, _Pawn_Input_Tag, e);

	return playerid;
}

int spawn_Enemy(ECS* e)
{
	int enemyid = create_Entity(e);

	add_Component(enemyid, _Position_Component, e);
	e->pos.px[enemyid] = rand() % 800;
	e->pos.py[enemyid] = rand() % 600;

	add_Component(enemyid, _Color_Component, e);
	e->color.r[enemyid] = rand() % 255;
	e->color.g[enemyid] = rand() % 255;
	e->color.b[enemyid] = rand() % 255;

	add_Component(enemyid, _Size_Component, e);
	e->size.w[enemyid] = 4;
	e->size.h[enemyid] = 4;

	add_Component(enemyid, _Motion_Component, e);
	e->motion.ax[enemyid] = 0;
	e->motion.ay[enemyid] = 0;
	e->motion.vx[enemyid] = 1.0f - 2.0f * rand() / RAND_MAX;
	e->motion.vy[enemyid] = 1.0f - 2.0f * rand() / RAND_MAX;

	add_Tag(enemyid, _Bounce_Off_Walls_Tag, e);

	return enemyid;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	srand(time(0));

	int screen_width = 800;
	int screen_height = 600;
	SDL_Window *window = SDL_CreateWindow("xXxKILLERJAPANESEWASPZxXx", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	ECS ecs;
	init(&ecs, 1000);

	int playerid = spawn_Player(&ecs);
	
	for (int i = 0; i < 500; ++i)
	{
		int enemyid = spawn_Enemy(&ecs);
	}
	
	//infinite loop
	for (;;)
	{
		int inputvertical = 0;
		int inputhorizontal = 0;
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				//terminate the process
				exit(0);
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_1)
				{
					for (int i = 0; i < ecs.maxnentities; ++i)
					{
						if (ecs.component_blueprint[i][_Motion_Component])
						{
							remove_Component(i, _Motion_Component, &ecs);
						}
					}
				}
				if (event.key.keysym.sym == SDLK_2)
				{
					for (int i = 0; i < ecs.maxnentities; ++i)
					{
						if (ecs.component_blueprint[i][_Motion_Component] == 0)
						{
							add_Component(i, _Motion_Component, &ecs);
						}
					}
				}
				if (event.key.keysym.sym == SDLK_3)
				{
					for (int i = 0; i < ecs.maxnentities; ++i)
					{
						if (ecs.tag_blueprint[i][_Pawn_Input_Tag] == 0)
						{
							add_Tag(i, _Pawn_Input_Tag, &ecs);
							break;
						}
					}
				}

				if (event.key.keysym.sym == SDLK_w)
				{
					inputvertical = -1;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					inputvertical = 1;
				}
				if (event.key.keysym.sym == SDLK_a)
				{
					inputhorizontal = -1;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					inputhorizontal = 1;
				}
			}
		}

		unsigned int current_time = SDL_GetTicks();

		system_Pawn_Input(&ecs, inputhorizontal, inputvertical, 2.0f);
		system_Bounce_Off_Walls(&ecs, 800, 600);
		system_Motion(&ecs, 1.0f);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
			
		system_Render_Color(&ecs, renderer);

		SDL_RenderPresent(renderer);

	}

	return 0;
}