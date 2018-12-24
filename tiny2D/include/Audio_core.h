#pragma once
#include "SDL2-2.0.8\include\SDL_mixer.h"
#include "Audio_data.h"
namespace Audio
{
	void init(int buffer_size);

	int add_Music(const char *music_filename);

	void play_Music(int index, int loop);

	void pause_Music();
	void resume_Music();

	int add_FX(const char *fx_filename);

	//0 to 128
	//returns previous volume
	int set_FX_Volume(int index, int volume);

	int set_Music_Volume(int volume);
	void play_FX(unsigned char index);
}

namespace Audio
{
	void init(int buffer_size)
	{
		int audio_buffer_size = buffer_size;
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, buffer_size);

		Mix_AllocateChannels(n_channel_per_sound*max_n_sounds);

		memset(sound_fx, 0, sizeof(Mix_Chunk*)*max_n_sounds*n_channel_per_sound);
		memset(ring, 0, sizeof(int)*max_n_sounds);

		queue = (int*)malloc(sizeof(int)*max_n_queue);
		queue_sound_table = (int*)malloc(sizeof(int)*(max_n_sounds + 1));
	}

	int add_Music(const char *music_filename)
	{
		int index = n_music;
		if (music[n_music] != NULL)
		{
			Mix_FreeMusic(music[n_music]);
			music[n_music] = NULL;
		}
		music[n_music] = Mix_LoadMUS(music_filename); assert(music[n_music]);
		n_music = (n_music + 1) & (max_n_music - 1);
		return index;
	}

	void play_Music(int index, int loop)
	{
		Mix_PlayMusic(music[index], loop);
	}

	void pause_Music()
	{
		Mix_PauseMusic();
	}

	void resume_Music()
	{
		Mix_ResumeMusic();
	}

	int add_FX(const char *fx_filename)
	{
		int index = n_sounds;
		//overwrite existing sound. sorry.
		if (sound_fx[n_sounds] != NULL)
		{
			Mix_FreeChunk(sound_fx[n_sounds]);
			sound_fx[n_sounds] = NULL;
		}
		sound_fx[n_sounds] = Mix_LoadWAV(fx_filename); assert(sound_fx[n_sounds]);
		n_sounds = (n_sounds + 1) & (max_n_sounds - 1);
		return index;
	}

	//0 to 128
	//returns previous volume
	int set_FX_Volume(int index, int volume)
	{
		int r = Mix_VolumeChunk(sound_fx[index], volume);
		return r;
	}

	int set_Music_Volume(int volume)
	{
		int r = Mix_VolumeMusic(volume);
		return r;
	}

	void play_FX(unsigned char index)
	{
		int available_channel = index * n_channel_per_sound + ring[index];
		ring[index] = (ring[index] + 1) & (n_channel_per_sound-1);
		Mix_PlayChannel(available_channel, sound_fx[index], 0);
	}

	void queue_FX(unsigned char index)
	{
		if (n_queue >= max_n_queue) return;
		queue[n_queue++] = index;
	}

	void play_Queue()
	{
		if (n_queue == 0) return;
		memset(queue_sound_table, 0, sizeof(int)*(max_n_sounds + 1));
		//use lookup table dont play the same sound in the same queue
		for (int i = 0; i < n_queue; i++)
		{
			if (queue_sound_table[queue[i]] == 0)
			{
				queue_sound_table[queue[i]] = 1;
				play_FX(queue[i]);
			}		
		}
		n_queue = 0;
	}
}