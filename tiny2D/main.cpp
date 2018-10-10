#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include "SDL2-2.0.8\include\SDL_mixer.h"

#include "Tilemap_core.h"
#include "Grid_core.h"
#include "Grid_Camera_core.h"
#include "Sprite_core.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")

#pragma comment(linker,"/subsystem:console")

namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 640;
	int screen_height = 480;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(0));

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	Tileset::Tileset tileset;
	Tileset::load(&tileset, "tileset.png", 32, 32, Game::renderer);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	Grid::Grid level;
	Grid::init(&level, 512, 512);

	Grid::Grid objects;
	Grid::init(&objects, 512, 512);

	for (int i = 0; i < level.n_cols*level.n_rows; i++)
	{
		level.data[i] = rand() % 20;
		objects.data[i] = -1;
	}

	SDL_Surface *sprite_surface = IMG_Load("saitama.png");
	assert(sprite_surface);
	SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(Game::renderer, sprite_surface);
	SDL_FreeSurface(sprite_surface);

	Sprite::Data sprite_database;
	Sprite::init(&sprite_database, 1000);

	Sprite::Data_Args entry;
	entry.frame_w = 32;
	entry.frame_h = 32;

	entry.frame_pos_x = 0;
	entry.frame_pos_y = 0;
	entry.n_frames = 3;
	Sprite::add(&sprite_database, &entry, sprite_texture);

	entry.frame_pos_x = 0;
	entry.frame_pos_y = 32;
	entry.n_frames = 3;
	Sprite::add(&sprite_database, &entry, sprite_texture);

	entry.frame_pos_x = 0;
	entry.frame_pos_y = 64;
	entry.n_frames = 2;
	Sprite::add(&sprite_database, &entry, sprite_texture);
	
	
	Sprite::Animation sprites;
	Sprite::init(&sprites, 20000);
	
	for (int i = 0; i < 20000; i++)
	{
		int n = (rand() % 512)*objects.n_cols + (rand() % 512);
		if (objects.data[n] < 0)
		{
			objects.data[n] = sprites.n_animations;
			int sprite_db_index = rand() % 3;
			Sprite::add(&sprites, &sprite_database, sprite_db_index, 60 + rand() % 60);
		}
	}

	printf("%d\n", sprites.n_animations);

	Grid_Camera::Grid_Camera camera;
	Grid_Camera::init(&camera, Game::screen_width, Game::screen_height);
	Grid_Camera::set_Viewport(&camera, 256, 256, Game::screen_width / 64, Game::screen_height / 64);

	float camera_move_speed = 0.00001;
	float camera_zoom_speed = 0.0001;
	
	bool done = false;
	while (!done)
	{
		unsigned int current_time = SDL_GetTicks();

		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		int cmd_UP = 0;
		int cmd_LEFT = 0;
		int cmd_RIGHT = 0;
		int cmd_DOWN = 0;
		int cmd_ZOOMIN = 0;
		int cmd_ZOOMOUT = 0;

		if (keys[SDL_SCANCODE_W]) cmd_UP = 1;
		if (keys[SDL_SCANCODE_S]) cmd_DOWN = 1;
		if (keys[SDL_SCANCODE_A]) cmd_LEFT = 1;
		if (keys[SDL_SCANCODE_D]) cmd_RIGHT = 1;
		if (keys[SDL_SCANCODE_R]) cmd_ZOOMOUT = 1;
		if (keys[SDL_SCANCODE_T]) cmd_ZOOMIN = 1;

		/*
		GAME CODE
		*/
		if (cmd_ZOOMIN)
		{
			camera.w += -2.0*camera_zoom_speed * Game::screen_width;
			camera.h += -2.0*camera_zoom_speed * Game::screen_height;
			camera.x += camera_zoom_speed * Game::screen_width;
			camera.y += camera_zoom_speed * Game::screen_height;
		}
		if (cmd_ZOOMOUT)
		{
			camera.w += 2.0*camera_zoom_speed * Game::screen_width;
			camera.h += 2.0*camera_zoom_speed * Game::screen_height;
			camera.x += camera_zoom_speed * Game::screen_width;
			camera.y += camera_zoom_speed * Game::screen_height;

			camera_zoom_speed *= 1.1;
		}

		if (cmd_LEFT)
		{
			camera.x += -camera_move_speed * Game::screen_width;
		}
		if (cmd_RIGHT)
		{
			camera.x += camera_move_speed * Game::screen_width;
		}
		if (cmd_UP)
		{
			camera.y += -camera_move_speed * Game::screen_height;
		}
		if (cmd_DOWN)
		{
			camera.y += camera_move_speed * Game::screen_height;
		}

		//printf("camera %f %f\n", camera.x, camera.y);

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		Grid::Active_Index_Range_Out active_grid_range;
		Grid_Camera::recalculate_Tile_Position_and_Size(&active_grid_range, &camera);

		int ty = camera.read_only.tile_y;
		for (int i = active_grid_range.y0; i <= active_grid_range.y1; i++)
		{
			int tx = camera.read_only.tile_x;

			int *tmp_level_data = &level.data[i*level.n_cols];
			int *tmp_object_data = &objects.data[i*objects.n_cols];
			for (int j = active_grid_range.x0; j <= active_grid_range.x1; j++)
			{
				int grid_data = tmp_level_data[j];

				Tileset::draw(tx, ty, camera.read_only.tile_w, camera.read_only.tile_h, grid_data, 0, &tileset, Game::renderer);

				int object_data = tmp_object_data[j];
				if (object_data >= 0)
				{
					Sprite::update(&sprites, object_data, &sprite_database, current_time);
					Sprite::draw(tx, ty, camera.read_only.tile_w, camera.read_only.tile_h, object_data, &sprites, &sprite_database, Game::renderer);
				}
				tx += camera.read_only.tile_w;
			}
			ty += camera.read_only.tile_h;
		}


		//printf("%f %f %f %f\n", (float)active_range.x1, (float)active_range.x1, camera.x, camera.y);

	//	float w = (float)Game::screen_width / (1 + active_range.x1 - active_range.x0);
	//	float h = (float)Game::screen_height / (1 + active_range.y1 - active_range.y0);
	//	Tileset::draw(Game::screen_width / 2, Game::screen_height / 2, w, h, 1.0, 12, 0, &tileset, Game::renderer);

		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}
