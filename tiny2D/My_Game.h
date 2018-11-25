#pragma once

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"

namespace My_Game
{

	namespace World
	{

		namespace Parameters
		{
			float bullet_force_magnitude = 600;
		}


		Tileset::Tileset tileset;
		Font::Font text;
		
		Grid::Grid map;
		Grid::Grid collision;
		Grid::Grid imprint;

		Actor::Factory player;
		Actor::Factory enemy;
		Actor::Factory bullet;
		Particle::Emitter bullet_spark;

		Grid_Camera::Grid_Camera camera;

		Vec2D::Vec2D mouse_grid_point;

		unsigned int last_bullet_time = 0;
		unsigned int bullet_freq = 60;

	}

	void init(int screen_w, int screen_h)
	{
		srand(time(0));
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		
		Audio::add_FX("marble.wav");
		Audio::add_FX("bullet.wav");
		Audio::add_FX("rico.wav");
		Audio::set_FX_Volume(0, 128);
		Audio::set_FX_Volume(1, 80);
		Audio::set_FX_Volume(2, 80);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::load(&World::map, "sandbox_map.csv");
		Grid::load(&World::collision, "sandbox_collision.csv");
		Grid::init(&World::imprint, World::map.n_rows, World::map.n_cols);

		Actor::init(&World::player, 2);
		Actor::init(&World::enemy, 512);
		Actor::init(&World::bullet, 1024);

		Actor::add_Animated_Sprite(&World::player, "saitama_pink_run.txt", Engine::renderer);
		Actor::add_Animated_Sprite(&World::enemy, "box.txt", Engine::renderer);
		Actor::add_Animated_Sprite(&World::bullet, "box.txt", Engine::renderer);

		Particle::init(&World::bullet_spark, "dirt.txt", 4096, Engine::renderer);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
	
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 32;
		World::camera.world_coord.h = 32 * Engine::screen_height/Engine::screen_width;

		int player_id = Actor::spawn(&World::player, 1.0, current_time);		
		Actor::set_Pos(player_id, 4, 4, &World::player);
		
		for (int i = 0; i < 64; i++)
		{

			double x = 1.0 + (World::map.n_cols - 2)*rand() / RAND_MAX;
			double y = 1.0 + (World::map.n_rows - 2)*rand() / RAND_MAX;
			int enemy_id = Actor::spawn(&World::enemy, 0.25, current_time);
			Actor::set_Pos(enemy_id, x, y, &World::enemy);

		}
	
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		World::camera.world_coord.x = Actor::get_World_Coord(0, &World::player)->x - 0.5*World::camera.world_coord.w;
		World::camera.world_coord.y = Actor::get_World_Coord(0, &World::player)->y - 0.5*World::camera.world_coord.h;

		if (World::camera.world_coord.x < 0) World::camera.world_coord.x = 0;
		if (World::camera.world_coord.y < 0) World::camera.world_coord.y = 0;
		if (World::camera.world_coord.x >= World::map.n_cols - World::camera.world_coord.w) World::camera.world_coord.x = World::map.n_cols - World::camera.world_coord.w;
		if (World::camera.world_coord.y >= World::map.n_rows - World::camera.world_coord.h) World::camera.world_coord.y = World::map.n_rows - World::camera.world_coord.h;

		Grid_Camera::calibrate(&World::camera);
		Grid_Camera::screen_to_Grid(&World::mouse_grid_point, Input::mouse_x, Input::mouse_y, &World::camera);
		
		if (Input::keys[SDL_SCANCODE_A])
		{
			Vec2D::Vec2D force = { -450,0 };
			Actor::add_Force(0, &World::player, &force);
		}
		if (Input::keys[SDL_SCANCODE_D])
		{
			Vec2D::Vec2D force = { 450,0 };
			Actor::add_Force(0, &World::player, &force);
		}
		if (Input::keys[SDL_SCANCODE_W])
		{
			Vec2D::Vec2D force = { 0,-450 };
			Actor::add_Force(0, &World::player, &force);
		}
		if (Input::keys[SDL_SCANCODE_S])
		{
			Vec2D::Vec2D force = { 0,450 };
			Actor::add_Force(0, &World::player, &force);
		}

		if (Input::mouse_left == 1 && Input::prev_mouse_left == 1)
		{
			if (current_time - World::last_bullet_time > World::bullet_freq)
			{
				World::last_bullet_time = current_time;

				int bullet_id = Actor::spawn(&World::bullet, 0.25, current_time);
				Shape::Rect::Data *player_rect = Actor::get_World_Coord(0, &World::player);
				Actor::set_Pos(bullet_id, player_rect->x + player_rect->w*0.5, player_rect->y + player_rect->h*0.5, &World::bullet);
				Vec2D::Vec2D *bullet_pos = Actor::get_Pos(bullet_id, &World::bullet);

				Vec2D::Vec2D force = World::mouse_grid_point;
				Vec2D::sub(&force, bullet_pos);

				Vec2D::norm(&force);

				Vec2D::scale(&force, World::Parameters::bullet_force_magnitude);
				Actor::add_Force(bullet_id, &World::bullet, &force);

				Audio::queue_FX(1);
			}
		}

		Actor::update_Vel(0, &World::player, dt);
		Actor::get_Vel(0, &World::player)->x *= 0.95;
		Actor::get_Vel(0, &World::player)->y *= 0.95;
		Vec2D::clip(Actor::get_Vel(0, &World::player), -6.0, 6.0, -6.0, 6.0);
		Actor::update_Pos(0, &World::player, dt);

		int len = World::imprint.n_rows*World::imprint.n_cols;
		for (int i = 0; i < len; i++)
		{
			World::imprint.data[i] = -1;
		}

#pragma region ENEMY
		{

			//flocking behavior forces
			Vec2D::Vec2D avg_vel = {};
			Vec2D::Vec2D avg_pos = {};
			int n_active = 0;
			for (int i = 0; i < World::enemy.array_size; i++)
			{
				if (Actor::is_Spawned(i, &World::enemy) == 0) continue;

				Vec2D::add(&avg_vel, Actor::get_Vel(i, &World::enemy));
				Vec2D::add(&avg_pos, Actor::get_Pos(i, &World::enemy));
				n_active++;
			}
			Vec2D::scale(&avg_vel, 1.0 / n_active);
			Vec2D::scale(&avg_pos, 1.0 / n_active);

			for (int i = 0; i < World::enemy.array_size; i++)
			{
				if (Actor::is_Spawned(i, &World::enemy) == 0) continue;

				Vec2D::Vec2D force = {};

				Vec2D::Vec2D v_force = avg_vel;
				Vec2D::norm(&v_force);
				Vec2D::scale(&v_force, 6.0);

				Vec2D::Vec2D p_force = avg_pos;
				Vec2D::sub(&p_force, Actor::get_Pos(i, &World::enemy));
				Vec2D::norm(&p_force);
				Vec2D::scale(&p_force, 8.0);

				Vec2D::add(&force, &v_force);
				Vec2D::add(&force, &p_force);
				
				Actor::add_Force(i, &World::enemy, &force);

				for (int j = i + 1; j < World::enemy.array_size; j++)
				{
					if (Actor::is_Spawned(j, &World::enemy) == 0) continue;

					Vec2D::Vec2D c_force = *Actor::get_Pos(i, &World::enemy);
					Vec2D::sub(&c_force, Actor::get_Pos(j, &World::enemy));

					if (c_force.x*c_force.x + c_force.y*c_force.y >= 2) continue;

					Vec2D::norm(&c_force);
					Vec2D::scale(&c_force, 2024.0/n_active);

					Actor::add_Force(i, &World::enemy, &c_force);
					Vec2D::scale(&c_force, -1.0);
					Actor::add_Force(j, &World::enemy, &c_force);
				}
				
			}

			for (int i = 0; i < World::enemy.array_size; i++)
			{
				if (Actor::is_Spawned(i, &World::enemy) == 0) continue;

				//	Vec2D::Vec2D gravity = { 0,4 };
				//	Actor::add_Force(i, &World::enemy, &gravity);
				Actor::update_Vel(i, &World::enemy, dt);
				Actor::get_Vel(i, &World::enemy)->x *= 0.95;
				Actor::get_Vel(i, &World::enemy)->y *= 0.95;
				Vec2D::clip(Actor::get_Vel(i, &World::enemy), -16.0, 16.0, -16.0, 16.0);

				//ENEMY COLLISION
				Grid::Region enemy_imprint_region;
				Actor::get_Grid_Collision(&enemy_imprint_region, &World::imprint, i, &World::enemy);

				for (int y = enemy_imprint_region.first_row; y <= enemy_imprint_region.last_row; y++)
				{
					for (int x = enemy_imprint_region.first_col; x <= enemy_imprint_region.last_col; x++)
					{
						//there is a possible collision another enemy
						int e = Grid::get_Tile(x, y, &World::imprint);
						if (e != -1 && e != i)
						{
							Shape::Rect::Data *enemy_world = Actor::get_World_Coord(i, &World::enemy);
							Shape::Rect::Data *target_enemy_world = Actor::get_World_Coord(e, &World::enemy);
							int r = Shape::Rect::collision(enemy_world, target_enemy_world);

							if (r == 1)
							{
								Vec2D::Vec2D *enemy_vel = Actor::get_Vel(i, &World::enemy);
								Vec2D::Vec2D *target_enemy_vel = Actor::get_Vel(e, &World::enemy);
								for (int k = 0; k < 10; k++)
								{
									Collision::impulse(enemy_world, enemy_vel, 1.0, target_enemy_world, target_enemy_vel, 1.0);
								}

								Vec2D::Vec2D initial_vel = {};
								Vec2D::Vec2D f_min = { -50,-50 };
								Vec2D::Vec2D f_max = { 50,50 };
								Vec2D::Vec2D pos = { 0.5*(enemy_world->x + target_enemy_world->x + target_enemy_world->w), 0.5*(enemy_world->y + target_enemy_world->y + target_enemy_world->h) };
								Particle::spawn(&World::bullet_spark, 1, 0.2, &pos, &initial_vel, &f_min, &f_max, 1000, 2000, current_time);
								
								Audio::queue_FX(0);
							}
						}
					}
				}

				//WALL COLLISION
				Grid::Region region;
				Actor::get_Grid_Collision(&region, &World::collision, i, &World::enemy);

				for (int y = region.first_row; y <= region.last_row; y++)
				{
					for (int x = region.first_col; x <= region.last_col; x++)
					{
						//there is a possible collision with wall
						if (Grid::get_Tile(x, y, &World::collision) != -1)
						{
							Shape::Rect::Data *enemy_world = Actor::get_World_Coord(i, &World::enemy);
							Shape::Rect::Data this_tile = { x,y,1.0,1.0 };
							int r = Shape::Rect::collision(enemy_world, &this_tile);

							if (r == 1)
							{
								Vec2D::Vec2D *enemy_vel = Actor::get_Vel(i, &World::enemy);
								for (int k = 0; k < 10; k++)
								{
									Collision::impulse(enemy_world, enemy_vel, 1.0, &this_tile);
								}

								Audio::queue_FX(0);
							}
						}
					}
				}

				Actor::update_Pos(i, &World::enemy, dt);
				Shape::Rect::Data *enemy_world = Actor::get_World_Coord(i, &World::enemy);
				Vec2D::Vec2D pos = *Actor::get_Pos(i, &World::enemy);
				Vec2D::clip(&pos, 1, World::map.n_cols - 1, 1, World::map.n_rows - 1);
				Actor::set_Pos(i, pos.x, pos.y, &World::enemy);

				float p_size = 0.2;
				Vec2D::Vec2D initial_vel = *Actor::get_Vel(i, &World::enemy);
				Vec2D::scale(&initial_vel, -1.0);
				Vec2D::Vec2D f_min = { -40,-40 };
				Vec2D::Vec2D f_max = { 40,40 };
				Vec2D::Vec2D epos = { enemy_world->x + 0.5*enemy_world->w - p_size*0.5, enemy_world->y + 0.5*enemy_world->h - p_size * 0.5 };
				Particle::spawn(&World::bullet_spark, 1, p_size, &epos, &initial_vel, &f_min, &f_max, 500, 1000, current_time);

				Grid::imprint_Set(&World::imprint, i, Actor::get_World_Coord(i, &World::enemy));
			}
		}

#pragma region BULLETS
		{
			for (int i = 0; i < World::bullet.array_size; i++)
			{
				if (Actor::is_Spawned(i, &World::bullet) == 0) continue;

				if (current_time - World::bullet.creation_time[i] > 3000)
				{
					Actor::destroy(i, &World::bullet);

					continue;
				}

				//update vel
				Actor::update_Vel(i, &World::bullet, dt);
				Vec2D::clip(Actor::get_Vel(i, &World::bullet), -16.0, 16.0, -16.0, 16.0);

				Grid::Region region;
				Actor::get_Grid_Collision(&region, &World::collision, i, &World::bullet);

				for (int y = region.first_row; y <= region.last_row; y++)
				{
					for (int x = region.first_col; x <= region.last_col; x++)
					{
						//there is a possible collision with wall
						if (Grid::get_Tile(x, y, &World::collision) != -1)
						{

							Shape::Rect::Data *bullet_world = Actor::get_World_Coord(i, &World::bullet);
							Shape::Rect::Data this_tile = { x,y,1.0,1.0 };
							int r = Shape::Rect::collision(bullet_world, &this_tile);

							if (r == 1)
							{
								Vec2D::Vec2D *bullet_vel = Actor::get_Vel(i, &World::bullet);
								for (int k = 0; k < 10; k++)
								{
									Collision::impulse(bullet_world, bullet_vel, 1.0, &this_tile);
								}

								Audio::queue_FX(2);
							}
						}

						//there is a possible collision with the enemy
						if (Grid::get_Tile(x, y, &World::imprint) != -1)
						{

							int enemy_id = Grid::get_Tile(x, y, &World::imprint);
							Shape::Rect::Data *bullet_world = Actor::get_World_Coord(i, &World::bullet);
							Shape::Rect::Data *enemy_world = Actor::get_World_Coord(enemy_id, &World::enemy);
							int r = Shape::Rect::collision(bullet_world, enemy_world);

							if (r == 1)
							{
								Vec2D::Vec2D *bullet_vel = Actor::get_Vel(i, &World::bullet);
								Vec2D::Vec2D *enemy_vel = Actor::get_Vel(enemy_id, &World::enemy);
								for (int k = 0; k < 10; k++)
								{
									Collision::impulse(bullet_world, bullet_vel, 1.0, enemy_world, enemy_vel, 1.0);
								}

								Vec2D::Vec2D initial_vel = {};
								Vec2D::Vec2D f_min = { -50,-50 };
								Vec2D::Vec2D f_max = { 50,50 };
								Vec2D::Vec2D pos = { enemy_world->x + 0.5*enemy_world->w, enemy_world->y + 0.5*enemy_world->h };
								Particle::spawn(&World::bullet_spark, 1, 0.125, &pos, &initial_vel, &f_min, &f_max, 500, 2000, current_time);
								Actor::destroy(i, &World::bullet);

								Audio::queue_FX(2);

							}
						}

					}
				}

				Actor::update_Pos(i, &World::bullet, dt);
			}
		}

		Particle::update_Vel_and_Life(&World::bullet_spark, current_time, dt);
		Particle::update_Pos(&World::bullet_spark, current_time, dt);

	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);

		Tileset::draw_Grid(&World::tileset, &World::camera, &World::map, &RGBA::default, Engine::renderer);

		

		Actor::draw(&World::player, &World::camera, current_time, Engine::renderer);
		Actor::draw(&World::enemy, &World::camera, current_time, Engine::renderer);
		Actor::draw(&World::bullet, &World::camera, current_time, Engine::renderer);
		Particle::draw(&World::bullet_spark, &World::camera, current_time, Engine::renderer);
		

		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

}