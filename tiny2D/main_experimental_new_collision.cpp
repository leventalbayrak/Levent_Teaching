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

	void init_Actor_Assets(Actor *p, int idle_sprite_index, int run_sprite_index, int jump_sprite_index);
	void update_Actor(Actor *p, unsigned int current_time);
	void update_Actor_Physics(Actor *p, unsigned int current_time);
	void draw_Actor(Actor *p);
	void update_Enemy_AI(Actor *e, unsigned int current_time);
	void update_Map_Events(Actor *e, unsigned int current_time);

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

		namespace Parameters
		{
			//some parameters for the map
			Vec2D::Vec2D gravity = { 0.0,0.00005 };

			float floor_friction = 0.985;
			float air_friction = 1.0;

			float max_vel_x = 1.0 / 24.0;
			float max_vel_y = 1.0 / 16.0;

			int super_jump_tile_id = 96;
			int teleport_tile_id = 283;

			const int n_enemies = 512;
			int n_active_enemies = 0;
		}

		Actor player;

		Actor enemies[Parameters::n_enemies];

		Grid::Grid collision_map;
		Grid::Grid tilemap;
		Grid::Grid object_map;

		Grid_Camera::Grid_Camera camera;
		Grid_Camera::Calibration calibration;

		Body::Body bodies;


	}

	void init()
	{
		//initialize all systems and open game window
		Engine::init("hello side scroller", 480 * 2, 384 * 2);

		Input::keys = (unsigned char*)SDL_GetKeyboardState(NULL);

		Assets::font_index = Font::add("Transparent_Font.png", 64, 64, Engine::renderer);

		//Assets::coin_sound = Audio::add_FX("coin.wav");
		//Assets::ting_sound = Audio::add_FX("ting.wav");
		//Assets::music = Audio::add_Music("dracula.mp3");

		//load tileset image
		Engine::E_Tileset::add("map_tileset.txt");

		//load collision map (level collisions), "tiled" export csv
		Grid::load(&World::collision_map, "platformer_collision.csv");
		//load tile map (visuals), "tiled" export csv
		Grid::load(&World::tilemap, "platformer_map.csv");
		//load tile map objects, "tiled" export csv
		Grid::load(&World::object_map, "platformer_objects.csv");

		//load player sprite
		int number_of_sprites_loaded = Engine::E_Sprite::add("saitama.txt");
		printf("loaded %d frames\n", number_of_sprites_loaded);
		Assets::player_run_sprite_index = 0;
		Assets::player_idle_sprite_index = 1;
		Assets::player_jump_sprite_index = 2;

		//init camera
		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.w = Engine::screen_width / 32;
		World::camera.world_coord.h = Engine::screen_height / 32;

		//create physics body manager
		Body::init(&World::bodies, World::Parameters::n_enemies);
	}

	void begin()
	{
		Audio::set_Music_Volume(64);
		Audio::play_Music(Assets::music, -1);

		//initialize player
		init_Actor_Assets(&World::player, Assets::player_idle_sprite_index, Assets::player_run_sprite_index, Assets::player_jump_sprite_index);
		World::player.world_coord = { 10,10,1,1 };
		World::player.jump_force_mag = 0.01;
		World::player.move_force_mag = 0.0001;
		World::player.sprite_direction = 0;
		World::player.state = 0;
		World::player.physics_body = Body::make(&World::bodies);
		Vec2D::Vec2D tmp_pos = { World::player.world_coord.x,World::player.world_coord.y };
		Body::modify(World::player.physics_body, &World::bodies, &tmp_pos, 1.0);

		for (int i = 0; i < World::Parameters::n_enemies; i++)
		{
			init_Actor_Assets(&World::enemies[i], Assets::player_idle_sprite_index, Assets::player_run_sprite_index, Assets::player_jump_sprite_index);
			World::enemies[i].world_coord = { (float)(5 + rand() % 10),(float)(10),1,1 };
			World::enemies[i].jump_force_mag = 0.01;
			World::enemies[i].move_force_mag = 0.0001;
			World::enemies[i].sprite_direction = 0;
			World::enemies[i].state = 0;
			World::enemies[i].physics_body = Body::make(&World::bodies);
			Vec2D::Vec2D tmp_pos = { World::enemies[i].world_coord.x,World::enemies[i].world_coord.y };
			Body::modify(World::enemies[i].physics_body, &World::bodies, &tmp_pos, 1.0);
		}
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

		//update player physics
		update_Map_Events(&World::player, current_time);
		update_Actor(&World::player, current_time);
		update_Actor_Physics(&World::player, current_time);

		if (World::Parameters::n_active_enemies >= World::Parameters::n_enemies)
		{
			exit(0);
		}
		for (int i = 0; i < World::Parameters::n_active_enemies; i++)
		{
			update_Enemy_AI(&World::enemies[i], current_time);
			update_Map_Events(&World::enemies[i], current_time);
			update_Actor(&World::enemies[i], current_time);
			update_Actor_Physics(&World::enemies[i], current_time);
		}

		//focus camera on player
		My_Game::World::camera.world_coord.x = World::player.world_coord.x - My_Game::World::camera.world_coord.w / 2;
		My_Game::World::camera.world_coord.y = World::player.world_coord.y - My_Game::World::camera.world_coord.h / 2;

	}

	void draw()
	{

		Grid::Region grid_region;
		//if the camera was on top of a grid, which cells would its grid_coord be covering
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &World::camera);
		Grid::clip_Grid_Region(&grid_region, World::tilemap.n_cols, World::tilemap.n_rows);

		Grid_Camera::calibrate(&World::calibration, &World::camera, &grid_region);

		//set color to white
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Engine::renderer);

		Engine::E_Tileset::draw(0, &World::calibration, &grid_region, &World::tilemap);

		draw_Actor(&World::player);
		for (int i = 0; i < World::Parameters::n_active_enemies; i++)
		{
			draw_Actor(&World::enemies[i]);
		}

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}

	void init_Actor_Assets(Actor *p, int idle_sprite_index, int run_sprite_index, int jump_sprite_index)
	{
		p->idle_sprite_db_index = idle_sprite_index;
		p->idle_sprite_id = Engine::E_Sprite::make_Instance(p->idle_sprite_db_index);
		Engine::E_Sprite::modify(p->idle_sprite_db_index, p->idle_sprite_id, 100);

		p->run_sprite_db_index = run_sprite_index;
		p->run_sprite_id = Engine::E_Sprite::make_Instance(p->run_sprite_db_index);
		Engine::E_Sprite::modify(p->run_sprite_db_index, p->run_sprite_id, 100);

		p->jump_sprite_db_index = jump_sprite_index;
		p->jump_sprite_id = Engine::E_Sprite::make_Instance(p->jump_sprite_db_index);
		Engine::E_Sprite::modify(p->jump_sprite_db_index, p->jump_sprite_id, 100);
	}

	void update_Actor_Physics(Actor *p, unsigned int current_time)
	{
		float current_friction = 0.985;//later...

		//DONE ADDING FORCES


		//PHYSICS BEGIN


		//printf("force %.4f %.4f vel %.4f %.4f\n", World::bodies.force[p->physics_body].x, World::bodies.force[p->physics_body].y, World::bodies.vel[p->physics_body].x, World::bodies.vel[p->physics_body].y);

		//integrate acceleration
		Body::update_Vel(p->physics_body, &World::bodies);
		//apply friction
		World::bodies.vel[p->physics_body].x *= current_friction;

		//clip velocity
		Vec2D::clip(&World::bodies.vel[p->physics_body], -World::Parameters::max_vel_x, World::Parameters::max_vel_x, -World::Parameters::max_vel_y, World::Parameters::max_vel_y);

		if (abs(World::bodies.vel[p->physics_body].x) < 0.00001) World::bodies.vel[p->physics_body].x = 0.0;
		if (abs(World::bodies.vel[p->physics_body].y) < 0.00001) World::bodies.vel[p->physics_body].y = 0.0;

		Shape::Rect tmp_pos = p->world_coord;
		tmp_pos.x += World::bodies.vel[p->physics_body].x;

		bool tmp_c = false;
		Grid::Region tmp_g;
		Shape::Rect collision_box;

		Collision::box(&collision_box, &tmp_pos, 0.1);
		Grid::get_Region_Under_Shape(&tmp_g, &collision_box);
		for (int y = tmp_g.y0; y <= tmp_g.y1; y++)
			for (int x = tmp_g.x0; x <= tmp_g.x1; x++)
			{
				if (Grid::tile(x, y, &World::collision_map) > 0)
				{
					tmp_c = true;
					//response
					World::bodies.vel[p->physics_body].x = 0;
					break;
				}
			}


		if (tmp_c == false)
		{
			World::bodies.pos[p->physics_body].x = tmp_pos.x;
		}

		tmp_pos.x = World::bodies.pos[p->physics_body].x;
		tmp_pos.y += World::bodies.vel[p->physics_body].y;

		tmp_c = false;
		Collision::box(&collision_box, &tmp_pos, 0.1);
		Grid::get_Region_Under_Shape(&tmp_g, &collision_box);
		for (int y = tmp_g.y0; y <= tmp_g.y1; y++)
			for (int x = tmp_g.x0; x <= tmp_g.x1; x++)
			{
				if (Grid::tile(x, y, &World::collision_map) > 0)
				{
					tmp_c = true;
					//response
					World::bodies.vel[p->physics_body].y = 0;
					break;
				}
			}

		if (tmp_c == false)
		{
			World::bodies.pos[p->physics_body].y = tmp_pos.y;
		}

		tmp_pos.y += World::bodies.vel[p->physics_body].y;

		World::bodies.force[p->physics_body] = {};

	}

	void update_Actor(Actor *p, unsigned int current_time)
	{
		if (p->move_cmd_left) p->sprite_direction = 1;
		else if (p->move_cmd_right) p->sprite_direction = 0;

		p->state = 0;//idle

		//player state 1 = running, 0 = idle
		if (p->move_cmd_left || p->move_cmd_right)
		{
			p->state = 1;
		}

		p->world_coord.x = World::bodies.pos[p->physics_body].x;
		p->world_coord.y = World::bodies.pos[p->physics_body].y;

		int on_ground = 0;
		Collision::Point_Feeler actor_feelers;
		Collision::point_Feeler_Pos(&actor_feelers, &p->world_coord, 0.1, 0.2);

		if (Grid::tile(&actor_feelers.bottom_feeler, &World::collision_map) > 0 ||
			Grid::tile(&actor_feelers.bottomleft_feeler, &World::collision_map) > 0 ||
			Grid::tile(&actor_feelers.bottomright_feeler, &World::collision_map) > 0)
		{
			on_ground = 1;
		}

		if (on_ground == 0)
		{
			p->state = 2;
		}

		//ADDING FORCES

		Body::add_Force(p->physics_body, &World::bodies, &World::Parameters::gravity);

		if (p->move_cmd_right == 1)
		{
			Vec2D::Vec2D f = { p->move_force_mag, 0 };
			Body::add_Force(p->physics_body, &World::bodies, &f);
		}
		if (p->move_cmd_left == 1)
		{
			Vec2D::Vec2D f = { -p->move_force_mag, 0 };
			Body::add_Force(p->physics_body, &World::bodies, &f);
		}

		if (p->move_cmd_up == 1 && on_ground == 1)
		{
			Vec2D::Vec2D f = { 0, -p->jump_force_mag*1.2 };
			Body::add_Force(p->physics_body, &World::bodies, &f);
		}

		//UPDATE ANIMATION FRAMES
		if (p->state == 0)
		{
			Engine::E_Sprite::update(p->idle_sprite_db_index, p->idle_sprite_id, current_time);
		}
		else if (p->state == 1)
		{
			Engine::E_Sprite::update(p->run_sprite_db_index, p->run_sprite_id, current_time);
		}
		else if (p->state == 2)
		{
			Engine::E_Sprite::update(p->jump_sprite_db_index, p->jump_sprite_id, current_time);
		}
		else
		{
			Engine::E_Sprite::update(p->idle_sprite_db_index, p->idle_sprite_id, current_time);
		}
	}

	void draw_Actor(Actor *p)
	{

		if (p->state == 0)
		{
			Engine::E_Sprite::draw(p->idle_sprite_db_index, p->idle_sprite_id, &p->world_coord, p->sprite_direction, &World::calibration, &World::camera);
		}
		else if (p->state == 1)
		{
			Engine::E_Sprite::draw(p->run_sprite_db_index, p->run_sprite_id, &p->world_coord, p->sprite_direction, &World::calibration, &World::camera);
		}
		else if (p->state == 2)
		{
			Engine::E_Sprite::draw(p->jump_sprite_db_index, p->jump_sprite_id, &p->world_coord, p->sprite_direction, &World::calibration, &World::camera);
		}
	}

	void update_Map_Events(Actor *e, unsigned int current_time)
	{
		Collision::Point_Feeler actor_feelers;
		Collision::point_Feeler_Pos(&actor_feelers, &e->world_coord, 0.05, 0.1);

		if (Grid::tile(&actor_feelers.mid_feeler, &World::object_map) == World::Parameters::teleport_tile_id)
		{
			e->world_coord.x = 10;
			e->world_coord.y = 10;
			World::bodies.pos[e->physics_body].x = e->world_coord.x;
			World::bodies.pos[e->physics_body].y = e->world_coord.y;

			World::Parameters::n_active_enemies++;
		}

		if (Grid::tile(&actor_feelers.mid_feeler, &World::object_map) == World::Parameters::super_jump_tile_id)
		{
			//FIX::feeler is inside the box for a long time!
			Vec2D::Vec2D f = { 0, -e->jump_force_mag*0.1 };
			Body::add_Force(e->physics_body, &World::bodies, &f);
		}
	}

	void update_Enemy_AI(Actor *e, unsigned int current_time)
	{

		e->move_cmd_left = 0;
		e->move_cmd_right = 0;
		e->move_cmd_up = 0;
		e->move_cmd_down = 0;


		Collision::Point_Feeler actor_feelers_far;
		Collision::point_Feeler_Pos(&actor_feelers_far, &e->world_coord, 0.4, 0.8);

		Collision::Point_Feeler actor_feelers_close;
		Collision::point_Feeler_Pos(&actor_feelers_close, &e->world_coord, 0.01, 0.02);

		if (Grid::tile(&actor_feelers_far.right_feeler, &World::collision_map) < 0)
		{
			e->move_cmd_right = 1;
		}
		else
		{
			//FIX::feeler inside collision for a long time!
			e->move_cmd_right = 1;
			if (rand() % 5 == 0)
			{
				e->move_cmd_right = 0;
				e->move_cmd_left = 1;
			}
			if (Grid::tile(&actor_feelers_close.bottom_feeler, &World::collision_map) > 0)
			{
				e->move_cmd_up = 1;
			}
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

		My_Game::update(current_time);

		My_Game::draw();
	}



	return 0;
}
