#pragma warning(disable : 4996)

#include "SDL2-2.0.8\include\SDL.h"
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

struct Float_Pair
{
	float x, y;
};

struct Color_Set
{
	unsigned char r, g, b, a;
};

namespace OM
{
	struct Rect
	{
		Float_Pair position;
		Color_Set color;
	};

	struct Entity
	{
		Rect body;
		Float_Pair velocity;
	};

	unsigned char find_Next_Open_Box(unsigned short boxN, const unsigned char *box_state)
	{
		for (unsigned short i = 0; i < boxN; i++)
		{
			if (box_state[i] == 0) return i + 1;
		}
		return 0;
	}

	void spawn(Entity *obj, unsigned char *state, unsigned short box_size, unsigned char max_vel, unsigned short screen_width, unsigned short screen_height)
	{
		*state = 1;
		obj->body.position.x = screen_width / 2.0 - (box_size / 2.0);
		obj->body.position.y = screen_height / 2.0 - (box_size / 2.0);
		obj->velocity.x = max_vel * (1.0 - 2.0 * rand() / RAND_MAX);
		obj->velocity.y = max_vel * (1.0 - 2.0 * rand() / RAND_MAX);
		obj->body.color.r = rand() % 256;
		obj->body.color.g = rand() % 256;
		obj->body.color.b = rand() % 256;
		int random = rand() % 3;
		if (random % 3 == 0) obj->body.color.r = 255;
		else if (random % 3 == 1) obj->body.color.g = 255;
		else obj->body.color.b = 255;
	}

	void despawn(unsigned char *state, unsigned short *bounce)
	{
		*bounce = 0;
		*state = 0;
	}

	void update(Float_Pair *position, const Float_Pair *velocity)
	{
		position->x += velocity->x;
		position->y += velocity->y;
	}

	void draw(Color_Set *canvas, const Rect *obj, unsigned short obj_w, unsigned short obj_h, unsigned short canvas_w)
	{
		for (int y = obj->position.y; y < obj->position.y + obj_h; y++)
		{
			for (int x = obj->position.x; x < obj->position.x + obj_w; x++)
			{
				canvas[y * canvas_w + x].r = obj->color.r;
				canvas[y * canvas_w + x].g = obj->color.g;
				canvas[y * canvas_w + x].b = obj->color.b;
			}
		}
	}

	int check_Collision(const Float_Pair *obj_position_1, const Float_Pair *obj_position_2, unsigned short box_size)
	{
		enum { NO_COLLISION = 0, TOP_2, RIGHT_2, BOTTOM_2, LEFT_2 };

		float avg_w = box_size;
		float avg_h = box_size;

		float diff_x = obj_position_1->x - obj_position_2->x;
		float diff_y = obj_position_1->y - obj_position_2->y;

		if (diff_x * diff_x <= avg_w * avg_w && diff_y * diff_y <= avg_h * avg_h)
		{
			float wy = avg_w * diff_y;
			float hx = avg_h * diff_x;

			if (wy > hx)
			{
				return (wy + hx > 0) ? BOTTOM_2 : LEFT_2;
			}
			else
			{
				return (wy + hx > 0) ? RIGHT_2 : TOP_2;
			}
		}
		else return NO_COLLISION;
	}

	int check_Collision(const Float_Pair *box_position, unsigned short box_size, const Float_Pair *border_position, unsigned short border_width, unsigned short border_height)
	{
		float avg_w = 0.5 * (box_size + border_width);
		float avg_h = 0.5 * (box_size + border_height);

		float diff_x = box_position->x - border_position->x + 0.5 * (box_size - border_width);
		float diff_y = box_position->y - border_position->y + 0.5 * (box_size - border_height);

		if (diff_x * diff_x <= avg_w * avg_w && diff_y * diff_y <= avg_h * avg_h)
		{
			return 1;
		}
		else return 0;
	}

	void impulse(Float_Pair *obj_velocity_1, Float_Pair *obj_velocity_2, unsigned char direction)
	{
		enum { NO_COLLISION = 0, TOP_2, RIGHT_2, BOTTOM_2, LEFT_2 };

		Float_Pair normal;

		if (direction == TOP_2) normal = { 0, -1 };
		else if (direction == RIGHT_2) normal = { 1, 0 };
		else if (direction == BOTTOM_2) normal = { 0, 1 };
		else if (direction == LEFT_2) normal = { -1, 0 };
		else normal = { 0,0 };

		Float_Pair relative_velocity = *obj_velocity_2;
		relative_velocity.x -= obj_velocity_1->x;
		relative_velocity.y -= obj_velocity_1->y;

		float dot = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

		if (dot < 0) return;

		float loss = -1.0 * dot;

		Float_Pair impulse = { normal.x * loss, normal.y * loss };

		obj_velocity_1->x -= impulse.x;
		obj_velocity_1->y -= impulse.y;

		obj_velocity_2->x += impulse.x;
		obj_velocity_2->y += impulse.y;

		float mt = 0.5;
		Float_Pair tangent = { normal.y, -normal.x };
		float tangent_dot = relative_velocity.x * tangent.x + relative_velocity.y * tangent.y;
		tangent.x *= -tangent_dot / 2.0;
		tangent.y *= -tangent_dot / 2.0;

		obj_velocity_1->x -= tangent.x;
		obj_velocity_1->y -= tangent.y;

		obj_velocity_2->x += tangent.x;
		obj_velocity_2->y += tangent.y;
	}

	void impulse(Float_Pair *obj_velocity, unsigned char direction)
	{
		enum { NO_COLLISION = 0, TOP_2, RIGHT_2, BOTTOM_2, LEFT_2 };

		Float_Pair normal;

		if (direction == TOP_2) normal = { 0, -1 };
		else if (direction == RIGHT_2) normal = { 1, 0 };
		else if (direction == BOTTOM_2) normal = { 0, 1 };
		else if (direction == LEFT_2) normal = { -1, 0 };
		else normal = { 0,0 };

		Float_Pair relative_velocity = { -obj_velocity->x, -obj_velocity->y };

		float dot = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

		if (dot < 0) return;

		float loss = -2.0 * dot;
		Float_Pair impulse = { normal.x * loss, normal.y * loss };

		obj_velocity->x -= impulse.x;
		obj_velocity->y -= impulse.y;
	}

	void inc_Bounce(unsigned short *bounce_count_1, unsigned short *bounce_count_2)
	{
		*bounce_count_1++;
		*bounce_count_2++;
	}

	void inc_Bounce(unsigned short *bounce_count)
	{
		*bounce_count++;

	}
}

void wmain()
{
	srand(time(0));

	SDL_Init(SDL_INIT_VIDEO);

	unsigned short screen_width = 800;
	unsigned short screen_height = 600;
	unsigned int screen_area = screen_width * screen_height;

	SDL_Window *window = SDL_CreateWindow("object managing simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface *print_surface = SDL_GetWindowSurface(window);
	SDL_Surface *work_surface = SDL_CreateRGBSurfaceWithFormat(0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);

	Color_Set *p = (Color_Set*)work_surface->pixels;
	Color_Set *canvas = (Color_Set*)calloc(screen_area, sizeof(Color_Set));
	for (unsigned int i = 0; i < screen_area; i++) canvas[i].a = 255;

	OM::Rect back;
	back.position.x = 0;
	back.position.y = 0;
	back.color.r = 0;
	back.color.g = 0;
	back.color.b = 0;
	back.color.a = 255;

	float border_size = 10;
	unsigned short border_width[5];
	unsigned short border_height[5];
	OM::Rect border[5];
	for (int i = 1; i < 5; i++)
	{
		border[i].color.r = 100;
		border[i].color.g = 100;
		border[i].color.b = 100;
		border[i].color.a = 255;
	}

	border_width[1] = screen_width;
	border_height[1] = border_size;
	border[1].position.x = 0;
	border[1].position.y = screen_height - border_size;

	border_width[2] = border_size;
	border_height[2] = screen_height;
	border[2].position.x = 0;
	border[2].position.y = 0;

	border_width[3] = screen_width;
	border_height[3] = border_size;
	border[3].position.x = 0;
	border[3].position.y = 0;

	border_width[4] = border_size;
	border_height[4] = screen_height;
	border[4].position.x = screen_width - border_size;
	border[4].position.y = 0;

	unsigned short boxN = 999;
	unsigned short box_size = 10;
	unsigned char max_box_vel = 5;
	unsigned char *box_state = (unsigned char*)calloc(boxN, sizeof(unsigned char));
	unsigned short *box_bounce = (unsigned short*)calloc(boxN, sizeof(unsigned short));
	OM::Entity *box = (OM::Entity*)calloc(boxN, sizeof(OM::Entity));
	for (unsigned short i = 0; i < boxN; i++) box[i].body.color.a = 255;

	unsigned int spawn_timer_start = 0;
	unsigned int spawn_timer_end = SDL_GetTicks();
	unsigned char box_spawn_time = 1;
	unsigned char max_box_bounce = 10;

	for (;;)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT) exit(0);
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
			}
		}

		if ((spawn_timer_end - spawn_timer_start) / 1000 == box_spawn_time)
		{
			spawn_timer_start = spawn_timer_end;
			unsigned short offset_next_open_box = OM::find_Next_Open_Box(boxN, box_state);
			if (offset_next_open_box != 0)
			{
				offset_next_open_box--;
				OM::spawn(&box[offset_next_open_box], &box_state[offset_next_open_box], box_size, max_box_vel, screen_width, screen_height);
			}
		}
		spawn_timer_end = SDL_GetTicks();

		for (unsigned short i = 0; i < boxN; i++)
		{
			if (box_state[i] == 1)
			{
				for (unsigned short j = i + 1; j < boxN; j++)
				{
					if (box_state[j] == 1)
					{
						unsigned char direction = OM::check_Collision(&box[i].body.position, &box[j].body.position, box_size);
						if (direction != 0)
						{
							OM::impulse(&box[i].velocity, &box[j].velocity, direction);
							OM::inc_Bounce(&box_bounce[i], &box_bounce[j]);
						}
					}
				}
				for (unsigned char j = 1; j <= 4; j++)
				{
					if (OM::check_Collision(&box[i].body.position, box_size, &border[j].position, border_width[j], border_height[j]) == 1)
					{
						OM::impulse(&box[i].velocity, j);
						OM::inc_Bounce(&box_bounce[i]);
					}
				}
				if (box_bounce[i] == max_box_bounce) OM::despawn(&box_state[i], &box_bounce[i]);
				OM::update(&box[i].body.position, &box[i].velocity);
			}
		}

		OM::draw(canvas, &back, screen_width, screen_height, screen_width);

		for (unsigned short i = 0; i < boxN; i++) if (box_state[i] == 1) OM::draw(canvas, &box[i].body, box_size, box_size, screen_width);

		for (unsigned char i = 1; i <= 4; i++) OM::draw(canvas, &border[i], border_width[i], border_height[i], screen_width);

		memcpy(p, canvas, sizeof(Color_Set) * screen_area);
		SDL_BlitScaled(work_surface, NULL, print_surface, NULL);

		SDL_UpdateWindowSurface(window);
	}
}