#pragma once

//load libraries
#pragma comment(lib,"include\\SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"include\\SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"include\\SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"include\\SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")
#pragma comment(linker,"/subsystem:console")

#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include "SDL2-2.0.8\include\SDL_mixer.h"

#include "Engine_data.h"
#include "Audio_core.h"
#include "Texture_core.h"
#include "Shape_core.h"
#include "Font_core.h"
#include "Collision_core.h"

#include <iostream>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

#include "Tileset_core.h"
#include "Grid_core.h"
#include "Grid_Camera_core.h"
#include "Sprite_core.h"
#include "Body_core.h"
#include "Font_core.h"
#include "Audio_core.h"
#include "RGBA_data.h"

namespace Engine
{
	unsigned int time()
	{
		return SDL_GetTicks();
	}

	void init(const char*window_title, int _screen_width, int _screen_height)
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
		srand(Engine::time());
		screen_width = _screen_width;
		screen_height = _screen_height;
		window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		Audio::init(2048);
		Font::init();
		Tileset::init(&tileset_db, 64);
		
		keys = (unsigned char*)SDL_GetKeyboardState(NULL);
		memcpy(prev_key_state, keys, 256);

		int n_connected = SDL_NumJoysticks();
		printf("found %d game controllers\n", n_connected);
		int k = min(n_connected, Input::max_n_game_controllers);
		Input::n_game_controllers = 0;
		for (int i = 0; i < k; i++)
		{
			if (SDL_IsGameController(i))
			{
				Input::game_controller[Input::n_game_controllers] = SDL_GameControllerOpen(i);
				if(Input::game_controller[Input::n_game_controllers]!=NULL) Input::n_game_controllers++;
			}
		}
		printf("connected to %d game controllers\n", Input::n_game_controllers);
	}

	void event_Loop()
	{
		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
			
		}

		for (int i = 0; i < Input::n_game_controllers; i++)
		{
			memcpy(Input::prev_game_controller_sticks[i], Input::game_controller_sticks[i], sizeof(int) * 6);
			for (int j = 0; j < 6; j++)
			{
				Input::game_controller_sticks[i][j] = SDL_GameControllerGetAxis(Input::game_controller[i], (SDL_GameControllerAxis)j);
			}
			memcpy(Input::prev_game_controller_buttons[i], Input::game_controller_buttons[i], sizeof(int) * 15);
			for (int j = 0; j < 15; j++)
			{
				Input::game_controller_buttons[i][j] = SDL_GameControllerGetButton(Input::game_controller[i], (SDL_GameControllerButton)j);
			}
		}
		
		int mouse_button = SDL_GetMouseState(&Input::mouse_x, &Input::mouse_y);
		Input::prev_mouse_left = Input::mouse_left;
		Input::prev_mouse_right = Input::mouse_right;
		Input::mouse_left = mouse_button & SDL_BUTTON(SDL_BUTTON_LEFT);
		Input::mouse_right = mouse_button & SDL_BUTTON(SDL_BUTTON_RIGHT);
	}

	namespace E_Tileset
	{
		void add(const char *filename)
		{
			Tileset::File::add(&tileset_db, filename, renderer);
		}

		
	}
}