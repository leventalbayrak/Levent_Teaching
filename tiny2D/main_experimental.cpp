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
#include "Collision_core.h"

namespace My_Game
{

	struct Actor
	{
		//0 idle, 1 run, 2 jump
		int state;
		//0 right, 1 left
		int sprite_direction;

		int run_sprite_id;
		int idle_sprite_id;
		int jump_sprite_id;

		int run_sprite_db_index;
		int idle_sprite_db_index;
		int jump_sprite_db_index;

		int move_cmd_left;
		int move_cmd_right;
		int move_cmd_up;
		int move_cmd_down;

		Shape::Rect world_coord;

		float move_force_mag;
		float jump_force_mag;

		int physics_body;
	};

	void init_Actor(Actor *p, Shape::Rect *r);
	void update_Actor(Actor *p, unsigned int current_time);
	void draw_Actor(Actor *p);

	namespace Input
	{
		unsigned char prev_key_state[256];
		unsigned char *keys = NULL;
	}

	namespace Command
	{
		int cmd_UP, cmd_DOWN, cmd_LEFT, cmd_RIGHT;
	}

	namespace Assets
	{
		int font_index;

		int coin_sound;
		int ting_sound;
		int music;

		int player_run_sprite_index;
		int player_idle_sprite_index;
		int player_jump_sprite_index;
	}

	namespace World
	{
		Actor player;

		Grid::Grid collision_map;
		Grid::Grid tilemap;
		Grid::Grid map_objects;

		Grid_Camera::Grid_Camera camera;

		Body::Body bodies;

		namespace Parameters
		{
			//some parameters for the map
			Vec2D::Vec2D gravity = { 0.0,0.00005 };

			float floor_friction = 0.985;
			float air_friction = 1.0;

			/*float player_current_friction = floor_friction;
			float player_move_force_magnitude_current = 0.0002;
			float player_jump_force_magnitude_current = 0.01;
			float player_super_jump_force_magnitude = 0.04;*/

			float max_vel_x = 1.0 / 32.0;
			float max_vel_y = 1.0 / 32.0;

			int super_jump_tile_id = 96;
		}
	}

	void init()
	{
		//initialize all systems and open game window
		Engine::init("hello side scroller", 480 * 2, 384 * 2);

		Input::keys = (unsigned char*)SDL_GetKeyboardState(NULL);

		Assets::font_index = Font::add("Transparent_Font.png", 64, 64, Engine::renderer);

		Assets::coin_sound = Audio::add_FX("coin.wav");
		Assets::ting_sound = Audio::add_FX("ting.wav");
		Assets::music = Audio::add_Music("dracula.mp3");

		//load tileset image
		Engine::add_Tileset("map_tileset.txt");

		//load collision map (level collisions), "tiled" export csv
		Grid::load(&World::collision_map, "platformer_collision.csv");
		//load tile map (visuals), "tiled" export csv
		Grid::load(&World::tilemap, "platformer_map.csv");
		//load tile map objects, "tiled" export csv
		Grid::load(&World::map_objects, "platformer_objects.csv");

		//load player sprite
		Engine::add_Sprite("saitama.txt");
		Assets::player_run_sprite_index = 0;
		Assets::player_idle_sprite_index = 1;
		Assets::player_jump_sprite_index = 2;

		//init camera
		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.grid_coord.w = Engine::screen_width / 32;
		World::camera.grid_coord.h = Engine::screen_height / 32;

		//create physics body manager
		Body::init(&World::bodies, 100);
	}

	void begin()
	{
		Audio::set_Music_Volume(64);
		Audio::play_Music(Assets::music, -1);

		Shape::Rect player_rect = { 10,10,1,1 };
		init_Actor(&World::player, &player_rect);
		
	}

	void update(unsigned int current_time)
	{

		memcpy(Input::prev_key_state, Input::keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		Command::cmd_UP = 0;
		Command::cmd_DOWN = 0;
		Command::cmd_LEFT = 0;
		Command::cmd_RIGHT = 0;

		if (Input::keys[SDL_SCANCODE_W] && Input::prev_key_state[SDL_SCANCODE_W] == 0) Command::cmd_UP = 1;
		if (Input::keys[SDL_SCANCODE_S]) Command::cmd_DOWN = 1;
		if (Input::keys[SDL_SCANCODE_A]) Command::cmd_LEFT = 1;
		if (Input::keys[SDL_SCANCODE_D]) Command::cmd_RIGHT = 1;

		World::player.move_cmd_left = Command::cmd_LEFT;
		World::player.move_cmd_right = Command::cmd_RIGHT;
		World::player.move_cmd_up = Command::cmd_UP;
		World::player.move_cmd_down = Command::cmd_DOWN;

		update_Actor(&World::player, current_time);
	}

	void draw()
	{

		My_Game::World::camera.grid_coord.x = World::player.world_coord.x - My_Game::World::camera.grid_coord.w / 2;
		My_Game::World::camera.grid_coord.y = World::player.world_coord.y - My_Game::World::camera.grid_coord.h / 2;

		//set color to white
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Engine::renderer);

		Grid::Region grid_region;
		//if the camera was on top of a grid, which cells would its grid_coord be covering
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &World::camera);
		Grid::clip_Grid_Region(&grid_region, World::tilemap.n_cols, World::tilemap.n_rows);

		Engine::E_Tileset::draw(0, &World::camera, &grid_region, &World::tilemap);

		draw_Actor(&World::player);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}

	void init_Actor(Actor *p, Shape::Rect *r)
	{
		p->run_sprite_id = Engine::make_Sprite_Instance(Assets::player_run_sprite_index);
		Engine::modify_Sprite_Instance(Assets::player_run_sprite_index, p->run_sprite_id, 100);

		p->idle_sprite_id = Engine::make_Sprite_Instance(Assets::player_idle_sprite_index);
		Engine::modify_Sprite_Instance(Assets::player_idle_sprite_index, p->idle_sprite_id, 100);

		p->jump_sprite_id = Engine::make_Sprite_Instance(Assets::player_jump_sprite_index);
		Engine::modify_Sprite_Instance(Assets::player_jump_sprite_index, p->jump_sprite_id, 100);

		p->world_coord = *r;

		p->physics_body = Body::make(&World::bodies);
		Vec2D::Vec2D tmp_pos = { p->world_coord.x,p->world_coord.y };
		Body::modify(p->physics_body, &World::bodies, &tmp_pos, 1.0);

		p->sprite_direction = 0;
		p->state = 0;
		
	}
	
	void update_Actor(Actor *p, unsigned int current_time)
	{
		if (p->move_cmd_left) p->sprite_direction = 1;
		else if (p->move_cmd_right) p->sprite_direction = 0;

		//player state 1 = running, 0 = idle
		if (p->move_cmd_left || p->move_cmd_right)
		{
			p->state = 1;
		}
		else
		{
			p->state = 0;
		}

		p->world_coord.x = World::bodies.pos[p->physics_body].x;
		p->world_coord.y = World::bodies.pos[p->physics_body].y;

		int actor_collision_top = 0;
		int actor_collision_bottom = 0;
		int actor_collision_left = 0;
		int actor_collision_right = 0;
		int actor_on_super_jump_tile = 0;

		Collision::Point_Feeler actor_feelers;
		Collision::sense_with_Point_Feelers(&actor_feelers, &p->world_coord);
		
		if (Grid::tile(&actor_feelers.right_feeler, &World::collision_map) > 0)
		{
			actor_collision_right = 1;
		}
		if (Grid::tile(&actor_feelers.left_feeler, &World::collision_map) > 0)
		{
			actor_collision_left = 1;
		}
		if (Grid::tile(&actor_feelers.bottom_feeler, &World::collision_map) > 0)
		{
			actor_collision_bottom = 1;
		}
		if (Grid::tile(&actor_feelers.top_feeler, &World::collision_map) > 0)
		{
			actor_collision_top = 1;
		}

		if (Grid::tile(&actor_feelers.bottom_feeler, &World::map_objects) == World::Parameters::super_jump_tile_id)
		{
			actor_on_super_jump_tile = 1;
		}

		if (actor_collision_bottom == 0)
		{
			p->state = 2;
		}

		//START ADDING FORCES
		//clear forces on player
		World::bodies.force[p->physics_body] = {};

		Body::add_Force(p->physics_body, &World::bodies, &World::Parameters::gravity);

		if (actor_on_super_jump_tile)
		{
			Vec2D::Vec2D f = { 0, -p->jump_force_mag*4 };
			Body::add_Force(p->physics_body, &World::bodies, &f);
		}

		if (p->move_cmd_right == 1 && actor_collision_right == 0)
		{
			Vec2D::Vec2D f = { p->move_force_mag, 0 };
			Body::add_Force(p->physics_body, &World::bodies, &f);

		}
		if (p->move_cmd_left == 1 && actor_collision_left == 0)
		{
			Vec2D::Vec2D f = { -p->move_force_mag, 0 };
			Body::add_Force(p->physics_body, &World::bodies, &f);

		}

		if (p->move_cmd_up == 1 && actor_collision_top == 0 && actor_collision_bottom == 1)
		{
			Vec2D::Vec2D f = { 0, -p->jump_force_mag };
			Body::add_Force(p->physics_body, &World::bodies, &f);

		}

		float current_friction = 0.985;//later...

		//DONE ADDING FORCES

		//PHYSICS BEGIN

		//integrate acceleration
		Body::update_Vel(p->physics_body, &World::bodies);
		//clip velocity
		Vec2D::clip(&World::bodies.vel[p->physics_body], -World::Parameters::max_vel_x, World::Parameters::max_vel_x, -World::Parameters::max_vel_y, World::Parameters::max_vel_y);

		//apply friction
		World::bodies.vel[p->physics_body].x *= current_friction;

		if (abs(World::bodies.vel[p->physics_body].x) < 0.00001) World::bodies.vel[p->physics_body].x = 0.0;
		if (abs(World::bodies.vel[p->physics_body].y) < 0.00001) World::bodies.vel[p->physics_body].y = 0.0;

		//apply velocity constraints
		if (actor_collision_bottom == 1 && World::bodies.vel[p->physics_body].y > 0)
		{
			World::bodies.vel[p->physics_body].y = 0;
		}
		if (actor_collision_top == 1 && World::bodies.vel[p->physics_body].y < 0)
		{
			World::bodies.vel[p->physics_body].y = 0;
		}
		if (actor_collision_right == 1 && World::bodies.vel[p->physics_body].x > 0)
		{
			World::bodies.vel[p->physics_body].x = 0;
		}
		if (actor_collision_left == 1 && World::bodies.vel[p->physics_body].x < 0)
		{
			World::bodies.vel[p->physics_body].x = 0;
		}

		//integrate velocity
		Body::update_Pos(p->physics_body, &World::bodies);

		if (p->state == 0)
		{
			Sprite::update(p->idle_sprite_db_index, p->idle_sprite_id, &Engine::sprite_db, current_time);
		}
		else if(p->state == 1)
		{
			Sprite::update(p->run_sprite_db_index, p->run_sprite_id, &Engine::sprite_db, current_time);	
		}
		else if(p->state == 2)
		{
			Sprite::update(p->jump_sprite_db_index, p->jump_sprite_id, &Engine::sprite_db, current_time);
		}
		else
		{
			Sprite::update(p->idle_sprite_db_index, p->idle_sprite_id, &Engine::sprite_db, current_time);
		}
	}

	void draw_Actor(Actor *p)
	{
		Shape::Rect player_screen_rect;
		Grid_Camera::grid_to_Screen(&player_screen_rect, &p->world_coord, &World::camera);

		if (p->state == 0)
		{
			Sprite::draw(Assets::player_idle_sprite_index, p->idle_sprite_id, &Engine::sprite_db, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, p->sprite_direction);
		}
		else if (p->state == 1)
		{
			Sprite::draw(Assets::player_run_sprite_index, p->run_sprite_id, &Engine::sprite_db, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, p->sprite_direction);
		}
		else if (p->state == 2)
		{
			Sprite::draw(Assets::player_jump_sprite_index, p->jump_sprite_id, &Engine::sprite_db, player_screen_rect.x, player_screen_rect.y, player_screen_rect.w, player_screen_rect.h, Engine::renderer, p->sprite_direction);
		}
	}
}

int main(int argc, char **argv)
{

	My_Game::init();
	My_Game::begin();

	bool done = false;
	while (!done)
	{
		unsigned int current_time = SDL_GetTicks();

		My_Game::update(done);

		My_Game::update(current_time);

		My_Game::draw();
	}



	return 0;
}
