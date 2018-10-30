#pragma once
#include "SDL2-2.0.8\include\SDL_mixer.h"
namespace Audio
{
	int n_sounds = 0;
	const int max_n_sounds = 256;
	const int n_channel_per_sound = 4;
	Mix_Chunk *sound_fx[max_n_sounds*n_channel_per_sound];
	int ring[max_n_sounds];

	int n_music = 0;
	const int max_n_music = 256;
	Mix_Music *music[max_n_music];
}
