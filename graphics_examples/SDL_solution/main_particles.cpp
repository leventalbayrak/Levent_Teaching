#include <iostream>
#include <assert.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")

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

	}

	void spawn(Particle_Emitter *p, Vec2D influence_vector, int how_many)
	{
		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] == 0)
			{
				p->state[i] = 1;

				p->pos[i] = p->emitter_pos;
				p->vel[i] = p->emitter_vel;
				p->life[i] = 1000.0;//TODO
				p->force[i] = {};

				p->force[i].x += influence_vector.x;
				p->force[i].y += influence_vector.y;

				if (--how_many <= 0) break;//TODO
			}
		}
	}

	void update(Particle_Emitter *p, float time_elapsed)
	{
		//implicit euler, mass=1.0
		Vec2D accel = p->emitter_force;

		p->emitter_vel.x += accel.x*time_elapsed;
		p->emitter_vel.y += accel.y*time_elapsed;
		p->emitter_pos.x += p->emitter_vel.x*time_elapsed;
		p->emitter_pos.y += p->emitter_vel.y*time_elapsed;

		for (int i = 0; i < p->n_particles; i++)
		{
			if (p->state[i] == 0) continue;
			
			Vec2D accel = p->force[i];
			p->vel[i].x += accel.x*time_elapsed;
			p->vel[i].y += accel.y*time_elapsed;
			p->pos[i].x += p->vel[i].x*time_elapsed;
			p->pos[i].y += p->vel[i].y*time_elapsed;
		
			p->life[i] -= time_elapsed;
			if (p->life[i] <= 0.0)
			{
				p->state[i] = 0;
			}
		}
	}

	void draw(Particle_Emitter *p,SDL_Renderer *renderer)
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

	

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);


	Particle_Emitter::Particle_Emitter emitter;
	Particle_Emitter::init(&emitter, 1000);
	emitter.emitter_pos.x = Game::screen_width / 2.0;
	emitter.emitter_pos.y = Game::screen_height / 2.0;

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
		
		emitter.emitter_force = {};
		for (int i = 0; i < emitter.n_particles; i++)
		{
			if (emitter.state[i] == 0) continue;
			emitter.force[i] = {};
		}

		if (keys[SDL_SCANCODE_W])
		{
			Vec2D f = { 0.0, 0.1 };
			float dampen = 0.01;
			f.x += 4.0*dampen * (1.0 - 2.0*rand() / RAND_MAX);
			f.y += dampen * (1.0 - 2.0*rand() / RAND_MAX);
			Particle_Emitter::spawn(&emitter,f, 1);

			emitter.emitter_force.y = -0.0001;
		}
		if (keys[SDL_SCANCODE_S])
		{
			Vec2D f = { 0.0, -0.1 };
			float dampen = 0.01;
			f.x += 4.0*dampen * (1.0 - 2.0*rand() / RAND_MAX);
			f.y += dampen * (1.0 - 2.0*rand() / RAND_MAX);
			Particle_Emitter::spawn(&emitter, f, 1);

			emitter.emitter_force.y = 0.0001;
		}
		if (keys[SDL_SCANCODE_A])
		{
			Vec2D f = { 0.1, 0.0 };
			float dampen = 0.01;
			f.x += 4.0*dampen * (1.0 - 2.0*rand() / RAND_MAX);
			f.y += dampen * (1.0 - 2.0*rand() / RAND_MAX);
			Particle_Emitter::spawn(&emitter, f, 1);

			emitter.emitter_force.x = -0.0001;
		}
		if (keys[SDL_SCANCODE_D])
		{
			Vec2D f = { -0.1, 0.0 };
			float dampen = 0.01;
			f.x += 4.0*dampen * (1.0 - 2.0*rand() / RAND_MAX);
			f.y += dampen * (1.0 - 2.0*rand() / RAND_MAX);
			Particle_Emitter::spawn(&emitter, f, 1);

			emitter.emitter_force.x = 0.0001;
		}


		/*
		GAME CODE
		*/

		Particle_Emitter::update(&emitter, 1.0);

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 255, 255);
		SDL_Rect rect = { emitter.emitter_pos.x,emitter.emitter_pos.y,5,5 };
		SDL_RenderFillRect(Game::renderer, &rect);

		Particle_Emitter::draw(&emitter, Game::renderer);

		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}