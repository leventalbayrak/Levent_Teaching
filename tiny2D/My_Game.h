#pragma once

#include "include/Engine_core.h"

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
			int idle_animation_database_id;

			int run_animation_instance;
			int idle_animation_instance;
		}
	}

	namespace World
	{
		

		namespace Input
		{
			int left_x;
			int left_y;
			int right_x;
			int right_y;
		}

		namespace Parameters
		{

		}

		Shape::Rect player_world_coord;
		Shape::Rect crosshair_world_coord;

		Grid_Camera::Grid_Camera camera;
		Grid::Grid tile_map;
		Grid::Grid collision_map;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Engine::E_Sprite::add("saitama_pink.txt");

		Assets::Animation::run_animation_database_id = 0;
		Assets::Animation::idle_animation_database_id = 1;

		Assets::Animation::run_animation_instance = Engine::E_Sprite::make_Instance(0);
		Assets::Animation::idle_animation_instance = Engine::E_Sprite::make_Instance(1);

		Engine::E_Sprite::modify(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance, 100);

		Engine::E_Tileset::add("map_tileset.txt");

		
		Grid::load(&World::tile_map, "twinstick_tilemap.csv");

		
		Grid::load(&World::collision_map, "twinstick_collision.csv");


		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.w = 20;
		World::camera.world_coord.h = 15;

	}

	void begin_Play()
	{

		World::player_world_coord.x = 50;
		World::player_world_coord.y = 50;
		World::player_world_coord.w = 1;
		World::player_world_coord.h = 1;


		World::crosshair_world_coord.x = 50;
		World::crosshair_world_coord.y = 50;
		World::crosshair_world_coord.w = 1;
		World::crosshair_world_coord.h = 1;
	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		//game controller input
		World::Input::left_x = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX];
		World::Input::left_y = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTY];
		World::Input::right_x = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_RIGHTX];
		World::Input::right_y = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_RIGHTY];

		if (World::Input::left_x < -6000)
		{
			World::player_world_coord.x -= 0.1;
		}
		if (World::Input::left_x > 6000)
		{
			World::player_world_coord.x += 0.1;
		}
		if (World::Input::left_y < -6000)
		{
			World::player_world_coord.y -= 0.1;
		}
		if (World::Input::left_y > 6000)
		{
			World::player_world_coord.y += 0.1;
		}

		if (World::Input::right_x < -6000)
		{
			World::crosshair_world_coord.x -= 0.1;
		}
		if (World::Input::right_x > 6000)
		{
			World::crosshair_world_coord.x += 0.1;
		}
		if (World::Input::right_y < -6000)
		{
			World::crosshair_world_coord.y -= 0.1;
		}
		if (World::Input::right_y > 6000)
		{
			World::crosshair_world_coord.y += 0.1;
		}


		//update animations
		Engine::E_Sprite::update(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance, current_time);
	}

	void draw()
	{

		SDL_RenderClear(Engine::renderer);

		World::camera.world_coord.w = 8+1.6*abs(World::player_world_coord.x - World::crosshair_world_coord.x);
		World::camera.world_coord.h = 6+1.2*abs(World::player_world_coord.y - World::crosshair_world_coord.y);
		World::camera.world_coord.x = World::player_world_coord.x - World::camera.world_coord.w / 2;
		World::camera.world_coord.y = World::player_world_coord.y - World::camera.world_coord.h / 2;

		Grid::Region region;
		Grid::get_Region_Under_Shape(&region, &World::camera.world_coord);

		//redundant region!!!!
		Grid_Camera::Calibration calibration;
		Grid_Camera::calibrate(&calibration, &World::camera, &region);

		Engine::E_Tileset::draw(0, &calibration, &region, &World::tile_map);

		Engine::E_Sprite::draw(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance,
			&World::player_world_coord,
			0,
			&calibration,
			&World::camera
		);


		Engine::E_Sprite::draw(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance,
			&World::crosshair_world_coord,
			0,
			&calibration,
			&World::camera
		);


		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}
}