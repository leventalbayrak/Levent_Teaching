
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(linker,"/subsystem:console")

SDL_Renderer *renderer = NULL;

class Sprite
{
	float x, y, w, h;
	float angular_vel;
	float angle;
	float vx, vy;
	unsigned int lifetime;
	unsigned int creation_time;
	
	int active;
	int r, g, b;
	
	int animation_frame;
	int number_of_frames;
	unsigned int last_time_animation_updated;
	unsigned int animation_speed;

	int frame_x;
	int frame_y;
	int frame_w;
	int frame_h;
	SDL_Texture *texture;
public:
	Sprite(const char *filename, int _frame_x, int _frame_y,int _frame_w, int _frame_h, int n_frames)
	{
		SDL_Surface *surface = IMG_Load(filename);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		frame_x = _frame_x;
		frame_y = _frame_y;
		frame_w = _frame_w;
		frame_h = _frame_h;
		number_of_frames = n_frames;

		x = 0;
		y = 0;
		w = 64;
		h = 64;

		animation_speed = 100;
	}

	void update(unsigned int current_time)
	{
		x += vx;
		y += vy;

		angle += angular_vel;

		if (current_time - last_time_animation_updated > animation_speed)
		{
			last_time_animation_updated = current_time;
			animation_frame++;
			animation_frame %= number_of_frames;
		}

	}

	void draw()
	{
		int current_frame_x = frame_x + animation_frame*frame_w;
		SDL_Rect src = {current_frame_x ,frame_y,frame_w,frame_h };
		SDL_Rect dest = { x,y,w,h };
		SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE);
	}


	void set_Velocity_and_Angle(float _vx, float _vy, float _angle_v)
	{
		vx = _vx;
		vy = _vy;
		angular_vel = _angle_v;
	}

	void limit_Pos(float min_x, float max_x, float min_y, float max_y)
	{
		if (x < min_x || x > max_x-w)
		{
			vx *= -1;
		}

		if (y < min_y || y > max_y-h)
		{
			vy *= -1;
		}
	}

};


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("james", 50, 50, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Sprite clement("images/werewolf.png", 0, 0, 32, 32, 4);
	clement.set_Velocity_and_Angle(0.1, 0.1, 0.1);

	Sprite leo("images/dvd.png", 0, 0, 64, 64, 4);
	leo.set_Velocity_and_Angle(0.09, 0.11, 0.1);

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;
		}

		unsigned int current_time = SDL_GetTicks();

		/*
		update	
		*/
		clement.update(current_time);
		clement.limit_Pos(0, 800, 0, 600);
		
		leo.update(current_time);
		leo.limit_Pos(0, 800, 0, 600);

		
		/*
		draw
		*/
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderClear(renderer);

		clement.draw();
		leo.draw();


		SDL_RenderPresent(renderer);
	}

	return 0;
}