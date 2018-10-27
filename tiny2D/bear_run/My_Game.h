#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "Heap_Example.h"

namespace My_Game
{

	namespace Command
	{

	}

	namespace Assets
	{
		namespace Animation
		{
			int run_animation_database_id;
			int *run_animation_instance;

		}
	}

	namespace World
	{
		
		namespace Parameters
		{
			int max_n_runners = 512;
			float min_run_speed = 0.08;
			float max_run_speed = 0.1;

			unsigned int unfortunate_event_frequency = 2000;
		}

		unsigned int last_unfortunate_event_time = 0;
		int last_unfortunate_runner = 0;

		Shape::Rect *runner_world_coord;
		Vec2D::Vec2D *runner_velocities;
		RGBA::RGBA *runner_colors;

		Grid_Camera::Grid_Camera camera;
		Grid::Grid tile_map;
		Grid::Grid background_map;

		Min_Heap::Min_Heap heap;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Engine::E_Sprite::add("saitama_pink.txt");
	
		Assets::Animation::run_animation_database_id = 0;

		Assets::Animation::run_animation_instance = new int[World::Parameters::max_n_runners];
		for (int i = 0; i < World::Parameters::max_n_runners; i++)
		{
			Assets::Animation::run_animation_instance[i] = Engine::E_Sprite::make_Instance(0);
			Engine::E_Sprite::modify(Assets::Animation::run_animation_database_id,
				Assets::Animation::run_animation_instance[i], 100);

		}

		Engine::E_Tileset::add("map_tileset.txt");

		Grid::load(&World::tile_map, "bear_run_tilemap.csv");
		Grid::load(&World::background_map, "bear_run_tilemap_background.csv");

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.w = 20;
		World::camera.world_coord.h = 15;

		World::runner_world_coord = new Shape::Rect[World::Parameters::max_n_runners];
		World::runner_velocities = new Vec2D::Vec2D[World::Parameters::max_n_runners];
		World::runner_colors = new RGBA::RGBA[World::Parameters::max_n_runners];

		Min_Heap::init(&World::heap, 1024);

	}

	void begin_Play()
	{

		for (int i = 0; i < World::Parameters::max_n_runners; i++)
		{
			World::runner_world_coord[i].x = 1;
			World::runner_world_coord[i].y = 14;
			World::runner_world_coord[i].w = 1;
			World::runner_world_coord[i].h = 1;

			double vel = World::Parameters::min_run_speed + (World::Parameters::max_run_speed - World::Parameters::min_run_speed)*(double)rand() / RAND_MAX;
			World::runner_velocities[i].x = vel;
			World::runner_velocities[i].y = 0;

			World::runner_colors[i].r = 200 + rand() % 56;
			World::runner_colors[i].g = 200 + rand() % 56;
			World::runner_colors[i].b = 200 + rand() % 56;
			World::runner_colors[i].a = 255;

			float priority = 1.0 / World::runner_velocities[i].x;
			Min_Heap::add(&World::heap, priority, i);
		}

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	
		float max_vel_x = 0.0;
		int max_vel_x_index = 0;
		for (int i = 0; i < World::Parameters::max_n_runners; i++)
		{
			World::runner_world_coord[i].x += World::runner_velocities[i].x;
			
			Engine::E_Sprite::update(Assets::Animation::run_animation_database_id,
				Assets::Animation::run_animation_instance[i], current_time);

			if (World::runner_velocities[i].x > max_vel_x)
			{
				max_vel_x = World::runner_velocities[i].x;
				max_vel_x_index = i;
			}
			
		}
		
		World::camera.world_coord.x = World::runner_world_coord[max_vel_x_index].x -World::camera.world_coord.w/2;
		World::camera.world_coord.y = 1;

		if (current_time - World::last_unfortunate_event_time >= World::Parameters::unfortunate_event_frequency)
		{
			World::last_unfortunate_event_time = current_time;

			
			{
				int which_runner = 0;
				float old_priority;
				Min_Heap::remove(old_priority, which_runner, &World::heap);

				World::last_unfortunate_runner = which_runner;

				double vel = World::Parameters::min_run_speed + (World::Parameters::max_run_speed - World::Parameters::min_run_speed)*(double)rand() / RAND_MAX;
				World::runner_velocities[which_runner].x = vel;
				World::runner_world_coord[which_runner].y = 12;
				World::runner_colors[which_runner].r = 255;
				World::runner_colors[which_runner].b = 0;

				float new_priority = 1.0 / World::runner_velocities[which_runner].x;
				Min_Heap::add(&World::heap, new_priority, which_runner);
			}
						
			{
				int which_runner = 0;
				float old_priority;
				World::heap.n_data--;
				which_runner = World::heap.data[World::heap.n_data];

				float vel = World::Parameters::min_run_speed + (World::Parameters::max_run_speed - World::Parameters::min_run_speed)*(double)rand() / RAND_MAX;
				World::runner_velocities[which_runner].x = vel;
				World::runner_world_coord[which_runner].y = 4;

				float new_priority = 1.0 / World::runner_velocities[which_runner].x;
				Min_Heap::add(&World::heap, new_priority, which_runner);
				World::runner_colors[which_runner].r = 0;
				World::runner_colors[which_runner].b = 255;
			}
			
			
		}
		
	}

	void draw()
	{

		SDL_RenderClear(Engine::renderer);

		Grid_Camera::calibrate(&World::camera);

		Engine::E_Tileset::draw(0, &World::camera, &World::background_map);
		Engine::E_Tileset::draw(0, &World::camera, &World::tile_map);
		
		for (int i = 0; i < World::Parameters::max_n_runners; i++)
		{
			
			Engine::E_Sprite::draw(Assets::Animation::run_animation_database_id,
				Assets::Animation::run_animation_instance[i],
				&World::runner_world_coord[i],
				0,
				&World::camera,
				&World::runner_colors[i]
			);
		}

		Engine::E_Sprite::draw(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance[World::last_unfortunate_runner],
			&World::runner_world_coord[World::last_unfortunate_runner],
			0,
			&World::camera,
			&World::runner_colors[World::last_unfortunate_runner]
		);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}
}