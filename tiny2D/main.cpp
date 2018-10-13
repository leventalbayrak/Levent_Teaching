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
#include "Texture_core.h"

int main(int argc, char **argv)
{
	//initialize all systems and open game window
	Engine::init("hello ToxicToddler2D", 960, 768);

	Tileset::Tileset tileset;
	Tileset::init(&tileset, 10);
	Tileset::modify(Tileset::make(&tileset), &tileset, "tileset.png", 32, 32, Engine::renderer);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	Grid::Grid level;
	Grid::load(&level,"map.txt");

	SDL_Texture *sprite_texture = Texture::load("saitama.png", Engine::renderer);

	Sprite::Data sprite_database;
	Sprite::init(&sprite_database, 10);
	Body::Body bodies;
	Body::init(&bodies, 100);

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

	Grid_Camera::Grid_Camera camera;
	Grid_Camera::init(&camera, Engine::screen_width, Engine::screen_height);
	camera.grid_coord.w = Engine::screen_width / 64;
	camera.grid_coord.h = Engine::screen_height / 64;

	int player_sprite = Sprite::make_Instance(0, &sprite_database);
	Sprite::modify(0, player_sprite, &sprite_database, 100);
	Shape::Rect player_grid_rect = { level.n_cols/2,level.n_rows/2,1,1 };//grid coords x,y,w,h
	
	int player_physics_body = Body::make(&bodies);
	Vec2D::Vec2D tmp_pos = { player_grid_rect.x,player_grid_rect.y };
	Body::modify(player_physics_body, &bodies, &tmp_pos, 1.0);

	float player_move_force_magnitude = 0.00001;

	Vec2D::Vec2D gravity = { 0,0.00001 };
	
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

		Body::clear_Forces(&bodies);
	
		if (cmd_LEFT)
		{
			Vec2D::Vec2D f = { -player_move_force_magnitude,0 };
			Body::add_Force(player_physics_body, &bodies, &f);
		}
		if (cmd_RIGHT)
		{
			Vec2D::Vec2D f = { player_move_force_magnitude,0 };
			Body::add_Force(player_physics_body, &bodies, &f);
		}
		if (cmd_UP)
		{
			Vec2D::Vec2D f = { 0, -player_move_force_magnitude };
			Body::add_Force(player_physics_body, &bodies, &f);
		}
		if (cmd_DOWN)
		{
			Vec2D::Vec2D f = { 0, player_move_force_magnitude };
			Body::add_Force(player_physics_body, &bodies, &f);
		}

		Body::add_Force(player_physics_body, &bodies, &gravity);

		Body::update(player_physics_body, &bodies);
		player_grid_rect.x = bodies.pos[player_physics_body].x;
		player_grid_rect.y = bodies.pos[player_physics_body].y;

		printf("player grid coord %f %f\n", player_grid_rect.x, player_grid_rect.y);

		camera.grid_coord.x = player_grid_rect.x - camera.grid_coord.w / 2;
		camera.grid_coord.y = player_grid_rect.y - camera.grid_coord.h / 2;
		Grid::Region grid_region;
		//if the camera was on top of a grid, which cells would its grid_coord be covering
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &camera);
		Grid::clip_Grid_Region(&grid_region, &level);
		//based on the area covered, recalculate tile size and position
		Grid_Camera::calibrate_Tiles(&camera, &grid_region);

		Grid::Region grid_under_player_rect;
		Grid::get_Region_Under_Shape(&grid_under_player_rect, &player_grid_rect);
		for (int i = grid_under_player_rect.y0; i < grid_under_player_rect.y1; i++)
		{
			for (int j = grid_under_player_rect.x0; j < grid_under_player_rect.x1; j++)
			{
				
			}
		}

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Engine::renderer);

		

		int ty = camera.read_only.tile_y;
		for (int i = grid_region.y0; i <= grid_region.y1; i++)
		{
			int tx = camera.read_only.tile_x;

			int *tmp_level_data = &level.data[i*level.n_cols];
			for (int j = grid_region.x0; j <= grid_region.x1; j++)
			{
				int grid_data = tmp_level_data[j];
				int tileset_idx = 0;
				Tileset::draw(tileset_idx, grid_data, 0, &tileset, tx, ty, camera.read_only.tile_w, camera.read_only.tile_h, Engine::renderer);
				
				tx += camera.read_only.tile_w;
			}
			ty += camera.read_only.tile_h;
		}

		Shape::Rect player_screen_rect;
		Grid_Camera::grid_Coord_to_Screen_Coord(&player_screen_rect, &player_grid_rect, &camera);
		Sprite::update(0, player_sprite, &sprite_database, current_time);
		Sprite::draw(0, player_sprite, &sprite_database, player_screen_rect.x,player_screen_rect.y,player_screen_rect.w,player_screen_rect.h , Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}



	return 0;
}
