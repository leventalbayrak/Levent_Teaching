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
			float bullet_force_magnitude = 500;
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

	}

	void init(int screen_w, int screen_h)
	{
		srand(time(0));
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Font::init(&World::text, "font_tileset.txt", Engine::renderer);
		
		Tileset::init(&World::tileset, "map_tileset.txt", Engine::renderer);

		Grid::load(&World::map, "sandbox_map.csv");
		Grid::load(&World::collision, "sandbox_collision.csv");
		Grid::init(&World::imprint, World::map.n_rows, World::map.n_cols);

		Actor::init(&World::player, 2);
		Actor::init(&World::enemy, 512);
		Actor::init(&World::bullet, 1024);

		Actor::add_Animated_Sprite(&World::player, "saitama_pink_run.txt", Engine::renderer);
		Actor::add_Animated_Sprite(&World::enemy, "saitama_pink_run.txt", Engine::renderer);
		Actor::add_Animated_Sprite(&World::bullet, "box.txt", Engine::renderer);

		Particle::init(&World::bullet_spark, "box.txt", 1000, Engine::renderer);

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
	
	}

	void begin_Play(unsigned int current_time)
	{
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = 16;
		World::camera.world_coord.h = 16 * Engine::screen_height/Engine::screen_width;

		int player_id = Actor::spawn(&World::player, 1.0, current_time);		
		Actor::set_Pos(player_id, 4, 4, &World::player);
		

		int enemy_id = Actor::spawn(&World::enemy, 1.0, current_time);
		Actor::set_Pos(enemy_id, 6, 6, &World::enemy);
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
			Vec2D::Vec2D force = { -250,0 };
			Actor::add_Force(0, &World::player, &force);
		}
		if (Input::keys[SDL_SCANCODE_D])
		{
			Vec2D::Vec2D force = { 250,0 };
			Actor::add_Force(0, &World::player, &force);
		}
		if (Input::keys[SDL_SCANCODE_W])
		{
			Vec2D::Vec2D force = { 0,-250 };
			Actor::add_Force(0, &World::player, &force);
		}
		if (Input::keys[SDL_SCANCODE_S])
		{
			Vec2D::Vec2D force = { 0,250 };
			Actor::add_Force(0, &World::player, &force);
		}

		if (Input::mouse_left == 1 && Input::prev_mouse_left == 1)
		{
			int bullet_id = Actor::spawn(&World::bullet, 0.25, current_time);
			Shape::Rect::Data *player_rect = Actor::get_World_Coord(0, &World::player);
			Actor::set_Pos(bullet_id, player_rect->x + player_rect->w*0.5, player_rect->y + player_rect->h*0.5, &World::bullet);

			Vec2D::Vec2D force = World::mouse_grid_point;
			Vec2D::sub(&force, Actor::get_Pos(bullet_id, &World::bullet));
			Vec2D::norm(&force);
			force.x *= World::Parameters::bullet_force_magnitude;
			force.y *= World::Parameters::bullet_force_magnitude;
			Actor::add_Force(bullet_id, &World::bullet, &force);
		}

		Actor::update_Vel(0, &World::player, dt);
		Actor::get_Vel(0, &World::player)->x *= 0.95;
		Actor::get_Vel(0, &World::player)->y *= 0.95;
		Vec2D::clip(Actor::get_Vel(0, &World::player), -3.0, 3.0, -3.0, 3.0);
		Actor::update_Pos(0, &World::player, dt);

		int len = World::imprint.n_rows*World::imprint.n_cols;
		for (int i = 0; i < len; i++)
		{
			World::imprint.data[i] = -1;
		}

		for (int i = 0; i < World::enemy.array_size; i++)
		{
			if (Actor::is_Spawned(i, &World::enemy) == 0) continue;

			Actor::update_Vel(i, &World::enemy, dt);
			Vec2D::clip(Actor::get_Vel(i, &World::enemy), -3.0, 3.0, -3.0, 3.0);

			Actor::update_Pos(i, &World::enemy, dt);

			Vec2D::Vec2D pos = *Actor::get_Pos(i, &World::enemy);
			Vec2D::clip(&pos, 0, World::map.n_cols - 1, 0, World::map.n_rows - 1);
			Actor::set_Pos(i, pos.x, pos.y, &World::enemy);
			
			Grid::imprint_Set(&World::imprint, i, Actor::get_World_Coord(i, &World::enemy));
		}

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
							Shape::Rect::Data this_tile = { x,y,1.0,1.0 };
							Vec2D::Vec2D *bullet_vel = Actor::get_Vel(i, &World::bullet);
							for (int k = 0; k < 5; k++)
							{
								Collision::impulse(bullet_world, bullet_vel, 1.0, &this_tile);
							}
						}
					}

					//there is a possible collision with the enemy
					if (Grid::get_Tile(x, y, &World::imprint) != -1)
					{

						int enemy_id = Grid::get_Tile(x, y, &World::imprint);
						Shape::Rect::Data *bullet_world = Actor::get_World_Coord(i, &World::bullet);
						Shape::Rect::Data *enemy_world = Actor::get_World_Coord(enemy_id, &World::enemy);
						int r = Shape::Rect::collision(bullet_world,enemy_world);

						if (r == 1)
						{
							Vec2D::Vec2D *bullet_vel = Actor::get_Vel(i, &World::bullet);
							Vec2D::Vec2D *enemy_vel = Actor::get_Vel(enemy_id, &World::enemy);
							for (int k = 0; k < 5; k++)
							{
								Collision::impulse(bullet_world, bullet_vel, 1.0, enemy_world, enemy_vel, 1.0);
							}
						}
					}

				}
			}

			Actor::update_Pos(i, &World::bullet, dt);

		
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