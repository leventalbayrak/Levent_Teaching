//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#pragma comment(linker,"/subsystem:console")
#include <iostream>
#include <assert.h>
using namespace std;

SDL_Renderer *renderer = NULL;
int screen_width = 800;
int screen_height = 600;

SDL_Window *window = NULL;


//Alternative example
struct Pixel
{
	unsigned char r, g, b, a;
};

void set_Pixel_Alternative(unsigned char* buffer, int width, int x, int y, int r, int g, int b, int a)
{
	Pixel *p = (Pixel*)buffer;
	p[y*width + x].r = r;
	p[y*width + x].g = g;
	p[y*width + x].b = b;
	p[y*width + x].a = a;
}

void fill_Rectangle(unsigned char*buffer, int buffer_width, int buffer_height, int rect_x, int rect_y, int rect_w, int rect_h, int r, int g, int b, int a)
{
	for (int i = 0; i < rect_w; i++)
	{
		for (int j = 0; j < rect_h; j++)
		{
			int x = i + rect_x;
			int y = j + rect_y;
			set_Pixel_Alternative(buffer, screen_width, x, y, r, g, b, a);
		}
	}

}

int direction_Collision(float x, float y, float w, float h, float x1, float y1, float w1, float h1)
{
	int no_collision = 0;
	int top_collision = 1;
	int right_collision = 2;
	int bottom_collision = 3;
	int left_collision = 4;

	float W = 0.5 * (w + w1);
	float H = 0.5 * (h + h1);
	float dx = x - x1 + 0.5*(w - w1);
	float dy = y - y1 + 0.5*(h - h1);

	if (dx*dx <= W * W && dy*dy <= H * H)
	{
		float wy = W * dy;
		float hx = H * dx;

		if (wy > hx)
		{
			if (wy + hx > 0)
			{
				return bottom_collision;
			}
			else
				return left_collision;
		}
		else
		{
			if (wy + hx > 0)
			{
				return right_collision;
			}
			else
				return top_collision;
		}
	}
	return no_collision;
}

void impulse_Collision(float x, float y, float w, float h, float *Xvec, float *Yvec, float mass_a, float x1, float y1, float w1, float h1, float *Xvec1, float *Yvec1, float mass_b)
{
	float normal_x;
	float normal_y;

	int k = direction_Collision(x, y, w, h, x1, y1, w1, h1);

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

	float relative_velocity_x = *Xvec1 - *Xvec;
	float relative_velocity_y = *Yvec1 - *Yvec;

	float dot = (relative_velocity_x * normal_x) + (relative_velocity_y * normal_y);

	if (dot < 0) return;

	float bounce = 1.0; //1.0 bounce, 0.0 mud
	float mass = -(1.0 + bounce) * dot / (mass_a + mass_b);

	float impulse_a_x = normal_x * mass * mass_a;
	float impulse_a_y = normal_y * mass * mass_a;

	float impulse_b_x = normal_x * mass * mass_b;
	float impulse_b_y = normal_y * mass * mass_b;

	*Xvec -= impulse_a_x;
	*Yvec -= impulse_a_y;

	*Xvec1 += impulse_b_x;
	*Yvec1 += impulse_b_y;

	float mt = 1.0 / (mass_a + mass_b);

	float tangent_x = normal_y;
	float tangent_y = -normal_x;

	float tangent_dot = (relative_velocity_x * tangent_x) + (relative_velocity_y * tangent_y);

	tangent_x = tangent_x  * -tangent_dot * mt;
	tangent_y = tangent_y  * -tangent_dot * mt;

	*Xvec -= tangent_x;
	*Yvec -= tangent_y;

	*Xvec1 += tangent_x;
	*Yvec1 += tangent_y;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		"POWEERRRRRR",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_SOFTWARE);

	SDL_Surface *your_draw_buffer = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_Surface *screen = SDL_GetWindowSurface(window);
	SDL_SetSurfaceBlendMode(your_draw_buffer, SDL_BLENDMODE_NONE);
	float *data = new float[screen_width * screen_height];

	unsigned char *my_own_buffer = (unsigned char*)malloc(sizeof(unsigned char)*screen_width*screen_height * 4);


	float box_size = 60;
	float box_x = 200;
	float box_x1 = 500;
	float box_y = 100;
	float box_y1 = 100;
	float Xvelocity = 1.2;
	float Yvelocity = 0.01;
	float Xvelocity2 = -1.2;
	float Yvelocity2 = 0.01;
	float box_mass1 = 1;
	float box_mass2 = 1;
	float wall_edges = 10;

	int max_boxes = 2;

	int bounce = 0;//counter for number of bounces
	int bounce1 = 0;//counter for number of bounces

	for (;;)
	{

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		////box wall collision
		//if (box_x <= wall_edges || box_x >= screen_width - (box_size + wall_edges))
		//{
		//	box_force_x *= -1;
		//	bounce++;
		//	printf("Number of bounces box1: %d\n", bounce);
		//}
		//if (box_y <= wall_edges || box_y >= screen_height - (box_size + wall_edges))
		//{
		//	box_force_y *= -1;
		//	bounce++;
		//	printf("Number of bounces box1: %d\n", bounce);
		//}

		////box wall collision
		//if (box_x1 <= wall_edges || box_x1 >= screen_width - (box_size + wall_edges))
		//{
		//	box_force_x1 *= -1;
		//	bounce1++;
		//	//printf("Number of bounces box2: %d\n", bounce1);
		//}
		//if (box_y1 <= wall_edges || box_y1 >= screen_height - (box_size + wall_edges))
		//{
		//	box_force_y1 *= -1;
		//	bounce1++;
		//	//printf("Number of bounces box2: %d\n", bounce1);
		//}

		box_x += Xvelocity;
		box_y += Yvelocity;

		box_x1 += Xvelocity2;
		box_y1 += Yvelocity2;

		//screen buffer
		for (int i = 0; i < screen_width*screen_height; i++)
		{
			my_own_buffer[i * 4] = 0;//r
			my_own_buffer[i * 4 + 1] = 0;//g
			my_own_buffer[i * 4 + 2] = 0;//b
			my_own_buffer[i * 4 + 3] = 0;//a
		}

		//Top Wall
		fill_Rectangle(my_own_buffer, screen_width, screen_height, 0, 0, screen_width, 10, 255, 0, 0, 255);
		//Left Wall
		fill_Rectangle(my_own_buffer, screen_width, screen_height, 0, 0, 10, screen_height, 255, 0, 0, 255);
		//Bottom Wall
		fill_Rectangle(my_own_buffer, screen_width, screen_height, 0, screen_height - 10, screen_width, 10, 255, 0, 0, 255);
		//Right Wall
		fill_Rectangle(my_own_buffer, screen_width, screen_height, screen_width - 10, 0, 10, screen_height, 255, 0, 0, 255);

		//boxes
		fill_Rectangle(my_own_buffer, screen_width, screen_height, box_x, box_y, box_size, box_size, 255, 255, 0, 255);
		fill_Rectangle(my_own_buffer, screen_width, screen_height, box_x1, box_y1, box_size, box_size, 255, 0, 0, 255);

		//box collisions
		impulse_Collision(box_x, box_y, box_size, box_size, &Xvelocity, &Yvelocity, box_mass1, box_x1, box_y1, box_size, box_size, &Xvelocity2, &Yvelocity2, box_mass2);
		//impulse_Collision(&box_x1, &box_y1, &box_size, &box_size, &Xvelocity2, &Yvelocity2, box_mass2, &box_x, &box_y, &box_size, &box_size, &Xvelocity, &Yvelocity, box_mass1);

		memcpy(your_draw_buffer->pixels, my_own_buffer, sizeof(unsigned char)*screen_width*screen_height * 4);

		//BLIT BUFFER TO SCREEN

		SDL_BlitScaled(your_draw_buffer, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
	}

	return 0;
}