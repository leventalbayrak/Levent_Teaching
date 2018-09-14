#include "Fast_Windows_Console.h"
#pragma warning(disable:4996)


namespace Body
{
	struct Mass
	{
		float mass;
	};
	struct Force
	{
		float fx, fy;
	};
	struct Pos
	{
		float px, py;
	};
	struct Vel
	{
		float vx, vy;
	};

	struct Bodies
	{
		Mass *m;
		Force *f;
		Pos *p;
		Vel *v;
		int n_bodies;
		int array_size;
	};

	void init(Bodies *b)
	{
		b->n_bodies = 0;
		b->array_size = 100;
		b->m = (Mass*)malloc(sizeof(Mass)*b->array_size); assert(b->m);
		b->f = (Force*)malloc(sizeof(Force)*b->array_size); assert(b->f);
		b->p = (Pos*)malloc(sizeof(Pos)*b->array_size); assert(b->p);
		b->v = (Vel*)malloc(sizeof(Vel)*b->array_size); assert(b->v);
	}

	void resize(Bodies *b)
	{
		b->array_size += b->array_size >> 1;
		b->m = (Mass*)realloc(b->m, sizeof(Mass)*b->array_size); assert(b->m);
		b->f = (Force*)realloc(b->f, sizeof(Force)*b->array_size); assert(b->f);
		b->p = (Pos*)realloc(b->p, sizeof(Pos)*b->array_size); assert(b->p);
		b->v = (Vel*)realloc(b->v, sizeof(Vel)*b->array_size); assert(b->v);
	}

	int create(Bodies *b)
	{
		if (b->n_bodies >= b->array_size) resize(b);
		b->m[b->n_bodies] = {};
		b->f[b->n_bodies] = {};
		b->p[b->n_bodies] = {};
		b->v[b->n_bodies] = {};
		return ++b->n_bodies - 1;
	}

	void force(Force *f, float fx, float fy)
	{
		f->fx += fx;
		f->fy += fy;
	}

	//implicit euler
	void update(Pos *p, Vel *v, const Force *f, const Mass *m, float dt)
	{
		float ax = f->fx / m->mass;
		float ay = f->fy / m->mass;

		v->vx += ax*dt;
		v->vy += ay*dt;
		p->px += v->vx*dt;
		p->py += v->vy*dt;
	}

}

void debug_Print(int x, int y, Body::Bodies *b, int index)
{
	static char msg[1024];
	FWC::color(0x0F);
	sprintf(msg, "pos: %.2f,%.2f", b->p[index].px, b->p[index].py);
	FWC::put(x, y++, msg);
	sprintf(msg, "vel: %.2f,%.2f", b->v[index].vx, b->v[index].vy);
	FWC::put(x, y++, msg);

	FWC::put(x, y++, "press WASD to move");
}

int main()
{

	FWC::init();

	Body::Bodies b;
	Body::init(&b);

	int actor = Body::create(&b);
	b.p[actor].px = 40;
	b.p[actor].py = 24;
	b.m[actor].mass = 1.0;

	int floor_y = FWC::height() - 1;

	float dt = 1.0;
	float v_friction = 0.07;
	float v_air_friction = 0.0005;
	float v_max = 0.1;
	float f_jump = 0.07;
	float f_move = 0.011;
	float f_gravity = 0.0006;
	for (;;)
	{
		FWC::clear();

		FWC::update_Input();

		b.f[actor].fx = 0;
		b.f[actor].fy = 0;

		b.f[actor].fy += f_gravity;
		if (FWC::key_State('W') == 1 && FWC::key_Last_State('W') == 0)
		{
			if (b.p[actor].py >= floor_y - 1)
			{
				b.f[actor].fy += -f_jump;
			}
		}
		if (FWC::key_State('A') == 1)
		{
			if (b.p[actor].py >= floor_y - 3)
			{
				b.f[actor].fx += -f_move;
			}
		}
		if (FWC::key_State('D') == 1)
		{
			if (b.p[actor].py >= floor_y - 3)
			{
				b.f[actor].fx += f_move;
			}
		}
		Body::update(&b.p[actor], &b.v[actor], &b.f[actor], &b.m[actor], dt);

		if (b.p[actor].px <= 0)
		{
			b.p[actor].px = 0;
			b.v[actor].vx = 0;
		}
		if (b.p[actor].px >= FWC::width() - 1)
		{
			b.p[actor].px = FWC::width() - 1;
			b.v[actor].vx = 0;
		}
		if (b.p[actor].py <= 0)
		{
			b.p[actor].py = 0;
			b.v[actor].vy = 0;
		}
		if (b.p[actor].py >= floor_y-1)
		{
			b.p[actor].py = floor_y-1;
			b.v[actor].vy = 0;
		}

		if (b.v[actor].vx*b.v[actor].vx > v_max*v_max)
		{
			if (b.v[actor].vx < 0) b.v[actor].vx = -v_max;
			else b.v[actor].vx = v_max;
		}

		if (b.p[actor].py == floor_y-1)
		{
			b.v[actor].vx *= 1.0 - v_friction;
			if (b.v[actor].vx *b.v[actor].vx < 0.0001) b.v[actor].vx = 0;
		}
		else
		{
			b.v[actor].vx *= 1.0 - v_air_friction;
		}
		
		FWC::color(0xA0);
		FWC::horizontal_Line(0, floor_y, FWC::width(), '^');

		FWC::color(0xF0);
		FWC::put(b.p[actor].px, b.p[actor].py, '_');

		debug_Print(0, 0, &b, actor);

		FWC::present();
	}

	return 0;
}