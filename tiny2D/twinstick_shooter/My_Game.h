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
			int crosshair_animation_database_id;
			int bullet_animation_database_id;

			int run_animation_instance;
			int idle_animation_instance;
			int crosshair_animation_instance;


			int *bullet_animation_instance;
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

			float crosshair_distance_from_player = 5.0;
			int max_n_bullets = 512;
			float bullet_inactivation_distance = 256.0;

		}

		Shape::Rect player_world_coord;
		Shape::Rect crosshair_world_coord;
		Shape::Rect *bullet_world_coord;
		int *bullet_state;
		
		Vec2D::Vec2D player_velocity;
		Vec2D::Vec2D *bullet_velocities;

		Grid_Camera::Grid_Camera camera;
		Grid::Grid tile_map;
		Grid::Grid collision_map;

		Vec2D::Vec2D right_stick_direction;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);

		Engine::E_Sprite::add("saitama_pink.txt");
		Engine::E_Sprite::add("crosshair.txt");

		Assets::Animation::run_animation_database_id = 0;
		Assets::Animation::idle_animation_database_id = 1;
		Assets::Animation::crosshair_animation_database_id = 3;
		Assets::Animation::bullet_animation_database_id = 0;

		Assets::Animation::run_animation_instance = Engine::E_Sprite::make_Instance(0);
		Assets::Animation::idle_animation_instance = Engine::E_Sprite::make_Instance(1);

		Assets::Animation::crosshair_animation_instance = Engine::E_Sprite::make_Instance(3);

		Assets::Animation::bullet_animation_instance = new int[World::Parameters::max_n_bullets];
		for (int i = 0; i < World::Parameters::max_n_bullets; i++)
		{
			Assets::Animation::bullet_animation_instance[i] = Engine::E_Sprite::make_Instance(0);
		}

		Engine::E_Sprite::modify(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance, 100);
		Engine::E_Sprite::modify(Assets::Animation::crosshair_animation_database_id,
			Assets::Animation::crosshair_animation_instance, 100);

		Engine::E_Tileset::add("map_tileset.txt");

		
		Grid::load(&World::tile_map, "twinstick_tilemap.csv");

		
		Grid::load(&World::collision_map, "twinstick_collision.csv");


		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.w = 20;
		World::camera.world_coord.h = 15;


		World::bullet_world_coord = new Shape::Rect[World::Parameters::max_n_bullets];
		World::bullet_state = new int [World::Parameters::max_n_bullets];
		World::bullet_velocities = new Vec2D::Vec2D[World::Parameters::max_n_bullets];
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

		for (int i = 0; i < World::Parameters::max_n_bullets; i++)
		{
			World::bullet_world_coord[i].x = 0;
			World::bullet_world_coord[i].y = 0;
			World::bullet_world_coord[i].w = 1;
			World::bullet_world_coord[i].h = 1;

			World::bullet_state[i] = 0;

			World::bullet_velocities[i] = {};
		}

	}

	void update(unsigned int current_time, float dt)
	{

		Engine::event_Loop();

		//game controller input
		World::Input::left_x = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX];
		World::Input::left_y = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTY];
		World::Input::right_x = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_RIGHTX];
		World::Input::right_y = Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_RIGHTY];

		World::player_velocity = {};

		if (World::Input::left_x < -6000)
		{
			World::player_velocity.x = -0.1;
		}
		if (World::Input::left_x > 6000)
		{
			World::player_velocity.x = 0.1;
		}
		if (World::Input::left_y < -6000)
		{
			World::player_velocity.y = -0.1;
		}
		if (World::Input::left_y > 6000)
		{
			World::player_velocity.y = 0.1;
		}

		int shoot_bullet = 0;
		if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_TRIGGERRIGHT] > 0)
		{
			shoot_bullet = 1;
		}

		if (World::Input::right_x > -6000 && World::Input::right_x < 6000)
		{
			if (World::Input::right_y > -6000 && World::Input::right_y < 6000)
			{
				World::Input::right_x = 0;
				World::Input::right_y = 0;
			}
			
		}
	
		World::right_stick_direction = {};

		double mag = sqrt(World::Input::right_x*World::Input::right_x + World::Input::right_y*World::Input::right_y);
		if (mag != 0)
		{
			World::right_stick_direction.x = World::Input::right_x / mag;
			World::right_stick_direction.y = World::Input::right_y / mag;
		}
		

		World::crosshair_world_coord.x = World::player_world_coord.x + World::right_stick_direction.x*World::Parameters::crosshair_distance_from_player;
		World::crosshair_world_coord.y = World::player_world_coord.y + World::right_stick_direction.y*World::Parameters::crosshair_distance_from_player;

		World::player_world_coord.x += World::player_velocity.x;
		World::player_world_coord.y += World::player_velocity.y;

		if (shoot_bullet == 1 && mag != 0)
		{

			for (int i = 0; i < World::Parameters::max_n_bullets; i++)
			{
				if (World::bullet_state[i] == 0)
				{
					World::bullet_state[i] = 1;
					World::bullet_world_coord[i].x = World::player_world_coord.x;
					World::bullet_world_coord[i].y = World::player_world_coord.y;
					
					World::bullet_velocities[i].x = World::player_velocity.x + World::right_stick_direction.x * 0.5;
					World::bullet_velocities[i].y = World::player_velocity.y + World::right_stick_direction.y * 0.5;
					
					break;
				}
			}
			
		}


		//update bullet pos
		for (int i = 0; i < World::Parameters::max_n_bullets; i++)
		{
			if (World::bullet_state[i] == 1)
			{
				Vec2D::Vec2D a = { World::bullet_world_coord[i].x, World::bullet_world_coord[i].y };
				Vec2D::Vec2D b = { World::player_world_coord.x, World::player_world_coord.y };
				if (Vec2D::dist_Squared(&a,&b) > World::Parameters::bullet_inactivation_distance)
				{
					World::bullet_state[i] = 0;
					continue;
				}
				World::bullet_world_coord[i].x += World::bullet_velocities[i].x;
				World::bullet_world_coord[i].y += World::bullet_velocities[i].y;
			}
		}


		//update animations
		Engine::E_Sprite::update(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance, current_time);
		Engine::E_Sprite::update(Assets::Animation::crosshair_animation_database_id,
			Assets::Animation::crosshair_animation_instance, current_time);
	}

	void draw()
	{

		SDL_RenderClear(Engine::renderer);

		World::camera.world_coord.x = World::player_world_coord.x - World::camera.world_coord.w / 2;
		World::camera.world_coord.y = World::player_world_coord.y - World::camera.world_coord.h / 2;

		
		Grid_Camera::calibrate(&World::camera);

		Engine::E_Tileset::draw(0, &World::camera, &World::tile_map);

		Engine::E_Sprite::draw(Assets::Animation::run_animation_database_id,
			Assets::Animation::run_animation_instance,
			&World::player_world_coord,
			0,
			&World::camera,
			&RGBA::default
		);


		Engine::E_Sprite::draw(Assets::Animation::crosshair_animation_database_id,
			Assets::Animation::crosshair_animation_instance,
			&World::crosshair_world_coord,
			0,
			&World::camera,
			&RGBA::default
		);

		int counter = 0;
		for (int i = 0; i < World::Parameters::max_n_bullets; i++)
		{
			if (World::bullet_state[i] == 1)
			{
				counter++;
				RGBA::RGBA rgba = RGBA::default;
				
				rgba.r = rand()%256;
				rgba.g = rand() % 256;
				rgba.b = rand() % 256;
				rgba.a = rand() % 256;

				Engine::E_Sprite::draw(Assets::Animation::run_animation_database_id,
					Assets::Animation::run_animation_instance,
					&World::bullet_world_coord[i],
					0,
					&World::camera,
					&rgba
				);
			}
		}

		//printf("number of active bullets %d\n", counter);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}
}