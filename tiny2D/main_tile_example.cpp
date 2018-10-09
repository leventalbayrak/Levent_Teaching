#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include "SDL2-2.0.8\include\SDL_mixer.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")

#pragma comment(linker,"/subsystem:console")

struct Vec2D
{
	float x;
	float y;
};

struct RGB
{
	unsigned char r, g, b;
};

namespace Tileset
{
	struct Tileset
	{
		SDL_Texture *tex;
		int tile_w;
		int tile_h;
		int n_rows;
		int n_cols;
	};

	void load(Tileset *t, char *image_filename, int tile_w, int tile_h, SDL_Renderer *renderer)
	{
		SDL_Surface *sprite_surface = IMG_Load(image_filename);
		assert(sprite_surface);
		SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);

		SDL_FreeSurface(sprite_surface);

		t->tile_w = tile_w;
		t->tile_h = tile_h;
		t->tex = sprite_texture;
	}

	void draw_Tile(int dest_x, int dest_y, float stretch,float alpha, int row, int col, Tileset *t, SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = col * t->tile_w;
		src.y = row * t->tile_h;
		src.w = t->tile_w;
		src.h = t->tile_h;

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = t->tile_w*stretch;
		dest.h = t->tile_h*stretch;

		SDL_SetTextureAlphaMod(t->tex, alpha*255.0);
		SDL_RenderCopyEx(renderer, t->tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}
}

//an emitter emits
//same color&size particles
//has a position
//also force
//min and max lifetime
//max number of particles
//min and maximum interval to emit

//each particle needs a
//position
//force vector
//velocity
//lifetime

namespace Particle_Emitter
{
	struct Particle_Emitter
	{
		Vec2D *pos;
		Vec2D *force;
		Vec2D *vel;
		float *life;
		int *state;
		int n_particles;

		float emitter_mass;
		float particle_mass;
		Vec2D emitter_pos;
		Vec2D emitter_force;
		Vec2D emitter_vel;

		RGB particle_color;
		float particle_size;
		
	};

	void init(Particle_Emitter *p, int n_particles)
	{
		p->n_particles = n_particles;

		//allocations
		p->pos = new Vec2D[p->n_particles];
		p->force = new Vec2D[p->n_particles];
		p->vel = new Vec2D[p->n_particles];
		p->life = new float[p->n_particles];
		p->state = new int[p->n_particles];

		memset(p->pos, 0, sizeof(Vec2D)*p->n_particles);
		memset(p->force, 0, sizeof(Vec2D)*p->n_particles);
		memset(p->vel, 0, sizeof(Vec2D)*p->n_particles);
		memset(p->life, 0, sizeof(float)*p->n_particles);
		memset(p->state, 0, sizeof(int)*p->n_particles);

		p->emitter_force = {};
		p->emitter_vel = {};
		p->emitter_pos = { 50,50 };//TODO
		p->particle_color = { 165,75,0 };
		p->particle_size = 4.0;
		p->emitter_mass = 1.0;
		p->particle_mass = 0.1;

	}

	void spawn_Many(Particle_Emitter *p, Vec2D influence_min, Vec2D influence_max, int how_many, float min_life, float max_life)
	{
		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] == 0)
			{
				if (--how_many < 0) break;//TODO

				p->state[i] = 1;

				p->pos[i] = p->emitter_pos;
				p->vel[i] = p->emitter_vel;
				p->life[i] = min_life + (max_life-min_life)*rand()/RAND_MAX;
				p->force[i] = {};

				p->force[i].x += influence_min.x + (influence_max.x - influence_min.x)*rand() / RAND_MAX;
				p->force[i].y += influence_min.y + (influence_max.y - influence_min.y)*rand() / RAND_MAX;	
			}
		}
	}

	void clear_Forces_from_Particles(Particle_Emitter *p)
	{
		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] != 0) p->force[i] = {};
		}
	}

	void add_Force_to_Particles(Particle_Emitter *p, Vec2D f)
	{
		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] != 0)
			{
				p->force[i].x += f.x;
				p->force[i].y += f.y;
			}
		}
	}

	void update(Particle_Emitter *p, float time_elapsed)
	{
		//implicit euler, mass=1.0
		Vec2D accel; 
		accel.x = p->emitter_force.x / p->emitter_mass;
		accel.y = p->emitter_force.y / p->emitter_mass;

		p->emitter_vel.x += accel.x*time_elapsed;
		p->emitter_vel.y += accel.y*time_elapsed;
		p->emitter_pos.x += p->emitter_vel.x*time_elapsed;
		p->emitter_pos.y += p->emitter_vel.y*time_elapsed;

		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] == 0) continue;
		
			p->life[i] -= time_elapsed;
			if (p->life[i] <= 0.0)
			{
				p->state[i] = 0;
				continue;
			}

			Vec2D accel;
			accel.x = p->force[i].x / p->particle_mass;
			accel.y = p->force[i].y / p->particle_mass;
			p->vel[i].x += accel.x*time_elapsed;
			p->vel[i].y += accel.y*time_elapsed;
			p->pos[i].x += p->vel[i].x*time_elapsed;
			p->pos[i].y += p->vel[i].y*time_elapsed;	
		}
	}

	void draw(Particle_Emitter *p, SDL_Renderer *renderer)
	{
		SDL_SetRenderDrawColor(renderer, p->particle_color.r, p->particle_color.g, p->particle_color.b, 255);
		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] == 0) continue;

			SDL_Rect rect = { p->pos[i].x,p->pos[i].y,p->particle_size, p->particle_size };
			SDL_RenderFillRect(renderer, &rect);
		}
	}

}

namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(0));

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	Tileset::Tileset tileset;
	Tileset::load(&tileset, "tileset.png", 32, 32, Game::renderer);

	Particle_Emitter::Particle_Emitter rocket;
	Particle_Emitter::init(&rocket, 200);
	rocket.emitter_pos.x = Game::screen_width / 2.0;
	rocket.emitter_pos.y = Game::screen_height / 2.0;

	Particle_Emitter::Particle_Emitter sparks_big;
	Particle_Emitter::init(&sparks_big, 50);
	sparks_big.particle_color = { 255,60,0 };
	sparks_big.particle_size = 6;
	sparks_big.particle_mass = 1.0;

	Particle_Emitter::Particle_Emitter sparks_small;
	Particle_Emitter::init(&sparks_small, 50);
	sparks_small.particle_color = { 120,180,0 };
	sparks_small.particle_size = 4;
	sparks_small.particle_mass = 0.25;
	

	Vec2D f_gravity = { 0.0,0.00005 };

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	bool done = false;
	while (!done)
	{
		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		int cmd_UP = 0;
		int cmd_LEFT = 0;
		int cmd_RIGHT = 0;
		int cmd_DOWN = 0;

		if (keys[SDL_SCANCODE_W]) cmd_UP = 1;
		if (keys[SDL_SCANCODE_S]) cmd_DOWN = 1;
		if (keys[SDL_SCANCODE_A]) cmd_LEFT = 1;
		if (keys[SDL_SCANCODE_D]) cmd_RIGHT = 1;

		/*
		GAME CODE
		*/

		//every frame
		//clear forces on emitter
		rocket.emitter_force = {};
		sparks_big.emitter_force = {};
		sparks_small.emitter_force = {};
		//clear forces on particles
		Particle_Emitter::clear_Forces_from_Particles(&rocket);
		Particle_Emitter::clear_Forces_from_Particles(&sparks_big);
		Particle_Emitter::clear_Forces_from_Particles(&sparks_small);

		//apply gravity to emitter
		rocket.emitter_force.x += f_gravity.x;
		rocket.emitter_force.y += f_gravity.y;
		//apply gravity to particles
		Particle_Emitter::add_Force_to_Particles(&rocket, f_gravity);
		Particle_Emitter::add_Force_to_Particles(&sparks_big, f_gravity);
		Particle_Emitter::add_Force_to_Particles(&sparks_small, f_gravity);

		//apply user input forces to particles
		if (cmd_UP)
		{
			Vec2D f_min = { -0.005, 0.001 };
			Vec2D f_max = { 0.005, 0.05 };
			Particle_Emitter::spawn_Many(&rocket, f_min, f_max, 1, 100, 500);

			rocket.emitter_force.y += -0.0001;
		}
		if (cmd_DOWN)
		{
			Vec2D f_min = { -0.005, -0.05 };
			Vec2D f_max = { 0.005, -0.001 };
			Particle_Emitter::spawn_Many(&rocket, f_min, f_max, 1, 100, 500);

			rocket.emitter_force.y += 0.0001;
		}
		if (cmd_LEFT)
		{
			Vec2D f_min = { 0.001, -0.005 };
			Vec2D f_max = { 0.05, 0.005 };
			Particle_Emitter::spawn_Many(&rocket, f_min, f_max, 1, 100, 500);

			rocket.emitter_force.x += -0.0001;
		}
		if (cmd_RIGHT)
		{
			Vec2D f_min = { -0.05, -0.005 };
			Vec2D f_max = { -0.001, 0.005 };
			Particle_Emitter::spawn_Many(&rocket, f_min, f_max, 1, 100, 500);

			rocket.emitter_force.x += 0.0001;
		}

		//update all rocket physics (emitter+particles)
		Particle_Emitter::update(&rocket, 1.0);

		//do not let emitter leave the screen
		if (rocket.emitter_pos.x <= 0)
		{
			rocket.emitter_vel.x *= -1;
			//spawn some sparks
			Vec2D f_min = { 0.001, -0.01 };
			Vec2D f_max = { 0.1, 0.01 };
			//spawn sparks where the rocket emitter is
			sparks_big.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_big, f_min, f_max, 5, 100, 600);

			sparks_small.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_small, f_min, f_max, 20, 100, 1200);
		}
		if (rocket.emitter_pos.x >= Game::screen_width)
		{
			rocket.emitter_vel.x *= -1;
			//spawn some sparks
			Vec2D f_min = { -0.1, -0.01 };
			Vec2D f_max = { -0.001, 0.01 };
			//spawn sparks where the rocket emitter is
			sparks_big.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_big, f_min, f_max, 5, 100, 600);

			sparks_small.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_small, f_min, f_max, 20, 100, 1200);
		}
		if (rocket.emitter_pos.y <= 0)
		{
			rocket.emitter_vel.y *= -1;
			//spawn some sparks
			Vec2D f_min = {-0.01, 0.001};
			Vec2D f_max = {0.01, 0.1};
			//spawn sparks where the rocket emitter is
			sparks_big.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_big, f_min, f_max, 5, 100, 600);

			sparks_small.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_small, f_min, f_max, 20, 100, 1200);
		}
		if (rocket.emitter_pos.y >= Game::screen_height)
		{
			rocket.emitter_vel.y *= -1;
			//spawn some sparks
			Vec2D f_min = {-0.01, -0.1};
			Vec2D f_max = {0.01, -0.001};
			//spawn sparks where the rocket emitter is
			sparks_big.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_big, f_min, f_max, 5, 100, 600);

			sparks_small.emitter_pos = rocket.emitter_pos;
			Particle_Emitter::spawn_Many(&sparks_small, f_min, f_max, 20, 100, 1200);
		}

		//update all spark physics (emitter+particles)
		Particle_Emitter::update(&sparks_big, 1.0);
		Particle_Emitter::update(&sparks_small, 1.0);

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 255, 255);
		SDL_Rect rect = { rocket.emitter_pos.x,rocket.emitter_pos.y,5,5 };
		SDL_RenderFillRect(Game::renderer, &rect);

		//draw rocket boost
		Particle_Emitter::draw(&rocket, Game::renderer);
		//draw sparks
		Particle_Emitter::draw(&sparks_big, Game::renderer);
		Particle_Emitter::draw(&sparks_small, Game::renderer);

		int grass_patch_x = 0;
		int grass_patch_y = 0;
		//draw grass, 8x8 patch
		for (int i = 0; i < 8; i++)
		{
			grass_patch_x = 0;
			for (int j = 0; j < 8; j++)
			{
				Tileset::draw_Tile(grass_patch_x, grass_patch_y, 1.0, 1.0, 0, 0, &tileset, Game::renderer);
				grass_patch_x += tileset.tile_w;
			}
			grass_patch_y += tileset.tile_h;
		}
		
		//draw 6 trees
		for (int i = 0; i < 6; i++)
		{
			int tree_x = tileset.tile_w*(rand()%8);
			int tree_y = tileset.tile_h*(rand() % 8);
			Tileset::draw_Tile(tree_x,tree_y, 1.0, 1.0, 1, 1, &tileset, Game::renderer);
		}
		

		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}
