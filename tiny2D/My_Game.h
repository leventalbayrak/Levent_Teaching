#pragma once

#include "include/Engine_core.h"

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
		int prev_move_cmd_left;
		int prev_move_cmd_right;
		int prev_move_cmd_up;
		int prev_move_cmd_down;

		Shape::Rect world_coord;

		float move_force_mag;
		float jump_force_mag;

		int physics_body;
	};

	void init_Actor_Assets(Actor *p, int idle_sprite_index, int run_sprite_index, int jump_sprite_index);
	void update_Actor(Actor *p, unsigned int current_time);
	void update_Actor_Physics(Actor *p, unsigned int current_time, float dt);
	void draw_Actor(Actor *p);
	void update_Enemy_AI(Actor *e, unsigned int current_time);
	void update_Map_Events(Actor *e, unsigned int current_time);

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

		int enemy_run_sprite_index;
		int enemy_idle_sprite_index;
		int enemy_jump_sprite_index;
	}

	namespace World
	{

		namespace Parameters
		{
			//some parameters for the map
			Vec2D::Vec2D gravity = { 0.0, 32 };

			float max_vel_x = 24.0;
			float max_vel_y = 16.0;

			int super_jump_tile_id = 15;
			int teleport_tile_id = 12;

			const int n_enemies = 1024;
			int n_active_enemies = 8;
		}

		Actor player;

		Actor enemies[Parameters::n_enemies];

		Grid::Grid collision_map;
		Grid::Grid tilemap;
		Grid::Grid object_map;
		Grid::Grid parallax_map1;
		Grid::Grid parallax_map2;

		Grid_Camera::Grid_Camera camera;
		Grid_Camera::Calibration calibration;

		Body::Body bodies;


	}

	void init()
	{
		//initialize all systems and open game window
		Engine::init("hello side scroller", 480 * 2, 384 * 2);

		//load tileset image
		Engine::E_Tileset::add("map_tileset.txt");

		//load collision map (level collisions), "tiled" export csv
		Grid::load(&World::collision_map, "platformer_collision.csv");
		//load tile map (visuals), "tiled" export csv
		Grid::load(&World::tilemap, "platformer_map.csv");
		//load tile map objects, "tiled" export csv
		Grid::load(&World::object_map, "platformer_objects.csv");
		//load parallax maps
		Grid::load(&World::parallax_map1, "platformer_parallax_map1.csv");
		Grid::load(&World::parallax_map2, "platformer_parallax_map2.csv");

		//load player sprite
		int number_of_sprites_loaded = Engine::E_Sprite::add("saitama_pink.txt");
		printf("loaded %d frames\n", number_of_sprites_loaded);
		Assets::player_run_sprite_index = 0;
		Assets::player_idle_sprite_index = 1;
		Assets::player_jump_sprite_index = 2;

		number_of_sprites_loaded = Engine::E_Sprite::add("saitama_blue.txt");
		printf("loaded %d frames\n", number_of_sprites_loaded);
		Assets::enemy_run_sprite_index = 3;
		Assets::enemy_idle_sprite_index = 4;
		Assets::enemy_jump_sprite_index = 5;
		
		//init camera
		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.w = Engine::screen_width / 64;
		World::camera.world_coord.h = Engine::screen_height / 64;

		//create physics body manager
		Body::init(&World::bodies, World::Parameters::n_enemies);
	}

	void begin_Play()
	{
		//initialize player
		init_Actor_Assets(&World::player, Assets::player_idle_sprite_index, Assets::player_run_sprite_index, Assets::player_jump_sprite_index);
		World::player.world_coord = { 10,10,1,1 };
		World::player.jump_force_mag = 512;
		World::player.move_force_mag = 100;
		World::player.sprite_direction = 0;
		World::player.state = 0;
		World::player.physics_body = Body::make(&World::bodies);
		Vec2D::Vec2D tmp_pos = { World::player.world_coord.x,World::player.world_coord.y };
		Body::modify(World::player.physics_body, &World::bodies, &tmp_pos, 1.0);

		for (int i = 0; i < World::Parameters::n_enemies; i++)
		{
			init_Actor_Assets(&World::enemies[i], Assets::enemy_idle_sprite_index, Assets::enemy_run_sprite_index, Assets::enemy_jump_sprite_index);
			World::enemies[i].world_coord = { (float)(5 + rand() % 10),(float)(10),1,1 };
			World::enemies[i].jump_force_mag = 512;
			World::enemies[i].move_force_mag = 96;
			World::enemies[i].sprite_direction = 0;
			World::enemies[i].state = 0;
			World::enemies[i].physics_body = Body::make(&World::bodies);
			Vec2D::Vec2D tmp_pos = { World::enemies[i].world_coord.x,World::enemies[i].world_coord.y };
			Body::modify(World::enemies[i].physics_body, &World::bodies, &tmp_pos, 1.0);
		}
	}

	void update(unsigned int current_time, float dt)
	{
		
		Engine::event_Loop();

		Command::cmd_UP = 0;
		Command::cmd_DOWN = 0;
		Command::cmd_LEFT = 0;
		Command::cmd_RIGHT = 0;

		if (Engine::keys[SDL_SCANCODE_W]) Command::cmd_UP = 1;
		if (Engine::keys[SDL_SCANCODE_SPACE]) Command::cmd_UP = 1;
		if (Engine::keys[SDL_SCANCODE_A]) Command::cmd_LEFT = 1;
		if (Engine::keys[SDL_SCANCODE_D]) Command::cmd_RIGHT = 1;

		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_A]) Command::cmd_UP = 1;
		if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX] < -5000) Command::cmd_LEFT = 1;
		else if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX] > 5000) Command::cmd_RIGHT = 1;

		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_LEFT]) Command::cmd_LEFT = 1;
		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_RIGHT]) Command::cmd_RIGHT = 1;

		World::player.prev_move_cmd_left = World::player.move_cmd_left;
		World::player.prev_move_cmd_right = World::player.move_cmd_right;
		World::player.prev_move_cmd_up = World::player.move_cmd_up;
		World::player.prev_move_cmd_down = World::player.move_cmd_down;
		World::player.move_cmd_left = Command::cmd_LEFT;
		World::player.move_cmd_right = Command::cmd_RIGHT;
		World::player.move_cmd_up = Command::cmd_UP;
		World::player.move_cmd_down = Command::cmd_DOWN;

		//update player physics
		update_Map_Events(&World::player, current_time);
		update_Actor(&World::player, current_time);
		update_Actor_Physics(&World::player, current_time, dt);

		if (World::Parameters::n_active_enemies >= World::Parameters::n_enemies)
		{
			exit(0);
		}
		for (int i = 0; i < World::Parameters::n_active_enemies; i++)
		{
			update_Enemy_AI(&World::enemies[i], current_time);
			update_Map_Events(&World::enemies[i], current_time);
			update_Actor(&World::enemies[i], current_time);
			update_Actor_Physics(&World::enemies[i], current_time, dt);
		}

		//focus camera on player
		My_Game::World::camera.world_coord.x = World::player.world_coord.x - My_Game::World::camera.world_coord.w / 2;
		My_Game::World::camera.world_coord.y = World::player.world_coord.y - My_Game::World::camera.world_coord.h / 2;

	}

	void draw()
	{



		//set color to white
		SDL_SetRenderDrawColor(Engine::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Engine::renderer);

		Grid::Region grid_region;


		Grid_Camera::Grid_Camera parallax_camera;
		Grid_Camera::Calibration parallax_calibration;



		parallax_camera.init = World::camera.init;
		parallax_camera.world_coord = World::camera.world_coord;
		parallax_camera.world_coord.x *= 0.2;
		parallax_camera.world_coord.y *= 1;
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &parallax_camera);
		Grid::clip_Grid_Region(&grid_region, World::parallax_map2.n_cols, World::parallax_map2.n_rows);
		Grid_Camera::calibrate(&parallax_calibration, &parallax_camera, &grid_region);
		Engine::E_Tileset::draw(0, &parallax_calibration, &grid_region, &World::parallax_map2);

		parallax_camera.init = World::camera.init;
		parallax_camera.world_coord = World::camera.world_coord;
		parallax_camera.world_coord.x *= 0.4;
		parallax_camera.world_coord.y *= 1;
		Grid_Camera::get_Grid_Region_Covered(&grid_region, &parallax_camera);
		Grid::clip_Grid_Region(&grid_region, World::parallax_map1.n_cols, World::parallax_map1.n_rows);
		Grid_Camera::calibrate(&parallax_calibration, &parallax_camera, &grid_region);
		Engine::E_Tileset::draw(0, &parallax_calibration, &grid_region, &World::parallax_map1);


		Grid_Camera::get_Grid_Region_Covered(&grid_region, &World::camera);
		Grid::clip_Grid_Region(&grid_region, World::tilemap.n_cols, World::tilemap.n_rows);
		Grid_Camera::calibrate(&World::calibration, &World::camera, &grid_region);
		Engine::E_Tileset::draw(0, &World::calibration, &grid_region, &World::tilemap);

		
		for (int i = 0; i < World::Parameters::n_active_enemies; i++)
		{
			draw_Actor(&World::enemies[i]);
		}
		draw_Actor(&World::player);

		//flip buffers
		SDL_RenderPresent(Engine::renderer);

	}

	void init_Actor_Assets(Actor *p, int idle_sprite_index, int run_sprite_index, int jump_sprite_index)
	{
		p->idle_sprite_db_index = idle_sprite_index;
		p->idle_sprite_id = Engine::E_Sprite::make_Instance(p->idle_sprite_db_index);
		Engine::E_Sprite::modify(p->idle_sprite_db_index, p->idle_sprite_id, 75);

		p->run_sprite_db_index = run_sprite_index;
		p->run_sprite_id = Engine::E_Sprite::make_Instance(p->run_sprite_db_index);
		Engine::E_Sprite::modify(p->run_sprite_db_index, p->run_sprite_id, 75);

		p->jump_sprite_db_index = jump_sprite_index;
		p->jump_sprite_id = Engine::E_Sprite::make_Instance(p->jump_sprite_db_index);
		Engine::E_Sprite::modify(p->jump_sprite_db_index, p->jump_sprite_id, 75);
	}

	void update_Actor_Physics(Actor *p, unsigned int current_time, float dt)
	{
		float current_friction = 0.8;//later...

		//DONE ADDING FORCES


		//PHYSICS BEGIN


		//printf("force %.4f %.4f vel %.4f %.4f\n", World::bodies.force[p->physics_body].x, World::bodies.force[p->physics_body].y, World::bodies.vel[p->physics_body].x, World::bodies.vel[p->physics_body].y);

		//integrate acceleration
		Body::update_Vel(p->physics_body, &World::bodies, dt);
		//apply friction
		World::bodies.vel[p->physics_body].x *= current_friction;// *dt;

		//clip velocity
		Vec2D::clip(&World::bodies.vel[p->physics_body], -World::Parameters::max_vel_x, World::Parameters::max_vel_x, -World::Parameters::max_vel_y, World::Parameters::max_vel_y);

		if (abs(World::bodies.vel[p->physics_body].x) < 0.00001) World::bodies.vel[p->physics_body].x = 0.0;
		if (abs(World::bodies.vel[p->physics_body].y) < 0.00001) World::bodies.vel[p->physics_body].y = 0.0;

		Shape::Rect tmp_pos = p->world_coord;
		tmp_pos.x += World::bodies.vel[p->physics_body].x * dt;

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
		tmp_pos.y += World::bodies.vel[p->physics_body].y * dt;

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

		if (p->move_cmd_up == 1 && p->prev_move_cmd_up == 0 && on_ground == 1)
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
			printf("n_active_enemies=%4d\n", World::Parameters::n_active_enemies);
		}

		if (Grid::tile(&actor_feelers.mid_feeler, &World::object_map) == World::Parameters::super_jump_tile_id)
		{
			//FIX::feeler is inside the box for a long time!
			Vec2D::Vec2D f = { e->move_force_mag*20, 0 };
			Body::add_Force(e->physics_body, &World::bodies, &f);
		}
	}

	void update_Enemy_AI(Actor *e, unsigned int current_time)
	{
		e->prev_move_cmd_left = e->move_cmd_left;
		e->prev_move_cmd_right = e->move_cmd_right;
		e->prev_move_cmd_up = e->move_cmd_up;
		e->prev_move_cmd_down = e->move_cmd_down;
		e->move_cmd_left = 0;
		e->move_cmd_right = 0;
		e->move_cmd_up = 0;
		e->move_cmd_down = 0;


		Collision::Point_Feeler actor_feelers_far;
		Collision::point_Feeler_Pos(&actor_feelers_far, &e->world_coord, 0.8, 0.8);

		Collision::Point_Feeler actor_feelers_close;
		Collision::point_Feeler_Pos(&actor_feelers_close, &e->world_coord, 0.1, 0.2);

		if (Grid::tile(&actor_feelers_far.right_feeler, &World::collision_map) < 0)
		{
			e->move_cmd_right = 1;
		}
		else
		{
			//FIX::feeler inside collision for a long time!
			e->move_cmd_right = 1;
			if (rand() % 2 == 0)
			{
				e->move_cmd_right = 0;
				e->move_cmd_left = 1;
			}
			if (Grid::tile(&actor_feelers_close.bottom_feeler, &World::collision_map) > 0
				||
				Grid::tile(&actor_feelers_close.bottomright_feeler, &World::collision_map) > 0)
			{
				e->move_cmd_up = 1;
			}
		}


	}
}