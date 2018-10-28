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
#include "Input_core.h"

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
		Input::init();
	}

	void event_Loop()
	{
		Input::prepare();

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
			
		}

		Input::update();
	}

}