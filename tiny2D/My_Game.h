#pragma once

//BEAR RUN

#include "include/Engine_core.h"
#include "Heap_Example.h"
#include "include/Actor_core.h"

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
			int max_n_runners = 512;
			float min_run_speed = 0.08;
			float max_run_speed = 0.1;

			unsigned int unfortunate_event_frequency = 2000;
		}

		Grid_Camera::Grid_Camera camera;
		Grid::Grid tile_map;
		Grid::Grid background_map;

		Actor::Factory saitama;

		Min_Heap::Min_Heap heap;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Engine::E_Tileset::add("map_tileset.txt");

		Grid::load(&World::tile_map, "bear_run_tilemap.csv");
		Grid::load(&World::background_map, "bear_run_tilemap_background.csv");

		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.w = 20;
		World::camera.world_coord.h = 15;

		Actor::init(&World::saitama, 1000);
		Actor::add(&World::saitama, "saitama_pink_run.txt", Engine::renderer);

	}

	void begin_Play()
	{
		for (int i = 0; i < 1000; i++)
		{
			int k = Actor::make_Instance(&World::saitama);
			Actor::set_Pos(k, 0, World::camera.world_coord.h-1, &World::saitama);
		}
		printf("n_actors = %d\n", World::saitama.n_actors);
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();
	
		//add forces to actors

		//after done adding all forces
		//execute the loop below

		for (int i = 0; i < World::saitama.n_actors; i++)
		{
			//integrate forces
			Actor::update_Vel(i, &World::saitama, dt);

			//check for collisions
			//to prevent a collision, set the velocity of the collision axis to zero


			//update position
			Actor::update_Pos(i, &World::saitama, dt);
	
			//zero the forces
			World::saitama.bodies.force[i] = {};
		}

		for (int i = 0; i < World::saitama.n_actors; i++)
		{
			Actor::update_Sprite_Frame(i, &World::saitama, current_time);
		}
		
	}

	void draw()
	{

		SDL_RenderClear(Engine::renderer);

		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		Grid_Camera::calibrate(&World::camera);

		Engine::E_Tileset::draw(0, &World::camera, &World::background_map);
		Engine::E_Tileset::draw(0, &World::camera, &World::tile_map);
		
		for (int i = 0; i < World::saitama.n_actors; i++)
		{
			Actor::draw(i, &World::saitama, 0, &RGBA::default,&World::camera, Engine::renderer);
		}
		
		
		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}
}