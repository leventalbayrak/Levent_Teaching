
#pragma once
#include "include/Engine_core.h"
namespace My_Game
	namespace Command
	{
		int UP, DOWN, LEFT, RIGHT;
	}
	namespace Asset
	{
		//LOAD ASSETS HERE
	}

	namespace World
	{
	    //GLOBAL VARIABLES, ARRAYS
		namespace Parameters
		{
			//WORLD PARAMETERS
		}

	}

	void init(int screen_w, int screen_h)
	{
		Engine::init("My_Game", screen_w, screen_h);
        //OTHER INIT, ALLOCATIONS
	}

	void begin_Play()
	{
		//GAME BEGIN
	}

	void update(unsigned int current_time)
	{
        //INPUT
		Engine::event_Loop();

		Command::UP = 0;
		Command::DOWN = 0;
		Command::LEFT = 0;
		Command::RIGHT = 0;

		if (Engine::keys[SDL_SCANCODE_W] && Engine::prev_key_state[SDL_SCANCODE_W] == 0) Command::UP = 1;
		if (Engine::keys[SDL_SCANCODE_SPACE] && Engine::prev_key_state[SDL_SCANCODE_SPACE] == 0) Command::UP = 1;
		if (Engine::keys[SDL_SCANCODE_A]) Command::LEFT = 1;
		if (Engine::keys[SDL_SCANCODE_D]) Command::RIGHT = 1;

		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_A] && Engine::Input::prev_game_controller_buttons[0][SDL_CONTROLLER_BUTTON_A] == 0) Command::UP = 1;
		if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX] < -5000) Command::LEFT = 1;
		else if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX] > 5000) Command::RIGHT = 1;
	    
		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_LEFT]) Command::LEFT = 1;
		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_RIGHT]) Command::RIGHT = 1;
        
        //UPDATE GAME
	    
	}

	void draw()
	{
        //DRAW
        
		SDL_RenderPresent(Engine::renderer);
	}
	