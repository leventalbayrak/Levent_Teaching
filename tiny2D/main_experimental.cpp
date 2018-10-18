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
#include "Audio_core.h"
#include "Texture_core.h"
#include "Shape_core.h"
#include "Font_core.h"

#define DEBUG 0

int main(int argc, char **argv)
{
	//initialize all systems and open game window
	Engine::init("hello side scroller", 960, 768);

	int font_index_0 = Font::add("Transparent_Font.png", 64, 64, Engine::renderer);


	int coin_sound = Audio::add_FX("coin.wav");
	int ting_sound = Audio::add_FX("ting.wav");
	int music = Audio::add_Music("dracula.mp3");

	//load tileset image
	Engine::add_Tileset("map_tileset.txt");
	
	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	//load collision map (level collisions), "tiled" export csv
	Grid::Grid collision_map;
	Grid::load(&collision_map, "platformer_collision.csv");
	//load tile map (visuals), "tiled" export csv
	Grid::Grid tilemap;
	Grid::load(&tilemap, "platformer_map.csv");

	//create sprite database that can store many animated sprites from different sprite sheets
	
	Engine::add_Sprite("saitama.txt");	

	//create physics bodies
	Body::Body bodies;
	Body::init(&bodies, 100);

	int player_state = 0;
	//create animation instances for player
	//run
	int k = Engine::make_Sprite_Instance(0);
	Engine::modify_Sprite_Instance(0, k, 100);

	k = Engine::make_Sprite_Instance(1);
	Engine::modify_Sprite_Instance(1, k, 100);

	//player position in the map and its size (size in tile cells)
	Shape::Rect player_grid_rect = { 10, 10 ,1.0,1.0 };

	//make a physics body instance for the player
	int player_physics_body = Body::make(&bodies);
	Vec2D::Vec2D tmp_pos = { player_grid_rect.x,player_grid_rect.y };
	Body::modify(player_physics_body, &bodies, &tmp_pos, 1.0);

	Vec2D::Vec2D gravity = { 0.0,0.00005 };

	//create a grid camera and adjust its canvas size
	Grid_Camera::Grid_Camera camera;
	Grid_Camera::init(&camera, Engine::screen_width, Engine::screen_height);
	camera.grid_coord.w = Engine::screen_width / 32;
	camera.grid_coord.h = Engine::screen_height / 32;

	//some parameters for the map
	
	float floor_friction = 0.985;
	float air_friction = 1.0;
	float player_current_friction = floor_friction;

	float player_move_force_magnitude_current = 0.0002;
	float player_jump_force_magnitude_current = 0.01;
	float max_vel_x = 1.0 / 32.0;
	float max_vel_y = 1.0 / 32.0;

	//type of tile which the player collision box collides at the bottom
	int vertical_collision_tile_type = 0;
	int horizontal_collision_tile_type = 0;

	int mouse_x, mouse_y;

	int flip = 0;

	Audio::set_Music_Volume(64);
	Audio::play_Music(music, -1);

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


		if (keys[SDL_SCANCODE_W] && prev_key_state[SDL_SCANCODE_W] == 0) cmd_UP = 1;
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

		if (vd == -1)
		{
			player_current_friction = air_friction;
		}
		else
		{
			//if (vertical_collision_tile_type == 1)
			{
				player_current_friction = floor_friction;
			}
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

		Body::add_Force(player_physics_body, &bodies, &gravity);

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

		if (cmd_UP == 1 && player_collision_top == 0 && player_collision_bottom == 1)
		{
			Vec2D::Vec2D f = { 0, -player_jump_force_magnitude_current };
			Body::add_Force(player_physics_body, &bodies, &f);

		}
	
		//DONE ADDING FORCES

		//PHYSICS BEGIN

		//integrate acceleration
		Body::update_Vel(player_physics_body, &bodies);
		//clip velocity
		Vec2D::clip(&bodies.vel[player_physics_body], -max_vel_x, max_vel_x, -max_vel_y, max_vel_y);

		//apply friction
		bodies.vel[player_physics_body].x *= player_current_friction;
		
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
			Audio::play_FX(coin_sound);
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

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Engine::renderer);

		Grid::Region grid_region;
		//if the camera was on top of a grid, which cells would its grid_coord be covering
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &camera);
		Grid::clip_Grid_Region(&grid_region, tilemap.n_cols, tilemap.n_rows);

		Engine::E_Tileset::draw(0, &camera, &grid_region, &tilemap);

		Shape::Rect player_screen_rect;
		Grid_Camera::grid_to_Screen(&player_screen_rect, &player_grid_rect, &camera);

		if (player_state == 1)
		{
			Sprite::update(0, 0, &Engine::sprite_db, current_time);
			Sprite::draw(0, 0, &Engine::sprite_db, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, flip);
		}
		else
		{
			Sprite::update(1, 0, &Engine::sprite_db, current_time);
			Sprite::draw(1, 0, &Engine::sprite_db, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, flip);

		}

		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}



	return 0;
}
