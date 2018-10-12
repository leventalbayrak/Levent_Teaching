//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")
#pragma comment(linker,"/subsystem:console")

#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include "SDL2-2.0.8\include\SDL_mixer.h"

#include "Engine_core.h"


int main(int argc, char **argv)
{
	//initialize all systems and open game window
	Engine::init("hello tiny2D", 960, 768);

	Tileset::Tileset tileset;
	Tileset::init(&tileset, 10);
	Tileset::modify(Tileset::make(&tileset), &tileset, "tileset.png", 32, 32, Engine::renderer);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	Grid::Grid level;
	Grid::init(&level, 512, 512);

	Grid::Grid objects;
	Grid::init(&objects, 512, 512);

	for (int i = 0; i < level.n_cols*level.n_rows; i++)
	{
		level.data[i] = rand() % 5;
		objects.data[i] = 0;
	}

	for (int i = 0; i < 100000; i++)
	{
		int n = (rand() % 512)*objects.n_cols + (rand() % 512);
		if (objects.data[n] < 0)
		{
			objects.data[n] = 5 + rand() % 3;
		}
	}

	SDL_Surface *sprite_surface = IMG_Load("saitama.png");
	assert(sprite_surface);
	SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(Engine::renderer, sprite_surface);
	SDL_FreeSurface(sprite_surface);

	Sprite::Data sprite_database;
	Sprite::init(&sprite_database, 1000);

	Sprite::Data_Args entry;
	entry.frame_w = 32;
	entry.frame_h = 32;

	entry.frame_pos_x = 0;
	entry.frame_pos_y = 0;
	entry.n_frames = 3;
	Sprite::modify(Sprite::make(&sprite_database), &sprite_database, &entry, sprite_texture);

	entry.frame_pos_x = 0;
	entry.frame_pos_y = 32;
	entry.n_frames = 3;
	Sprite::modify(Sprite::make(&sprite_database), &sprite_database, &entry, sprite_texture);

	entry.frame_pos_x = 0;
	entry.frame_pos_y = 64;
	entry.n_frames = 2;
	Sprite::modify(Sprite::make(&sprite_database), &sprite_database, &entry, sprite_texture);
	
	Sprite::Animation sprites;
	Sprite::init(&sprites, 100000);

	Body::Body bodies;
	Body::init(&bodies, 100000);
	
	for (int i = 0; i < 100000; i++)
	{
		int sprite_db_index = rand() % 3;
		int sprite_idx = Sprite::make(&sprites);
		Sprite::modify(sprite_idx, &sprites, &sprite_database, sprite_db_index, 60 + rand() % 600);
		int body_idx = Body::make(&bodies);
	}

	Grid_Camera::Grid_Camera camera;
	Grid_Camera::init(&camera, Engine::screen_width, Engine::screen_height);
	camera.canvas.x = 256;
	camera.canvas.y = 256;
	camera.canvas.w = Engine::screen_width / 64;
	camera.canvas.h = Engine::screen_height / 64;

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

		if (keys[SDL_SCANCODE_W]) cmd_UP = 1;
		if (keys[SDL_SCANCODE_S]) cmd_DOWN = 1;
		if (keys[SDL_SCANCODE_A]) cmd_LEFT = 1;
		if (keys[SDL_SCANCODE_D]) cmd_RIGHT = 1;
		
		/*
		GAME CODE
		*/
	
		if (cmd_LEFT)
		{
			camera.canvas.x += -camera_move_speed * Engine::screen_width;
		}
		if (cmd_RIGHT)
		{
			camera.canvas.x += camera_move_speed * Engine::screen_width;
		}
		if (cmd_UP)
		{
			camera.canvas.y += -camera_move_speed * Engine::screen_height;
		}
		if (cmd_DOWN)
		{
			camera.canvas.y += camera_move_speed * Engine::screen_height;
		}


		Body::clear_Forces(&bodies);
		Body::update(&bodies);

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Engine::renderer);

		Grid::Region grid_region;
		//if the camera was on top of a grid, which cells would its canvas be covering
		Grid_Camera::get_Grid_Region_Covered_by_Canvas(&grid_region, &camera);
		//based on the area covered, recalculate tile size and position
		Grid_Camera::calibrate_Tiles(&camera, &grid_region);

		int ty = camera.read_only.tile_y;
		for (int i = grid_region.y0; i <= grid_region.y1; i++)
		{
			int tx = camera.read_only.tile_x;

			int *tmp_level_data = &level.data[i*level.n_cols];
			int *tmp_object_data = &objects.data[i*objects.n_cols];
			for (int j = grid_region.x0; j <= grid_region.x1; j++)
			{
				int grid_data = tmp_level_data[j];
				Tileset::draw(0, tx, ty, camera.read_only.tile_w, camera.read_only.tile_h, grid_data, 0, &tileset, Engine::renderer);
				
				int object_data = tmp_object_data[j];
				if (object_data != 0)
				{
					Tileset::draw(0, tx, ty, camera.read_only.tile_w, camera.read_only.tile_h, 0, object_data, &tileset, Engine::renderer);
				}
				tx += camera.read_only.tile_w;
			}
			ty += camera.read_only.tile_h;
		}

		for (int i = 0; i < bodies.n_bodies; i++)
		{
			Sprite::update(i, &sprites, current_time);
			Sprite::draw(i, &sprites, bodies.pos[i].x, bodies.pos[i].y, camera.read_only.tile_w, camera.read_only.tile_h, Engine::renderer);
		}


		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}



	return 0;
}
