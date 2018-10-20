#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "SDL2-2.0.8\include\SDL_mixer.h"
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"


//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")

#pragma comment(linker,"/subsystem:console")

#include <iostream>
#include <fstream>
#include "Table_File_core.h"
using namespace std;


int main(int argc, char **argv)
{



	{
		//READ ABOUT STATE MACHINES
		enum { idle, running, walking, eating, crying };
		int my_state = idle;



		for (;;)
		{
			bool see_bear = true;

			//things happen here
			//bear on bear off

			if (my_state == idle)
			{
				//do idle stuff


				//decide on transition
				if (see_bear)
				{
					my_state = running;
				}
			}
			else if (my_state == running)
			{
				//do running stuff


				if (!see_bear)
				{
					my_state = walking;
				}
				if (rand() % 100 < 2)
				{
					my_state = idle;
				}
			}


		}
	}


	{
		int state_table[5][5];
		int my_state = 0;

		for (;;)
		{
			double r = (double)rand() / RAND_MAX;

			double sum = 0.0;
			for (int i = 0; i < 5; i++)
			{
				sum += state_table[my_state][i];
				if (sum >= r)
				{
					my_state = i;
					break;
				}
			}
			
		}

	}
	

	return 0;

}