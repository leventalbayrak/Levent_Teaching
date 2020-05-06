#pragma once
#include <stdlib.h>

//--ADD COMPONENTS HERE

struct Position_Component
{
	float* px;
	float* py;
};

struct Motion_Component
{
	float* vx;
	float* vy;
	float* ax;
	float* ay;
};

struct Color_Component
{
	int* r;
	int* g;
	int* b;
};

struct Size_Component
{
	float* w;
	float* h;
};

//just need integers to identify these
enum
{
	_Position_Component=0,
	_Motion_Component,
	_Color_Component,
	_Size_Component
};

enum
{
	_Bounce_Off_Walls_Tag=0,
	_Pawn_Input_Tag
};

int compare_Blueprint(unsigned char* entity, unsigned char* system, int n)
{
	//-1 mean exclude entities containing the component
	for (int i = 0; i < n; ++i)
	{
		if (system[i] == 1 && entity[i] == 0) return 0;
		if (system[i] == -1 && entity[i] == 1) return 0;
	}

	return 1;
}

const int maxncomponents = 128;
const int maxntags = 512;
struct ECS
{
	//--ADD COMPONENTS HERE
	Position_Component pos;
	Motion_Component motion;
	Color_Component color;
	Size_Component size;

	//---- DO NOT TOUCH BELOW THIS
	unsigned char* used;
	unsigned char** component_blueprint;
	unsigned char** tag_blueprint;
	int maxnentities;
};

//--ADD COMPONENTS HERE
void init(ECS* e, int maxnentities)
{
	e->maxnentities = maxnentities;
	e->pos.px = (float*)malloc(sizeof(float) * maxnentities);
	e->pos.py = (float*)malloc(sizeof(float) * maxnentities);
	e->motion.vx = (float*)malloc(sizeof(float) * maxnentities);
	e->motion.vy = (float*)malloc(sizeof(float) * maxnentities);
	e->motion.ax = (float*)malloc(sizeof(float) * maxnentities);
	e->motion.ay = (float*)malloc(sizeof(float) * maxnentities);
	e->color.r = (int*)malloc(sizeof(int) * maxnentities);
	e->color.g = (int*)malloc(sizeof(int) * maxnentities);
	e->color.b = (int*)malloc(sizeof(int) * maxnentities);
	e->size.w = (float*)malloc(sizeof(float) * maxnentities);
	e->size.h = (float*)malloc(sizeof(float) * maxnentities);

	e->used = (unsigned char*)calloc(maxnentities, sizeof(unsigned char));
	e->component_blueprint = (unsigned char**)malloc(sizeof(unsigned char*) * maxnentities);
	e->tag_blueprint = (unsigned char**)malloc(sizeof(unsigned char*) * maxnentities);
	for (int i = 0; i < maxnentities; ++i) e->component_blueprint[i] = (unsigned char*)calloc(maxncomponents, sizeof(unsigned char));
	for (int i = 0; i < maxnentities; ++i) e->tag_blueprint[i] = (unsigned char*)calloc(maxntags, sizeof(unsigned char));

}

int create_Entity(ECS* e)
{
	for (int i = 0; i < e->maxnentities; ++i)
	{
		if (e->used[i] == 0)
		{
			e->used[i] = 1;
			return i;
		}
	}

	return -1;
}

void add_Component(int entityid, int componentid, ECS* e)
{
	e->component_blueprint[entityid][componentid] = 1;
}

void remove_Component(int entityid, int componentid, ECS* e)
{
	e->component_blueprint[entityid][componentid] = 0;
}

void add_Tag(int entityid, int tagid, ECS* e)
{
	e->tag_blueprint[entityid][tagid] = 1;
}

void remove_Tag(int entityid, int tagid, ECS* e)
{
	e->tag_blueprint[entityid][tagid] = 0;
}
