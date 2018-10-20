#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2_image-2.0.3\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

#include <iostream>
#include <fstream>
#include "Table_File_core.h"
using namespace std;

namespace Tileset
{
	struct Tileset
	{
		int tile_w;
		int tile_h;
		int n_cols;
	};

	int get_Col(int tile_id, Tileset *t)
	{
		return t->tile_w * (tile_id % t->n_cols);
	}

	int get_Row(int tile_id, Tileset *t)
	{
		return t->tile_w * (tile_id / t->n_cols);
	}
}


int main(int argc, char **argv)
{
	//SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_ShowCursor(SDL_DISABLE);

	int screen_width = 32 * 32;
	int screen_height = 32 * 16;

	SDL_Window *window = SDL_CreateWindow(
		"tilemap",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *surface = IMG_Load("tileset.png");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	Tileset::Tileset tileset;
	tileset.tile_w = 32;
	tileset.tile_h = 32;
	tileset.n_cols = 23;

	Table_File::Table_File map_file;
	Table_File::read("party_map.csv", &map_file);
	
	int **map = new int*[map_file.nrows];
	for (int i = 0; i < map_file.nrows; i++)
	{
		map[i] = new int[map_file.ncols[i]];

		for (int j = 0; j < map_file.ncols[i]; j++)
		{
			map[i][j] = atoi(map_file.table[i][j]);
		}
	}
	
	int map_n_cols = map_file.ncols[0];
	int map_n_rows = map_file.nrows;

	
	bool done = false;

	while (!done)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}


		//DRAW
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < map_n_rows; i++)
		{
			for (int j = 0; j < map_n_cols; j++)
			{
				int tile_id = map[i][j];

				SDL_Rect src;
				src.x = Tileset::get_Col(tile_id, &tileset);
				src.y = Tileset::get_Row(tile_id, &tileset);
				src.w = 32;
				src.h = 32;

				SDL_Rect dest;
				dest.x = j * tileset.tile_w;
				dest.y = i * tileset.tile_h;
				dest.w = 32;
				dest.h = 32;

				SDL_RenderCopyEx(renderer, texture, &src, &dest, 0, NULL, SDL_FLIP_NONE);

			}
		}

		

		

		
		SDL_RenderPresent(renderer);
	}

	return 0;

}