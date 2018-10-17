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
#include "Font_core.h"

#define DEBUG 0

int main(int argc, char **argv)
{
	//initialize all systems and open game window
	Engine::init("hello topdown", 960, 768);

	Font::init();
	int font_index_0 = Font::add("lazy_font.png", 64, 64, Engine::renderer);
	//int font_index_1 = Font::add("lazy_font_2.png", 64, 64, Engine::renderer);

	//load tileset image
	Tileset::Tileset tileset;
	Tileset::init(&tileset, 10);
	Tileset::modify(Tileset::make(&tileset), &tileset, "tileset.png", 32, 32, 23, 21, Engine::renderer);
	
	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	//load collision map (level collisions), "tiled" export csv
	Grid::Grid collision_map;
	Grid::load(&collision_map, "topdown_collision.csv");
	//load tile map (visuals), "tiled" export csv
	Grid::Grid tilemap;
	Grid::load(&tilemap, "topdown_map.csv");

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
	Shape::Rect player_grid_rect = { tilemap.n_cols / 2, tilemap.n_rows / 2 ,1.00,1.00 };

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
	float current_friction = 0.985;
	float player_move_force_magnitude_current = 0.0002;
	float max_vel_x = 1.0 / 32.0;
	float max_vel_y = 1.0 / 32.0;

	//type of tile which the player collision box collides at the bottom
	int vertical_collision_tile_type = 0;
	int horizontal_collision_tile_type = 0;

	int mouse_x, mouse_y;

	int flip = 0;

	

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


		unsigned int button = SDL_GetMouseState(&mouse_x, &mouse_y);
		Vec2D::Vec2D mouse_screen_pos = { mouse_x,mouse_y };
		Vec2D::Vec2D mouse_grid_pos;
		Grid_Camera::screen_to_Grid(&mouse_grid_pos, &mouse_screen_pos, &camera);
		if (button & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			int index = (int)mouse_grid_pos.y*collision_map.n_cols + (int)mouse_grid_pos.x;
			collision_map.data[index] = 1;
			tilemap.data[index] = 9;
		}
		if (button & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			int index = (int)mouse_grid_pos.y*collision_map.n_cols + (int)mouse_grid_pos.x;
			collision_map.data[index] = 0;
			tilemap.data[index] = 0;
		}

		/*
		GAME CODE
		*/

		if (cmd_LEFT) flip = 1;
		if (cmd_RIGHT) flip = 0;

		//player state 1 = running, 0 = idle
		if (cmd_LEFT || cmd_RIGHT || cmd_UP || cmd_DOWN)
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

		int vc, vr, vd, hc, hr, hd;
		Grid::handle_Collision_Ugly(vc, vr, vd, hc, hr, hd, &player_collision_rect_vertical, &collision_map);

		vertical_collision_tile_type = 0;
		if (vd == 0)
		{
			player_collision_bottom = 1;
			vertical_collision_tile_type = tilemap.data[vr*collision_map.n_cols + vc];
		}
		else if (vd == 1)
		{
			player_collision_top = 1;
			vertical_collision_tile_type = tilemap.data[vr*collision_map.n_cols + vc];
		}

		//vertical collision

		Shape::Rect player_collision_rect_horizontal;
		player_collision_rect_horizontal.x = player_grid_rect.x + 0.2;
		player_collision_rect_horizontal.y = player_grid_rect.y + 0.4;
		player_collision_rect_horizontal.w = player_grid_rect.w - 0.4;
		player_collision_rect_horizontal.h = player_grid_rect.h - 2.0*0.4;

		vc, vr, vd, hc, hr, hd;
		Grid::handle_Collision_Ugly(vc, vr, vd, hc, hr, hd, &player_collision_rect_horizontal, &collision_map);

		horizontal_collision_tile_type = 0;
		if (hd == 0)
		{
			player_collision_left = 1;
			horizontal_collision_tile_type = tilemap.data[hr*collision_map.n_cols + hc];
		}
		else if (hd == 1)
		{
			player_collision_right = 1;
			horizontal_collision_tile_type = tilemap.data[hr*collision_map.n_cols + hc];
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

		if (cmd_UP == 1 && player_collision_top == 0)
		{
			Vec2D::Vec2D f = { 0, -player_move_force_magnitude_current };
			Body::add_Force(player_physics_body, &bodies, &f);
		}

		if (cmd_DOWN == 1 && player_collision_bottom == 0)
		{
			Vec2D::Vec2D f = { 0, player_move_force_magnitude_current };
			Body::add_Force(player_physics_body, &bodies, &f);
		}


		//DONE ADDING FORCES

		//PHYSICS BEGIN

		//integrate acceleration
		Body::update_Vel(player_physics_body, &bodies);
		//clip velocity
		Vec2D::clip(&bodies.vel[player_physics_body], -max_vel_x, max_vel_x, -max_vel_y, max_vel_y);

		//apply friction
		bodies.vel[player_physics_body].x *= current_friction;
		bodies.vel[player_physics_body].y *= current_friction;
		if (abs(bodies.vel[player_physics_body].x) < 0.00001) bodies.vel[player_physics_body].x = 0.0;
		if (abs(bodies.vel[player_physics_body].y) < 0.00001) bodies.vel[player_physics_body].y = 0.0;

		//apply velocity constraints
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
		Body::update_Pos(player_physics_body, &bodies);

		//PHYSICS DONE

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

			int *tmp_level_data = &tilemap.data[i*tilemap.n_cols];
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
		Grid_Camera::grid_to_Screen(&player_screen_rect, &player_grid_rect, &camera);

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

		static char msg[256];
		Vec2D::Vec2D text_pos;
		text_pos.x = mouse_screen_pos.x + 5;
		text_pos.y = mouse_screen_pos.y + 15;
		sprintf(msg, "[%.1f,%.1f]\n", mouse_grid_pos.x, mouse_grid_pos.y);
		Font::draw(&text_pos, msg, strlen(msg), font_index_0, 0.25, &camera, Engine::renderer);

		text_pos.x = 10;
		text_pos.y = 10;
		sprintf(msg, "left click - add block\nright click - delete block");
		Font::draw(&text_pos, msg, strlen(msg), font_index_0, 0.25, &camera, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}



	return 0;
}
