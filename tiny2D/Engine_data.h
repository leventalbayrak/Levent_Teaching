#pragma once
#include "SDL2-2.0.8/include/SDL.h"
#include "Tileset_data.h"
#include "Sprite_data.h"
#include "Grid_Camera_core.h"

namespace Engine
{
	SDL_Renderer *renderer = NULL;
	SDL_Window *window = NULL;
	int screen_width;
	int screen_height;

	Tileset::Tileset tileset_db;
	Sprite::Data sprite_db;

	unsigned char prev_key_state[256];
	unsigned char *keys = NULL;

	const int max_n_game_controllers = 4;
	SDL_GameController *game_controller[max_n_game_controllers];
	//[joystick id] [x or y axis]
	/*
	0	SDL_CONTROLLER_AXIS_LEFTX
	1	SDL_CONTROLLER_AXIS_LEFTY
	2	SDL_CONTROLLER_AXIS_RIGHTX
	3	SDL_CONTROLLER_AXIS_RIGHTY
	4	SDL_CONTROLLER_AXIS_TRIGGERLEFT
	5	SDL_CONTROLLER_AXIS_TRIGGERRIGHT
	*/
	int prev_game_controller_sticks[max_n_game_controllers][6];
	int game_controller_sticks[max_n_game_controllers][6];
	/*
	0	SDL_CONTROLLER_BUTTON_A
	1	SDL_CONTROLLER_BUTTON_B
	2	SDL_CONTROLLER_BUTTON_X
	3	SDL_CONTROLLER_BUTTON_Y
	4	SDL_CONTROLLER_BUTTON_BACK
	5	SDL_CONTROLLER_BUTTON_GUIDE
	6	SDL_CONTROLLER_BUTTON_START
	7	SDL_CONTROLLER_BUTTON_LEFTSTICK
	8	SDL_CONTROLLER_BUTTON_RIGHTSTICK
	9	SDL_CONTROLLER_BUTTON_LEFTSHOULDER
	10	SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
	11	SDL_CONTROLLER_BUTTON_DPAD_UP
	12	SDL_CONTROLLER_BUTTON_DPAD_DOWN
	13	SDL_CONTROLLER_BUTTON_DPAD_LEFT
	14	SDL_CONTROLLER_BUTTON_DPAD_RIGHT
	*/
	int prev_game_controller_buttons[max_n_game_controllers][15];
	int game_controller_buttons[max_n_game_controllers][15];
	int n_game_controllers = 0;
	
}