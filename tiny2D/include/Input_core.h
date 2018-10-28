#pragma once
#include "Input_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
namespace Input
{
	void init()
	{
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
				if (Input::game_controller[Input::n_game_controllers] != NULL) Input::n_game_controllers++;
			}
		}
		printf("connected to %d game controllers\n", Input::n_game_controllers);
	}

	void prepare()
	{
		memcpy(prev_key_state, keys, 256);
	}

	void update()
	{
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
}