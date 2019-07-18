

/*
just in case you do not understand the rules: https://www.youtube.com/watch?v=FWSR_7kZuYg
http://www.conwaylife.com/wiki/Conway%27s_Game_of_Life
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
*/


#ifdef _WIN32
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
#else
#include <SDL2/SDL.h>
#endif

struct Vec2D
{
	float x, y;
};

namespace Jelly
{
	struct Jelly
	{
		int n_points;
		float point_size;//points are small circles
		//explicit verlett integration
		//does not use velocity
		Vec2D* pos_t;
		Vec2D* pos_t_minus_1;
		Vec2D* accel;
	};

	void init(Jelly* a, int n_points, float point_size)
	{
		a->n_points = n_points;
		a->point_size = point_size;

		a->pos_t = (Vec2D*)malloc(sizeof(Vec2D) * a->n_points);
		a->pos_t_minus_1 = (Vec2D*)malloc(sizeof(Vec2D) * a->n_points);
		a->accel = (Vec2D*)malloc(sizeof(Vec2D) * a->n_points);

		memset(a->pos_t, 0, sizeof(Vec2D) * a->n_points);
		memset(a->pos_t_minus_1, 0, sizeof(Vec2D) * a->n_points);
		memset(a->accel, 0, sizeof(Vec2D) * a->n_points);
	}

	void reset_Forces(Jelly* a)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			a->accel[i] = { 0 };
		}
	}

	//call after forces are done
	void accelerate(Jelly* a, float dt)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			//explicit verlett integration

			a->pos_t[i].x += a->accel[i].x * dt * dt;
			a->pos_t[i].y += a->accel[i].y * dt * dt;
			a->accel[i].x = 0;
			a->accel[i].y = 0;
		}
	}

	//call last after fixing is done
	void inertia(Jelly* a)
	{
		//printf("pos_t %f %f\n", a->pos_t[0].x, a->pos_t[0].y);
		for (int i = 0; i < a->n_points; i++)
		{
			//explicit verlett integration
			float new_pos_x = 2.0 * a->pos_t[i].x - a->pos_t_minus_1[i].x;
			float new_pos_y = 2.0 * a->pos_t[i].y - a->pos_t_minus_1[i].y;
			a->pos_t_minus_1[i] = a->pos_t[i];
			a->pos_t[i] = { new_pos_x,new_pos_y };
		}
	}

	//call after accelerate, before inertia, corrects positions
	void bounce_0(Jelly* a, Jelly* b, float alpha)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = 0; j < b->n_points; j++)
			{
				float dx = a->pos_t[i].x - b->pos_t[j].x;
				float dy = a->pos_t[i].y - b->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < a->point_size + b->point_size)
				{
					float factor = (length - a->point_size - b->point_size) / length;
					a->pos_t[i].x -= dx * factor * alpha;
					a->pos_t[i].y -= dy * factor * alpha;
					b->pos_t[j].x += dx * factor * (1.0 - alpha);
					b->pos_t[j].y += dy * factor * (1.0 - alpha);
				}
			}
		}
	}

	//call after inertia, handles deflection
	void bounce_1(Jelly* a, Jelly* b)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = 0; j < a->n_points; j++)
			{
				float dx = a->pos_t[i].x - b->pos_t[j].x;
				float dy = a->pos_t[i].y - b->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < a->point_size + b->point_size)
				{
					float v1x = a->pos_t[i].x - a->pos_t_minus_1[i].x;
					float v1y = a->pos_t[i].y - a->pos_t_minus_1[i].y;
					float v2x = b->pos_t[j].x - b->pos_t_minus_1[j].x;
					float v2y = b->pos_t[j].y - b->pos_t_minus_1[j].y;

					float factor = (length - a->point_size - b->point_size) / length;
					a->pos_t[i].x -= dx * factor * 0.5;
					a->pos_t[i].y -= dy * factor * 0.5;
					b->pos_t[j].x += dx * factor * 0.5;
					b->pos_t[j].y += dy * factor * 0.5;

					float damping = 0.95;
					float f1 = (damping * (dx * v1x + dy * v1y)) / slength;
					float f2 = (damping * (dx * v2x + dy * v2y)) / slength;

					v1x += f2 * dx - f1 * dx;
					v1y += f2 * dy - f1 * dy;

					v2x += f1 * dx - f2 * dx;
					v2y += f1 * dy - f2 * dy;

					a->pos_t_minus_1[i].x = a->pos_t[i].x - v1x;
					a->pos_t_minus_1[i].y = a->pos_t[i].y - v1y;
					b->pos_t_minus_1[j].x = b->pos_t[j].x - v2x;
					b->pos_t_minus_1[j].y = b->pos_t[j].y - v2y;

				}
			}
		}
	}

	//call after accelerate, before inertia, corrects positions
	void bounce_0_Self(Jelly* a)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = i + 1; j < a->n_points; j++)
			{
				float dx = a->pos_t[i].x - a->pos_t[j].x;
				float dy = a->pos_t[i].y - a->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < 2.0 * a->point_size)
				{
					float factor = (length - 2 * a->point_size) / length;
					a->pos_t[i].x -= dx * factor * 0.5;
					a->pos_t[i].y -= dy * factor * 0.5;
					a->pos_t[j].x += dx * factor * 0.5;
					a->pos_t[j].y += dy * factor * 0.5;
				}
			}
		}
	}

	//call after inertia, handles deflection
	void bounce_1_Self(Jelly* a)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = i + 1; j < a->n_points; j++)
			{
				float dx = a->pos_t[i].x - a->pos_t[j].x;
				float dy = a->pos_t[i].y - a->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < 2 * a->point_size)
				{
					float v1x = a->pos_t[i].x - a->pos_t_minus_1[i].x;
					float v1y = a->pos_t[i].y - a->pos_t_minus_1[i].y;
					float v2x = a->pos_t[j].x - a->pos_t_minus_1[j].x;
					float v2y = a->pos_t[j].y - a->pos_t_minus_1[j].y;

					float factor = (length - 2 * a->point_size) / length;
					a->pos_t[i].x -= dx * factor * 0.5;
					a->pos_t[i].y -= dy * factor * 0.5;
					a->pos_t[j].x += dx * factor * 0.5;
					a->pos_t[j].y += dy * factor * 0.5;

					float damping = 0.95;
					float f1 = (damping * (dx * v1x + dy * v1y)) / slength;
					float f2 = (damping * (dx * v2x + dy * v2y)) / slength;

					v1x += f2 * dx - f1 * dx;
					v1y += f2 * dy - f1 * dy;

					v2x += f1 * dx - f2 * dx;
					v2y += f1 * dy - f2 * dy;

					a->pos_t_minus_1[i].x = a->pos_t[i].x - v1x;
					a->pos_t_minus_1[i].y = a->pos_t[i].y - v1y;
					a->pos_t_minus_1[j].x = a->pos_t[j].x - v2x;
					a->pos_t_minus_1[j].y = a->pos_t[j].y - v2y;

				}
			}
		}
	}

	//call after accelerate, before inertia, corrects positions
	void bounce_0_Solid_B(Jelly* a, Jelly* b, float alpha)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = 0; j < b->n_points; j++)
			{
				float dx = a->pos_t[i].x - b->pos_t[j].x;
				float dy = a->pos_t[i].y - b->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < a->point_size + b->point_size)
				{
					float factor = (length - a->point_size - b->point_size) / length;
					a->pos_t[i].x -= dx * factor * alpha;
					a->pos_t[i].y -= dy * factor * alpha;
				}
			}
		}
	}

	//call after inertia, handles deflection
	void bounce_1_Solid_B(Jelly* a, Jelly* b)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = 0; j < a->n_points; j++)
			{
				float dx = a->pos_t[i].x - b->pos_t[j].x;
				float dy = a->pos_t[i].y - b->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < a->point_size + b->point_size)
				{
					float v1x = a->pos_t[i].x - a->pos_t_minus_1[i].x;
					float v1y = a->pos_t[i].y - a->pos_t_minus_1[i].y;

					float factor = (length - a->point_size - b->point_size) / length;
					a->pos_t[i].x -= dx * factor;
					a->pos_t[i].y -= dy * factor;

					float damping = 0.95;
					float f = (damping * (dx * v1x + dy * v1y)) / slength;
					v1x -= f * dx;
					v1y -= f * dy;

					a->pos_t_minus_1[i].x = a->pos_t[i].x - v1x;
					a->pos_t_minus_1[i].y = a->pos_t[i].y - v1y;
				}
			}
		}
	}


	int collision_Present(Jelly* a, Jelly* b, float alpha)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			for (int j = 0; j < b->n_points; j++)
			{
				float dx = a->pos_t[i].x - b->pos_t[j].x;
				float dy = a->pos_t[i].y - b->pos_t[j].y;
				float slength = dx * dx + dy * dy;
				float length = sqrt(slength);

				if (length < alpha * (a->point_size + b->point_size))
				{
					return 1;
				}
			}
		}
		return 0;
	}

	void draw(Jelly* a, Vec2D* offset, SDL_Texture* tex)
	{
		for (int i = 0; i < a->n_points; i++)
		{
			SDL_Rect dest = { a->pos_t[i].x - offset->x - a->point_size,a->pos_t[i].y - offset->y - a->point_size,2 * a->point_size,2 * a->point_size };
			SDL_RenderFillRect(renderer, &dest);

			//SDL_RenderCopy(renderer, tex, NULL, &dest);
		}
	}
}


SDL_Renderer *renderer = NULL;
int screen_w = 0;
int screen_h = 0;


int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);

	//FULL SCREEN
	SDL_DisplayMode displayMode;
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
	{
		screen_w = displayMode.w;
		screen_h = displayMode.h;
	}

	SDL_Window* window = SDL_CreateWindow("SDL Android Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Jelly::Jelly jelly;
	Jelly::init(&jelly, 100, 64);

	Jelly::Jelly floor;
	Jelly::init(&floor, 1000, 8);

	for (;;)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				exit(0);
			}
			else if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					screen_w = e.window.data1;
					screen_h = e.window.data2;
				}
			}
			else if (e.type == SDL_FINGERUP)
			{
				float mx = e.tfinger.x * screen_w;
				float my = e.tfinger.y * screen_h;

				
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				float mx = e.button.x;
				float my = e.button.y;
				
				
				
			}
		}

		int n_fingers = SDL_GetNumTouchFingers(0);
		const SDL_Finger *finger = SDL_GetTouchFinger(0, 0);
		if (finger != NULL)
		{
			
		}
		
		//RESET FORCES
		Jelly::reset_Forces(&jelly);

		float dt = 15.0 / 1000.0;
		Jelly::accelerate(&jelly, dt);
		Jelly::bounce_0_Solid_B(&jelly, &floor, 1.0);
		Jelly::bounce_0_Self(&jelly);
		Jelly::inertia(&jelly);
		Jelly::bounce_1_Solid_B(&jelly, &floor);
		Jelly::bounce_1_Self(&jelly);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		Vec2D camera_pos = { 0 };
		Jelly::draw(&jelly, &camera_pos, NULL);
		
		SDL_RenderPresent(renderer);


	}

	return 0;
}