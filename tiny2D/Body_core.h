#pragma once
#include "Body_data.h"
#include "Vec2D_core.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

namespace Body
{
	void init(Body *a, int array_size)
	{
		a->array_size = array_size;
		a->n_bodies = 0;

		a->dir = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*a->array_size);
		a->force = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*a->array_size);
		a->pos = (Vec2D::Vec2D*)malloc(sizeof(Vec2D::Vec2D)*a->array_size);
		a->speed = (float*)malloc(sizeof(float)*a->array_size);
		a->mass = (float*)malloc(sizeof(float)*a->array_size);
	}

	void clear_Forces(Body *a)
	{
		memset(a->force, 0, sizeof(Vec2D::Vec2D)*a->n_bodies);
	}

	int make(Body *a)
	{
		++a->n_bodies;
		return a->n_bodies - 1;
	}

	void modify(int index,Body *a, Vec2D::Vec2D *pos, float mass)
	{
		a->dir[index] = {};
		a->force[index] = {};
		a->speed[index] = 0;
		a->pos[index] = *pos;
		a->mass[index] = mass;
	}

	void add_Force(int index, Body *a, Vec2D::Vec2D *f)
	{
		Vec2D::add(&a->force[index], f);
	}

	void update(Body *a)
	{
		for (int i = 0; i < a->n_bodies; i++)
		{
			//implicit euler
			Vec2D::Vec2D accel = { a->force[i].x / a->mass[i],a->force[i].y / a->mass[i] };
			Vec2D::Vec2D vel = { a->dir[i].x*a->speed[i], a->dir[i].y*a->speed[i] };
			
			Vec2D::add(&vel, &accel);
			Vec2D::add(&a->pos[i], &vel);

			//recalculate direction and magnitude
			float mag = Vec2D::unit(&vel);
			a->speed[i] = mag;
			a->dir[i] = vel;
		}
	}
}