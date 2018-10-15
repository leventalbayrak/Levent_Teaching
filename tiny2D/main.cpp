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
#include "Shape_core.h"

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
	Grid::load(&level, "test.tsv");
	//Grid::load(&level,"test.csv");

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

	int player_state = 0;

	Grid_Camera::Grid_Camera camera;
	Grid_Camera::init(&camera, Engine::screen_width, Engine::screen_height);
	camera.grid_coord.w = Engine::screen_width / 32;
	camera.grid_coord.h = Engine::screen_height / 32;

	Sprite::make_Instance(0, &sprite_database);
	Sprite::modify(0, 0, &sprite_database, 100);
	Sprite::make_Instance(1, &sprite_database);
	Sprite::modify(1, 0, &sprite_database, 100);
	Shape::Rect player_grid_rect = { level.n_cols/2,level.n_rows/2,1.00,1.00 };//grid coords x,y,w,h
	//Shape::Rect player_grid_rect = { 1,1,1.0,1.0 };//grid coords x,y,w,h

	printf("map nrows %d ncols %d\n", level.n_rows, level.n_cols);
	//getchar();

	int player_physics_body = Body::make(&bodies);
	Vec2D::Vec2D tmp_pos = { player_grid_rect.x,player_grid_rect.y };
	Body::modify(player_physics_body, &bodies, &tmp_pos, 1.0);

	float player_move_force_magnitude = 0.00001;

	Vec2D::Vec2D gravity = { 0,0.00005 };

	int flip = 0;

	float max_vel_x = 1.0/32.0;
	float max_vel_y = 1.0/32.0;

	int player_collision_top = 0;
	int player_collision_bottom = 0;
	int player_collision_left = 0;
	int player_collision_right = 0;
	
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
		

		if (keys[SDL_SCANCODE_W] == 1 && prev_key_state[SDL_SCANCODE_W] == 0) cmd_UP = 1;
		if (keys[SDL_SCANCODE_S]) cmd_DOWN = 1;
		if (keys[SDL_SCANCODE_A]) cmd_LEFT = 1;
		if (keys[SDL_SCANCODE_D]) cmd_RIGHT = 1;
		
		/*
		GAME CODE
		*/

		if (cmd_LEFT) flip = 1;
		if (cmd_RIGHT) flip = 0;

		if (cmd_LEFT || cmd_RIGHT)
		{
			player_state = 1;
		}
		else
		{
			player_state = 0;
		}

		player_grid_rect.x = bodies.pos[player_physics_body].x;
		player_grid_rect.y = bodies.pos[player_physics_body].y;
		//printf("player grid coord %.2f %.2f\n", player_grid_rect.x, player_grid_rect.y);

		Shape::Rect player_collision_rect;
		player_collision_rect.x = player_grid_rect.x - max_vel_x;
		player_collision_rect.y = player_grid_rect.y - max_vel_y;
		player_collision_rect.w = player_grid_rect.w + 2.0*max_vel_x;
		player_collision_rect.h = player_grid_rect.h + 2.0*max_vel_y;

		//printf("player grid rect: %.2f %.2f %.2f %.2f\n", player_grid_rect.x, player_grid_rect.y, player_grid_rect.w, player_grid_rect.h);
		//printf("player col rect: %.2f %.2f %.2f %.2f\n", player_collision_rect.x, player_collision_rect.y, player_collision_rect.w, player_collision_rect.h);

		player_collision_left = 0;
		player_collision_right = 0;
		player_collision_top = 0;
		player_collision_bottom = 0;

		Grid::Region grid_under_player_rect;
		Grid::get_Region_Under_Shape(&grid_under_player_rect, &player_collision_rect);
		Grid::clip_Grid_Region(&grid_under_player_rect, &level);
		for (int i = grid_under_player_rect.y0; i <= grid_under_player_rect.y1; i++)
		{
			for (int j = grid_under_player_rect.x0; j <= grid_under_player_rect.x1; j++)
			{
				if (level.data[i*level.n_cols + j] == 0) continue;

				//Shape::Rect w = { j,i,1.0,1.0 };
				//int c = Shape::collision_with_Dir(&player_collision_rect, &w);

				if (player_collision_rect.x + player_collision_rect.w > j)
				{
					//player_collision_right = 1;
				}
				if (player_collision_rect.x > j + 1.0)
				{
					player_collision_left = 1;
				}
				if (player_collision_rect.y + player_collision_rect.h > i)
				{
					player_collision_bottom = 1;
				}
				if (player_collision_rect.y > i + 1.0)
				{
					player_collision_top = 1;
				}

			}
		}

		//printf("grid x0%d y0%d x1%d y1%d\n", grid_under_player_rect.x0, grid_under_player_rect.y0, grid_under_player_rect.x1, grid_under_player_rect.y1);

		//printf("left %d right %d top %d bottom %d\n", player_collision_left, player_collision_right, player_collision_top, player_collision_bottom);

		//START FORCES
		Body::clear_Forces(&bodies);

		if (cmd_RIGHT == 1)
		{
			if (player_collision_bottom == 1)
			{
				Vec2D::Vec2D f = { player_move_force_magnitude * 15, 0 };
				Body::add_Force(player_physics_body, &bodies, &f);
			}
			else
			{
				Vec2D::Vec2D f = { player_move_force_magnitude * 1, 0 };
				Body::add_Force(player_physics_body, &bodies, &f);
			}

		}
		if (cmd_LEFT == 1)
		{
			if (player_collision_bottom == 1)
			{
				Vec2D::Vec2D f = { -player_move_force_magnitude * 15, 0 };
				Body::add_Force(player_physics_body, &bodies, &f);
			}
			else
			{
				Vec2D::Vec2D f = { -player_move_force_magnitude * 1, 0 };
				Body::add_Force(player_physics_body, &bodies, &f);
			}
		}

		if (cmd_UP == 1 && player_collision_bottom == 1)
		{
			Vec2D::Vec2D f = { 0, -player_move_force_magnitude * 1200 };
			Body::add_Force(player_physics_body, &bodies, &f);
		}

		Body::add_Force(player_physics_body, &bodies, &gravity);

		//DONE FORCES

		//integrate acceleration
		bodies.vel[player_physics_body].x += bodies.force[player_physics_body].x / bodies.mass[player_physics_body];
		bodies.vel[player_physics_body].y += bodies.force[player_physics_body].y / bodies.mass[player_physics_body];
		//clip velocity
		if (bodies.vel[player_physics_body].x > max_vel_x)bodies.vel[player_physics_body].x = max_vel_x;
		if (bodies.vel[player_physics_body].x < -max_vel_x)bodies.vel[player_physics_body].x = -max_vel_x;
		if (bodies.vel[player_physics_body].y > max_vel_y)bodies.vel[player_physics_body].y = max_vel_y;
		if (bodies.vel[player_physics_body].y < -max_vel_y)bodies.vel[player_physics_body].y = -max_vel_y;
		
		if (player_collision_bottom == 1)
		{
			bodies.vel[player_physics_body].x *= 0.98;
			if (abs(bodies.vel[player_physics_body].x) < 0.00001) bodies.vel[player_physics_body].x = 0.0;
		}

		if (player_collision_bottom == 1 && bodies.vel[player_physics_body].y > 0)
		{
			bodies.vel[player_physics_body].y = 0;
		}
		if (player_collision_top == 1 && bodies.vel[player_physics_body].y < 0)
		{
			bodies.vel[player_physics_body].y = 0;
		}
		if (player_collision_right == 1 && bodies.vel[player_physics_body].x > 0)
		{
			bodies.vel[player_physics_body].x = 0;
		}
		if (player_collision_left == 1 && bodies.vel[player_physics_body].x < 0)
		{
			bodies.vel[player_physics_body].x = 0;
		}

		//integrate velocity
		bodies.pos[player_physics_body].x += bodies.vel[player_physics_body].x;
		bodies.pos[player_physics_body].y += bodies.vel[player_physics_body].y;

		camera.grid_coord.x = player_grid_rect.x - camera.grid_coord.w / 2;
		camera.grid_coord.y = player_grid_rect.y - camera.grid_coord.h / 2;
		Grid::Region grid_region;
		//if the camera was on top of a grid, which cells would its grid_coord be covering
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &camera);
		Grid::clip_Grid_Region(&grid_region, &level);
		//based on the area covered, recalculate tile size and position
		Grid_Camera::calibrate_Tiles(&camera, &grid_region);

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
				int tileset_idx = grid_data / 23;
				int tileset_offset = grid_data % 23;
				Tileset::draw(0, tileset_idx, tileset_offset, &tileset, tx, ty, camera.read_only.tile_w, camera.read_only.tile_h, Engine::renderer);

				tx += camera.read_only.tile_w;
			}
			ty += camera.read_only.tile_h;
		}

		Shape::Rect player_screen_rect;
		Grid_Camera::grid_Coord_to_Screen_Coord(&player_screen_rect, &player_grid_rect, &camera);
		
		if (player_state == 1)
		{
			Sprite::update(0, 0, &sprite_database, current_time);
			Sprite::draw(0, 0, &sprite_database, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, flip);
		}
		else
		{
			Sprite::update(1, 0, &sprite_database, current_time);
			Sprite::draw(1, 0, &sprite_database, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, flip);

		}

		int red = 0;
		int green = 0;
		for (int i = grid_under_player_rect.y0; i <= grid_under_player_rect.y1; i++)
		{
			for (int j = grid_under_player_rect.x0; j <= grid_under_player_rect.x1; j++)
			{
				if (level.data[i*level.n_cols + j] == 0) continue;
				Shape::Rect tile_affected_rect = { j, i, 1, 1 };
				Shape::Rect tile_collision_screen_rect;
				Grid_Camera::grid_Coord_to_Screen_Coord(&tile_collision_screen_rect, &tile_affected_rect, &camera);

				SDL_SetRenderDrawColor(Engine::renderer, red, green, 255, 255);
				SDL_Rect r2 = { tile_collision_screen_rect.x,tile_collision_screen_rect.y,tile_collision_screen_rect.w,tile_collision_screen_rect.h };
				SDL_RenderFillRect(Engine::renderer, &r2);

				red += j * 5;
				red %= 256;
				green += i * 7;
				green %= 256;
			}
		}

		SDL_SetRenderDrawColor(Engine::renderer, 255, 0, 0, 255);
		Shape::Rect player_collision_screen_rect;
		Grid_Camera::grid_Coord_to_Screen_Coord(&player_collision_screen_rect, &player_collision_rect, &camera);

		SDL_Rect r = { player_collision_screen_rect.x,player_collision_screen_rect.y,player_collision_screen_rect.w,player_collision_screen_rect.h };
		SDL_RenderDrawRect(Engine::renderer, &r);
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}



	return 0;
}
