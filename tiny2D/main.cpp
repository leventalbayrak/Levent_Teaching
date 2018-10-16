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

	//load tileset image
	Tileset::Tileset tileset;
	Tileset::init(&tileset, 10);
	Tileset::modify(Tileset::make(&tileset), &tileset, "tileset.png", 32, 32, 23, 21, Engine::renderer);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	//load collision map (level collisions), "tiled" export csv
	Grid::Grid collision_map;
	Grid::load(&collision_map, "test_collision.csv");
	//load tile map (visuals), "tiled" export csv
	Grid::Grid tile_map;
	Grid::load(&tile_map, "test_terrain.csv");
	
	//load sprite sheet into a texture
	SDL_Texture *sprite_texture = Texture::load("saitama.png", Engine::renderer);

	//create sprite database that can store many animated sprites
	Sprite::Data sprite_database;
	Sprite::init(&sprite_database, 10);

	//create physics bodies
	Body::Body bodies;
	Body::init(&bodies, 100);

	//extract animations from the loaded texture
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
	//create animation instances for player
	//run
	Sprite::make_Instance(0, &sprite_database);
	Sprite::modify(0, 0, &sprite_database, 100);
	//idle
	Sprite::make_Instance(1, &sprite_database);
	Sprite::modify(1, 0, &sprite_database, 100);
	//player position in the map and its size (size in tile cells)
	Shape::Rect player_grid_rect = { 2,8,1.00,1.00 };

	//make a physics body instance for the player
	int player_physics_body = Body::make(&bodies);
	Vec2D::Vec2D tmp_pos = { player_grid_rect.x,player_grid_rect.y };
	Body::modify(player_physics_body, &bodies, &tmp_pos, 1.0);

	//create a grid camera and adjust its canvas size
	Grid_Camera::Grid_Camera camera;
	Grid_Camera::init(&camera, Engine::screen_width, Engine::screen_height);
	camera.grid_coord.w = Engine::screen_width / 64;
	camera.grid_coord.h = Engine::screen_height / 64;

	printf("map nrows %d ncols %d\n", collision_map.n_rows, collision_map.n_cols);

	//some parameters for the map
	float current_friction = 0.0;
	float friction_gravel = 0.98;
	float friction_ice = 0.9998;
	float friction_swamp = 0.95;

	float player_move_force_magnitude_current = 0.0001;
	float player_move_force_magnitude_gravel = 0.00012;
	float player_move_force_magnitude_ice = 0.000024;
	float player_move_force_magnitude_swamp = 0.0001;
	float player_move_force_magnitude_air = 0.00002;

	float player_jump_force_magnitude_current = 0.01;
	float player_jump_force_magnitude_gravel = 0.012;
	float player_jump_force_magnitude_ice = 0.01;
	float player_jump_force_magnitude_swamp = 0.008;

	//gravity applied to player body
	Vec2D::Vec2D gravity = { 0,0.00005 };

	//type of tile which the player collision box collides at the bottom
	int bottom_tile_type = 0;

	int flip = 0;

	float max_vel_x = 1.0/32.0;
	float max_vel_y = 1.0/32.0;
	
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

		//player state 1 = running, 0 = idle
		if (cmd_LEFT || cmd_RIGHT)
		{
			player_state = 1;
		}
		else
		{
			player_state = 0;
		}

		//get the body position and assign to player grid position
		player_grid_rect.x = bodies.pos[player_physics_body].x;
		player_grid_rect.y = bodies.pos[player_physics_body].y;
		
		int player_collision_top = 0;
		int player_collision_bottom = 0;
		int player_collision_left = 0;
		int player_collision_right = 0;

		//vertical collision check
		Shape::Rect player_collision_rect_vertical;
		player_collision_rect_vertical.x = player_grid_rect.x + 0.4;
		player_collision_rect_vertical.y = player_grid_rect.y + 0.2;
		player_collision_rect_vertical.w = player_grid_rect.w - 2.0*0.4;
		player_collision_rect_vertical.h = player_grid_rect.h - 0.3;

		bottom_tile_type = 0;
		Grid::Region grid_under_collision_rect_vertical;
		Grid::get_Region_Under_Shape(&grid_under_collision_rect_vertical, &player_collision_rect_vertical);
		Grid::clip_Grid_Region(&grid_under_collision_rect_vertical, &collision_map);
		for (int i = grid_under_collision_rect_vertical.y0; i <= grid_under_collision_rect_vertical.y1; i++)
		{
			for (int j = grid_under_collision_rect_vertical.x0; j <= grid_under_collision_rect_vertical.x1; j++)
			{
				if (collision_map.data[i*collision_map.n_cols + j] == 0) continue;

				if (player_collision_rect_vertical.y < i)
				{
					player_collision_bottom = 1;

					bottom_tile_type = tile_map.data[i*collision_map.n_cols + j];
					
				}
				else
				{
					player_collision_top = 1;
				}

			}
		}


		//vertical collision

		Shape::Rect player_collision_rect_horizontal;
		player_collision_rect_horizontal.x = player_grid_rect.x + 0.2;
		player_collision_rect_horizontal.y = player_grid_rect.y + 0.4;
		player_collision_rect_horizontal.w = player_grid_rect.w - 0.4;
		player_collision_rect_horizontal.h = player_grid_rect.h - 2.0*0.4;

		Grid::Region grid_under_collision_rect_horizontal;
		Grid::get_Region_Under_Shape(&grid_under_collision_rect_horizontal, &player_collision_rect_horizontal);
		Grid::clip_Grid_Region(&grid_under_collision_rect_horizontal, &collision_map);
		for (int i = grid_under_collision_rect_horizontal.y0; i <= grid_under_collision_rect_horizontal.y1; i++)
		{
			for (int j = grid_under_collision_rect_horizontal.x0; j <= grid_under_collision_rect_horizontal.x1; j++)
			{
				if (collision_map.data[i*collision_map.n_cols + j] == 0) continue;

				if (player_collision_rect_horizontal.x < j)
				{
					player_collision_right = 1;
				}
				else
				{
					player_collision_left = 1;
				}

			}
		}


		//START ADDING FORCES
		Body::clear_Forces(&bodies);

		if (cmd_RIGHT == 1 && player_collision_right == 0)
		{
			Vec2D::Vec2D f = { player_move_force_magnitude_current, 0 };
			Body::add_Force(player_physics_body, &bodies, &f);
		}
		if (cmd_LEFT == 1 && player_collision_left == 0)
		{
			Vec2D::Vec2D f = { -player_move_force_magnitude_current, 0 };
			Body::add_Force(player_physics_body, &bodies, &f);
		}

		if (cmd_UP == 1 && player_collision_bottom == 1)
		{
			Vec2D::Vec2D f = { 0, -player_jump_force_magnitude_current};
			Body::add_Force(player_physics_body, &bodies, &f);
		}

		Body::add_Force(player_physics_body, &bodies, &gravity);

		if (bottom_tile_type == 247)
		{
			Vec2D::Vec2D f = { 0, -player_jump_force_magnitude_current*20 };
			Body::add_Force(player_physics_body, &bodies, &f);
			//bodies.vel[player_physics_body].y = 0.0;
		}
		else if (bottom_tile_type == 96)
		{
			Vec2D::Vec2D f = { 0, -player_jump_force_magnitude_current * 40 };
			Body::add_Force(player_physics_body, &bodies, &f);
			//bodies.vel[player_physics_body].y = 0.0;
		}

		//DONE ADDING FORCES

		//integrate body physics

		//integrate acceleration
		bodies.vel[player_physics_body].x += bodies.force[player_physics_body].x / bodies.mass[player_physics_body];
		bodies.vel[player_physics_body].y += bodies.force[player_physics_body].y / bodies.mass[player_physics_body];
		//clip velocity
		if (bodies.vel[player_physics_body].x > max_vel_x)bodies.vel[player_physics_body].x = max_vel_x;
		if (bodies.vel[player_physics_body].x < -max_vel_x)bodies.vel[player_physics_body].x = -max_vel_x;
		if (bodies.vel[player_physics_body].y > max_vel_y)bodies.vel[player_physics_body].y = max_vel_y;
		if (bodies.vel[player_physics_body].y < -max_vel_y)bodies.vel[player_physics_body].y = -max_vel_y;
		

		if (bottom_tile_type == 235)
		{
			current_friction = friction_ice;
			player_jump_force_magnitude_current = player_jump_force_magnitude_ice;
			player_move_force_magnitude_current = player_move_force_magnitude_ice;
		}
		else if (bottom_tile_type == 207)
		{
			current_friction = friction_swamp;
			player_jump_force_magnitude_current = player_jump_force_magnitude_swamp;
			player_move_force_magnitude_current = player_move_force_magnitude_swamp;
		}
		else if (bottom_tile_type == 0)
		{
			current_friction = 0.0;
			player_move_force_magnitude_current = player_move_force_magnitude_air;
		}
		else
		{
			current_friction = friction_gravel;
			player_jump_force_magnitude_current = player_jump_force_magnitude_gravel;
			player_move_force_magnitude_current = player_move_force_magnitude_gravel;
		}

		if (player_collision_bottom == 1)
		{
			bodies.vel[player_physics_body].x *= current_friction;
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
		Grid::clip_Grid_Region(&grid_region, &collision_map);
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

			int *tmp_level_data = &tile_map.data[i*tile_map.n_cols];
			for (int j = grid_region.x0; j <= grid_region.x1; j++)
			{
				int grid_data = tmp_level_data[j];
				int tileset_idx = grid_data / tileset.n_cols[0];
				int tileset_offset = grid_data % tileset.n_cols[0];
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

#if DEBUG 1
		int red = 0;
		int green = 0;
		for (int i = grid_under_collision_rect_vertical.y0; i <= grid_under_collision_rect_vertical.y1; i++)
		{
			for (int j = grid_under_collision_rect_vertical.x0; j <= grid_under_collision_rect_vertical.x1; j++)
			{
				if (collision_map.data[i*collision_map.n_cols + j] == 0) continue;
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

		for (int i = grid_under_collision_rect_horizontal.y0; i <= grid_under_collision_rect_horizontal.y1; i++)
		{
			for (int j = grid_under_collision_rect_horizontal.x0; j <= grid_under_collision_rect_horizontal.x1; j++)
			{
				if (collision_map.data[i*collision_map.n_cols + j] == 0) continue;
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
		Shape::Rect player_collision_screen_rect_vertical;
		Grid_Camera::grid_Coord_to_Screen_Coord(&player_collision_screen_rect_vertical, &player_collision_rect_vertical, &camera);

		SDL_Rect r = { player_collision_screen_rect_vertical.x,player_collision_screen_rect_vertical.y,player_collision_screen_rect_vertical.w,player_collision_screen_rect_vertical.h };
		SDL_RenderDrawRect(Engine::renderer, &r);

		Shape::Rect player_collision_screen_rect_horizontal;
		Grid_Camera::grid_Coord_to_Screen_Coord(&player_collision_screen_rect_horizontal, &player_collision_rect_horizontal, &camera);

		SDL_Rect r2 = { player_collision_screen_rect_horizontal.x,player_collision_screen_rect_horizontal.y,player_collision_screen_rect_horizontal.w,player_collision_screen_rect_horizontal.h };
		SDL_RenderDrawRect(Engine::renderer, &r2);
#endif
		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}



	return 0;
}
