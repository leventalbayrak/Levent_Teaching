#pragma once
#include <iostream>
#include <assert.h>
#include <math.h>
#include <ctime>
#include <cstdlib>
using namespace std;

//include SDL header
#include "SDL2-2.0.8/include/SDL.h"

//load libraries
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(linker, "/subsystem:console")

void fill_rectangle(unsigned char*buffer, int buffer_width, int buffer_height, int rect_x, int rect_y, int rect_w, int rect_h, int r, int g, int b, int a)
{
	for (int y = 0; y < rect_h; y++)
	{
		for (int x = 0; x < rect_w; x++)
		{
			int index = 4 * (rect_y + y)*buffer_width + 4 * (x + rect_x);
			buffer[index] = r;
			buffer[index + 1] = g;
			buffer[index + 2] = b;
			buffer[index + 3] = a;
		}

	}
}

SDL_Renderer *renderer = NULL;
int screen_width = 800;
int screen_height = 600;

SDL_Window *window = NULL;

struct Box
{
	float x, y;
	float w, h;
	float speed_x, speed_y;
	int r, g, b, a;
	int count_hits;
};

struct Border
{
	float x, y;
	float w, h;
};


// int collision_with_Dir(const Data *a, const Data *b)
int collision_with_dir(float *rect_A_x, float *rect_A_y, float *rect_A_w, float *rect_A_h, float *rect_B_x, float *rect_B_y, float *rect_B_w, float *rect_B_h)
{
	enum { NO_COLLISION, TOP_OF_B, RIGHT_OF_B, BOTTOM_OF_B, LEFT_OF_B };

	float w = 0.5 * (*rect_A_w + *rect_B_w);
	float h = 0.5 * (*rect_A_h + *rect_B_h);
	float dx = *rect_A_x - *rect_B_x + 0.5*(*rect_A_w - *rect_B_w);
	float dy = *rect_A_y - *rect_B_y + 0.5*(*rect_A_h - *rect_B_h);

	if (dx*dx <= w * w && dy*dy <= h * h)
	{
		float wy = w * dy;
		float hx = h * dx;

		if (wy > hx)
		{
			return (wy + hx > 0) ? BOTTOM_OF_B : LEFT_OF_B;
		}
		else
		{
			return (wy + hx > 0) ? RIGHT_OF_B : TOP_OF_B;
		}
	}
	return NO_COLLISION;
}

//void impulse
void impulse(float *rect_A_x, float *rect_A_y, float *rect_A_w, float *rect_A_h, float *rect_A_speed_x, float *rect_A_speed_y, float *a_inv_mass,
	float *rect_B_x, float *rect_B_y, float *rect_B_w, float *rect_B_h, float *rect_B_speed_x, float *rect_B_speed_y, float *b_inv_mass)
{
	float normal_x;
	float normal_y;

	int k = collision_with_dir(rect_A_x, rect_A_y, rect_A_w, rect_A_h, rect_B_x, rect_B_y, rect_B_w, rect_B_h);
	if (k == 1)
	{
		normal_x = 0;
		normal_y = -1;
	}
	else if (k == 2)
	{
		normal_x = 1;
		normal_y = 0;
	}
	else if (k == 3)
	{
		normal_x = 0;
		normal_y = 1;
	}
	else if (k == 4)
	{
		normal_x = -1;
		normal_y = 0;
	}
	else
	{
		normal_x = 0;
		normal_y = 0;
	}

	float relative_speed_x = *rect_B_speed_x;
	float relative_speed_y = *rect_B_speed_y;

	relative_speed_x -= *rect_A_speed_x;
	relative_speed_y -= *rect_A_speed_y;

	float d = relative_speed_x * normal_x + relative_speed_y * normal_y; // dot product

	if (d < 0) return;

	float e = 1.0; // 1.0 is for elastic bounce, 0.0 for mud
	float j = -(1.0 + e)*d / (*a_inv_mass + *b_inv_mass); // mass of tile is infinity (1/ma+1/mb), if you use the actual mass then objects will bounce

	float impulse_a_x = normal_x;
	float impulse_a_y = normal_y;

	impulse_a_x *= j * *a_inv_mass;  //scale
	impulse_a_y *= j * *a_inv_mass;

	float impulse_b_x = normal_x;
	float impulse_b_y = normal_y;

	impulse_b_x *= j * *b_inv_mass;  //scale
	impulse_b_y *= j * *b_inv_mass;

	*rect_A_speed_x -= impulse_a_x;  // sub
	*rect_A_speed_y -= impulse_a_y;

	*rect_B_speed_x += impulse_b_x;	//add
	*rect_B_speed_y += impulse_b_y;

	float mt = 1.0 / (*a_inv_mass + *b_inv_mass);

	float tangent_x = normal_y;
	float tangent_y = -normal_x;

	float tangent_d = relative_speed_x * tangent_x + relative_speed_y * tangent_y; // dot product

	tangent_x *= (-tangent_d)*mt; //scale
	tangent_y *= (-tangent_d)*mt;

	*rect_A_speed_x -= tangent_x;  //sub
	*rect_A_speed_y -= tangent_y;

	*rect_B_speed_x += tangent_x;  //add
	*rect_B_speed_y += tangent_y;

}

//impulse against immovable object
void impulse(float *rect_A_x, float *rect_A_y, float *rect_A_w, float *rect_A_h, float *rect_A_speed_x, float *rect_A_speed_y, float *a_inv_mass,
	float *rect_B_x, float *rect_B_y, float *rect_B_w, float *rect_B_h)
{
	float normal_x;
	float normal_y;

	int k = collision_with_dir(rect_A_x, rect_A_y, rect_A_w, rect_A_h, rect_B_x, rect_B_y, rect_B_w, rect_B_h);
	if (k == 1)
	{
		normal_x = { 0 };
		normal_y = { -1 };
	}
	else if (k == 2)
	{
		normal_x = { 1 };
		normal_y = { 0 };
	}
	else if (k == 3)
	{
		normal_x = { 0 };
		normal_y = { 1 };
	}
	else if (k == 4)
	{
		normal_x = { -1 };
		normal_y = { 0 };
	}
	else
	{
		normal_x = { 0 };
		normal_y = { 0 };
	}


	float relative_vel_x = -*rect_A_speed_x;
	float relative_vel_y = -*rect_A_speed_y;

	float d = relative_vel_x * normal_x + relative_vel_y * normal_y;

	if (d < 0) return;

	float e = 1.0;//1.0 is for elastic bounce, 0.0 for mud
	float j = -(1.0 + e) * d / (*a_inv_mass);//mass of tile is infinity (1/ma+1/mb), if you use actual mass then objects will bounce

	float impulse_x = normal_x;
	float impulse_y = normal_y;

	impulse_x *= j;
	impulse_y *= j;

	*rect_A_speed_x -= impulse_x;
	*rect_A_speed_y -= impulse_y;
}


void Update_inicial_values(float *box_x, float *box_y, float *box_w, float *box_h, int *box_r, int *box_g, int *box_b, int *box_a, float *box_speed_x, float *box_speed_y, int *box_count_hits)
{
	*box_x = screen_width / 2;
	*box_y = screen_height / 2;
	*box_w = 25;
	*box_h = 25;
	*box_r = rand() % 256;
	*box_g = rand() % 256;
	*box_b = rand() % 256;
	*box_a = 256;
	*box_speed_x = 1.0 - 2.0*rand() / RAND_MAX;
	*box_speed_y = 1.0 - 2.0*rand() / RAND_MAX;
	*box_count_hits = 0;
}

int main(int argc, char**argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("ALEJANDRA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_Surface*your_draw_buffer = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface*screen = SDL_GetWindowSurface(window);

	SDL_SetSurfaceBlendMode(your_draw_buffer, SDL_BLENDMODE_NONE);

	unsigned char *my_own_buffer = (unsigned char*)malloc(sizeof(unsigned char)*screen_width*screen_height * 4);

	// BORDERS
	Border *border = (Border*)malloc(sizeof(Border) * 10);
	{	// TOP
		border[1].x = 1;
		border[1].y = 1;
		border[1].w = screen_width - 1;
		border[1].h = 5;
		//BOTTOM
		border[2].x = 1;
		border[2].h = 5;
		border[2].y = screen_height - border[2].h - 1;
		border[2].w = screen_width - 1;
		//LEFT
		border[3].x = 1;
		border[3].y = 1;
		border[3].w = 5;
		border[3].h = screen_height - 1;
		//RIGHT
		border[4].y = 1;
		border[4].w = 5;
		border[4].h = screen_height - 1;
		border[4].x = screen_width - border[4].w - 1;
	}

	float a_inv_mass = 1;
	float b_inv_mass = 1;

	int n_current_boxes = 0;
	int n_boxes_max = 200;
	int n_max_hits = 20;

	Box *box = (Box*)malloc(sizeof(Box)*n_boxes_max);
	for (int i = 0; i < n_boxes_max; i++)
	{
		box[i].x = screen_width / 2;
		box[i].y = screen_height / 2;
		box[i].w = 20;
		box[i].h = 20;
		box[i].r = rand() % 256;
		box[i].g = rand() % 256;
		box[i].b = rand() % 256;
		box[i].a = 256;
		box[i].speed_x = 1.0 - 2.0*rand() / RAND_MAX;
		box[i].speed_y = 1.0 - 2.0*rand() / RAND_MAX;
		box[i].count_hits = 0;
	}

	// CLOCk 
	clock_t TimeZero = clock(); //Start timer
	double deltaTime = 0;
	double secondsToDelay = 1; // 

	for (;;)
	{
		//Consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		// TO DELETE BOXES : currently not working
		for (int i = 0; i < n_current_boxes; i++)
		{
			if (box[i].count_hits > n_max_hits)
			{
				box[i].r = 0;
				box[i].g = 0;
				box[i].b = 0;
				box[i].a = 256;
				Update_inicial_values(&box[i].x, &box[i].y, &box[i].w, &box[i].h, &box[i].r, &box[i].g, &box[i].b, &box[i].a, &box[i].speed_x, &box[i].speed_y, &box[i].count_hits);
			}

		}

		memset(my_own_buffer, 0, 4 * screen_width*screen_height);

		// TO FILL BOXES
		for (int i = 0; i < n_current_boxes; i++)
		{
			fill_rectangle(my_own_buffer, screen_width, screen_height, box[i].x, box[i].y, box[i].w, box[i].h, box[i].r, box[i].g, box[i].b, box[i].a);
			box[i].x += box[i].speed_x;
			box[i].y += box[i].speed_y;
		}

		// TO FILL THE BORDERS 
		for (int j = 1; j <= 4; j++)
		{
			fill_rectangle(my_own_buffer, screen_width, screen_height, border[j].x, border[j].y, border[j].w, border[j].h, 255, 0, 0, 255);
		}

		// COLLISION / IMPULSE
		for (int i = 0; i < n_current_boxes; i++)
		{
			for (int j = i + 1; j < n_current_boxes; j++)
			{
				impulse(&box[i].x, &box[i].y, &box[i].w, &box[i].h, &box[i].speed_x, &box[i].speed_y, &a_inv_mass,
					&box[j].x, &box[j].y, &box[j].w, &box[j].h, &box[j].speed_x, &box[j].speed_y, &b_inv_mass);
			}
			// TO COLLIDE WITH THE BORDERS
			for (int k = 1; k <= 4; k++)
			{
				impulse(&box[i].x, &box[i].y, &box[i].w, &box[i].h, &box[i].speed_x, &box[i].speed_y, &a_inv_mass, &border[k].x, &border[k].y, &border[k].w, &border[k].h);
			}
		}

		int k_box = 0;
		int k_border = 0;
		// COUNT HITS
		for (int i = 0; i < n_current_boxes; i++)
		{
			for (int j = i + 1; j < n_current_boxes; j++)
			{
				k_box = collision_with_dir(&box[j].x, &box[j].y, &box[j].w, &box[j].h, &box[i].x, &box[i].y, &box[i].w, &box[i].h);
				if (k_box != 0) box[i].count_hits++;
			}

			for (int j = 0; j <= 4; j++)
			{
				k_border = collision_with_dir(&border[j].x, &border[j].y, &border[j].w, &border[j].h, &box[i].x, &box[i].y, &box[i].w, &box[i].h);
				if (k_border != 0) box[i].count_hits++;
			}
		}

		memcpy(your_draw_buffer->pixels, my_own_buffer, sizeof(unsigned char)*screen_width*screen_height * 4);

		//BLIT BUFFER TO SCREEN
		SDL_BlitScaled(your_draw_buffer, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);

		// CLOCK CLOCK
		deltaTime = (clock() - TimeZero) / CLOCKS_PER_SEC;
		if (deltaTime > secondsToDelay)
		{
			printf("\n\nTimer working every 1 seconds");
			if (n_current_boxes < n_boxes_max) n_current_boxes++;
			cout << "\nBalls on screen: " << n_current_boxes;

			//reset the clock timers
			deltaTime = clock();
			TimeZero = clock();

			// PRINT HITS
			for (int i = 0; i < n_current_boxes; i++)
			{
				printf("\nBall # %d: Current Hits: %d", i + 1, box[i].count_hits);
			}
		}
	}
	return 0;
}