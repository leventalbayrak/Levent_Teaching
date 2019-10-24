#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2-2.0.10/include/SDL.h"

#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2.lib")
#pragma comment(lib,"SDL2-2.0.10/lib/x86/SDL2main.lib")
#pragma comment(linker,"/subsystem:console")

#pragma warning(disable:4996)

typedef unsigned char uchar;

int screen_width = 800;
int screen_height = 600;

struct Box
{
	float x, y, w, h;
	uchar  r, g, b;
};

struct Player
{
	Box box;
	float dx, dy;//unit vector
	float speed;
};

struct Tree
{
	Box box;
};

struct Bullet
{
	Box box;
	float dx, dy;//unit vector
	float speed;
};

void setup_Forest_Scene(Tree* t, int ntrees)
{
	for (int i = 0; i < ntrees; ++i)
	{
		
		t[i].box.w = 20 + rand() % 60;
		t[i].box.h = 40 + rand() % 200;
		t[i].box.x = rand() % (int)(screen_width - t[i].box.w);
		t[i].box.y = 30 + rand() % (int)(screen_height - t[i].box.h);
		t[i].box.r = 50 + rand() % 100;
		t[i].box.g = 50 + rand() % 100;
		t[i].box.b = rand() % 7;
	}

}

void bubble_Sort_by_Y(Box* t, int nboxes)
{
	for (int i = 0; i < nboxes; ++i)
	{
		for (int j = i + 1; j < nboxes; ++j)
		{
			if (t[i].y + t[i].h > t[j].y + t[j].h)
			{
				Box tmp = t[i];
				t[i] = t[j];
				t[j] = tmp;
			}
		}
	}
}

const int ntrees = 30;
static Tree forest[ntrees] = { 0 };

const int nbullets = 200;
static Bullet bullets[nbullets] = { 0 };
static uchar bulletstate[nbullets] = { 0 };

Player player = { 0 };

static Box tmp_boxes[20000];

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	srand(time(0));

	int screen_width = 800;
	int screen_height = 600;
	SDL_Window* window = SDL_CreateWindow("LOOP", 100, 100, screen_width, screen_height, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	setup_Forest_Scene(forest, ntrees);

	player = { 0 };
	player.box.x = screen_width / 2;
	player.box.y = screen_height / 2;
	player.box.w = 40;
	player.box.h = 120;
	player.box.r = 255;
	player.box.g = 0;
	player.box.b = 0;
	player.speed = 0.2f;
	

	uchar* keys = (uchar*) SDL_GetKeyboardState(NULL);
	uchar prev_keys[256] = { 0 };

	for (;;)
	{
		SDL_memcpy(prev_keys, keys, 256);
		/*
		consume events
		*/
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				printf("GOODBYE\n");
				exit(0);
			}
		}

		

		player.dx = 0;
		player.dy = 0;
		if (keys[SDL_SCANCODE_W])
		{
			player.dy = -1;
		}
		if (keys[SDL_SCANCODE_S])
		{
			player.dy = 1;
		}
		if (keys[SDL_SCANCODE_A])
		{
			player.dx = -1;
		}
		if (keys[SDL_SCANCODE_D])
		{
			player.dx = 1;
		}
		

		if (keys[SDL_SCANCODE_SPACE] == 1 && prev_keys[SDL_SCANCODE_SPACE] == 0)
		{
			int mouse_x = 0;
			int mouse_y = 0;
			SDL_GetMouseState(&mouse_x, &mouse_y);
			float dx = mouse_x - player.box.x;
			float dy = mouse_y - player.box.y;
			float mag = sqrt(dx*dx + dy*dy);
			if (mag != 0.0f)
			{
				dx /= mag;
				dy /= mag;
			}
			for (int i = 0; i < nbullets; ++i)
			{

				if (bulletstate[i] == 0)
				{
					bulletstate[i] = 1;
					bullets[i].box.x = player.box.x;
					bullets[i].box.y = player.box.y;
					bullets[i].box.w = 30;
					bullets[i].box.h = 30;
					bullets[i].speed = 0.1;
					bullets[i].dx = dx;
					bullets[i].dy = dy;
					break;
				}


			}


		}

		float mag = sqrt(player.dx*player.dx + player.dy*player.dy);
		if (mag != 0.0f)
		{
			player.dx /= mag;
			player.dy /= mag;
		}

		player.box.x += player.dx * player.speed;
		player.box.y += player.dy * player.speed;

		for (int i = 0; i < nbullets; i++)
		{
			if (bulletstate[i] == 0) continue;
			bullets[i].box.x += bullets[i].dx * bullets[i].speed;
			bullets[i].box.y += bullets[i].dy * bullets[i].speed;
		}


		/*
		game code here
		*/
		int ntmpboxes = 0;
		for (int i = 0; i < ntrees; ++i) tmp_boxes[ntmpboxes++] = forest[i].box;
		for (int i = 0; i < nbullets; ++i) tmp_boxes[ntmpboxes++] = bullets[i].box;
		tmp_boxes[ntmpboxes++] = player.box;

		bubble_Sort_by_Y(tmp_boxes, ntmpboxes);
		/*
		draw
		clear screen once
		*/
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		for (int i = 0; i < ntmpboxes; ++i)
		{
			{
				SDL_Rect rectangle = { 0 };
				rectangle.x = tmp_boxes[i].x - 1;
				rectangle.y = tmp_boxes[i].y - 1;
				rectangle.w = tmp_boxes[i].w + 2;
				rectangle.h = tmp_boxes[i].h + 2;

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rectangle);
			}

			SDL_Rect rectangle = { 0 };
			rectangle.x = tmp_boxes[i].x;
			rectangle.y = tmp_boxes[i].y;
			rectangle.w = tmp_boxes[i].w;
			rectangle.h = tmp_boxes[i].h;

			SDL_SetRenderDrawColor(renderer, tmp_boxes[i].r, tmp_boxes[i].g, tmp_boxes[i].b, 255);
			SDL_RenderFillRect(renderer, &rectangle);
		}

		/*
		once per frame!
		*/
		SDL_RenderPresent(renderer);

	}








	return 0;
}