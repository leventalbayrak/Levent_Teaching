#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{

	namespace Command
	{

	}

	namespace Assets
	{
		namespace Animation
		{
			
		}
	}

	namespace World
	{
		
		namespace Parameters
		{
			unsigned int bullet_freq = 100;
			float f_player_move_mag = 1000;
			float friction_player = 0.5;
			Vec2D::Vec2D player_max_vel = { 12,12 };

			int n_enemies = 100;
		}

		Tileset::Tileset tileset;

		Font::Font text;

		Particle::Emitter emitter;
		Actor::Factory player;
		int player_actor_id;

		Actor::Factory enemy;
		Actor::Factory bullet;

		Grid_Camera::Grid_Camera camera;
		Grid::Grid tile_map;
		Grid::Grid background_map;

		
		unsigned int last_time_bullet_spawned;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Particle::init(&World::emitter, "bullet_casing.txt", 256, Engine::renderer);

		Actor::init(&World::player, 1);
		Actor::add_Sprite(&World::player, "ship.txt", Engine::renderer);

		Actor::init(&World::bullet, 512);
		Actor::add_Sprite(&World::bullet, "bullet.txt", Engine::renderer);

		Actor::init(&World::enemy, 512);
		Actor::add_Sprite(&World::enemy, "ship.txt", Engine::renderer);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);

		Grid::load(&World::tile_map, "bear_run_tilemap_tilemap.csv");
		Grid::load(&World::background_map, "bear_run_tilemap_background.csv");

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 1;
		World::camera.world_coord.w = 0.025 * Engine::screen_width;
		World::camera.world_coord.h = 0.025 * Engine::screen_height;

	}

	void begin_Play(unsigned int current_time)
	{
		World::player_actor_id = Actor::spawn(&World::player, 2.0, current_time);

		Font::set_Screen_Size(&World::text, 16, 16);

		Actor::set_Pos(World::player_actor_id, 5, 5, &World::player);

		for (int i = 0; i < World::Parameters::n_enemies; i++)
		{
			int k = Actor::spawn(&World::enemy, 2.0, current_time);
			Actor::set_Pos(k, 2 + rand() % 10, 2 + rand() % 10, &World::enemy);
		}

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	
		//add forces to actors

		if (current_time - World::last_time_bullet_spawned >= World::Parameters::bullet_freq)
		{
			if (Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_A] == 1 ||
				Input::keys[SDL_SCANCODE_SPACE] == 1)
			{
				int k = Actor::spawn(&World::bullet, 0.5, current_time);

				Actor::set_Pos(k,
					World::player.world_coords.rect[World::player_actor_id].x + 0.9*World::player.world_coords.rect[World::player_actor_id].w,
					World::player.world_coords.rect[World::player_actor_id].y + 0.3*World::player.world_coords.rect[World::player_actor_id].h,
					&World::bullet);

				Vec2D::Vec2D bullet_force = { 820, 0 };
				Actor::add_Force(k, &World::bullet, &bullet_force);

				Vec2D::Vec2D f_min, f_max;
				f_min.x = -80;
				f_max.x = 80;
				f_min.y = -400;
				f_max.y = -420;
				
				Vec2D::Vec2D pos = { World::player.world_coords.rect[World::player_actor_id].x + 0.1*World::player.world_coords.rect[World::player_actor_id].w,
					World::player.world_coords.rect[World::player_actor_id].y };

				Particle::spawn(&World::emitter, 1, 0.25, &pos, Actor::get_Vel(World::player_actor_id,&World::player) ,&f_min, &f_max, 2000, 5000, current_time);

				World::last_time_bullet_spawned = current_time;
			}
		}

		if (Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_LEFT] == 1 ||
			Input::keys[SDL_SCANCODE_A] == 1)
		{
			Vec2D::Vec2D f_move = { -World::Parameters::f_player_move_mag,0 };
			Actor::add_Force(World::player_actor_id, &World::player, &f_move);
		}
		if (Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_RIGHT] == 1 ||
			Input::keys[SDL_SCANCODE_D] == 1)
		{
			Vec2D::Vec2D f_move = { World::Parameters::f_player_move_mag,0 };
			Actor::add_Force(World::player_actor_id, &World::player, &f_move);
		}
		if (Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_UP] == 1 ||
			Input::keys[SDL_SCANCODE_W] == 1)
		{
			Vec2D::Vec2D f_move = { 0, -World::Parameters::f_player_move_mag };
			Actor::add_Force(World::player_actor_id, &World::player, &f_move);
		}
		if (Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_DOWN] == 1 ||
			Input::keys[SDL_SCANCODE_S] == 1)
		{
			Vec2D::Vec2D f_move = { 0, World::Parameters::f_player_move_mag };
			Actor::add_Force(World::player_actor_id, &World::player, &f_move);
		}


		//after done adding all forces
		//execute below

		
		Actor::update_Vel(World::player_actor_id, &World::player, dt);
		Vec2D::Vec2D friction = { World::Parameters::friction_player,World::Parameters::friction_player };
		Actor::apply_Friction(World::player_actor_id, &friction, &World::player);
		Vec2D::clip(Actor::get_Vel(World::player_actor_id, &World::player), -World::Parameters::player_max_vel.x, World::Parameters::player_max_vel.x, -World::Parameters::player_max_vel.y, World::Parameters::player_max_vel.y);
		Actor::update_Pos(World::player_actor_id, &World::player, dt);
		
		//collision checks here
		//if there is a collision call Actor::undo_Pos_Update()

		for (int i = 0; i < World::bullet.spawn_stack.array_size; i++)
		{
			if (World::bullet.spawn_stack.spawned[i] == 1)
			{
				if (current_time - World::bullet.creation_time[i] > 5000)
				{
					Actor::destroy(i, &World::bullet);
					continue;
				}

				Actor::update_Vel(i, &World::bullet,dt);
				Actor::update_Pos(i, &World::bullet,dt);
				//collision checks here
				//if there is a collision call Actor::undo_Pos_Update()
			}
		}

		for (int i = 0; i < World::enemy.spawn_stack.array_size; i++)
		{
			if (World::enemy.spawn_stack.spawned[i] == 1)
			{
				Actor::update_Vel(i, &World::enemy, dt);
				Actor::update_Pos(i, &World::enemy, dt);

				//collision checks here
				//if there is a collision call Actor::undo_Pos_Update()
			}
		}

		//collision
		for (int i = 0; i < World::bullet.spawn_stack.array_size; i++)
		{
			if (World::bullet.spawn_stack.spawned[i] == 0) continue;
			
			for (int j = 0; j < World::enemy.spawn_stack.array_size; j++)
			{
				if (World::enemy.spawn_stack.spawned[j] == 0) continue;

				int r = Shape::Rect::collision(&World::bullet.world_coords.rect[i], &World::enemy.world_coords.rect[j]);
				if (r)
				{
					Actor::add_Force(j, &World::enemy, &World::bullet.bodies.vel[i]);

					Vec2D::Vec2D def = {};
					Vec2D::Vec2D f_min = {-300,-300};
					Vec2D::Vec2D f_max = {200,200};
					Particle::spawn(&World::emitter, 8, 0.1,
						Actor::get_Pos(i,&World::bullet),
						&def,&f_min,&f_max,500,2000,current_time);

					Actor::destroy(i, &World::bullet);
				}

			}
		}
		
		Vec2D::Vec2D gravity = { 0, 20 };
		Particle::apply_Force(&World::emitter, &gravity);

		Particle::update_Vel_and_Life(&World::emitter, current_time, dt);
		Particle::update_Pos(&World::emitter, current_time, dt);
		
	}

	void draw(unsigned int current_time)
	{

		SDL_RenderClear(Engine::renderer);

		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 1;
		Grid_Camera::calibrate(&World::camera);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::background_map, Engine::renderer);
		Tileset::draw_Grid(&World::tileset, &World::camera, &World::tile_map, Engine::renderer);
		
		int active_player_count = Actor::draw(&World::player, &World::camera, current_time, Engine::renderer);
		int active_bullet_count = Actor::draw(&World::bullet, &World::camera, current_time, Engine::renderer);
		int active_enemy_count = Actor::draw(&World::enemy, &World::camera, current_time, Engine::renderer);

		int active_particle_count = Particle::draw(&World::emitter, &World::camera, current_time, Engine::renderer);
	
		static char str_stats[512];

		sprintf(str_stats, "n_player_actors %d\nn_bullet_actors %d\nn_particles %d\nn_enemies %d\n", active_player_count, active_bullet_count, active_particle_count, active_enemy_count);
		Font::set_Screen_Pos(&World::text, 5, 5);
		Font::draw(&World::text, str_stats, strlen(str_stats), &World::camera, Engine::renderer);
	
		Vec2D::Vec2D *p = Actor::get_Vel(World::player_actor_id, &World::player);
		sprintf(str_stats, "player vel %.4f %.4f\n", p->x, p->y);
		Font::draw(&World::text, str_stats, strlen(str_stats), &World::camera, Engine::renderer);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}
}