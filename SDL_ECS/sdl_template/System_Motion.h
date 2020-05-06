#pragma once
#include "ECS.h"
#include "SDL2-2.0.9/include/SDL.h"

unsigned char system_Motion_blueprint[maxncomponents] = { 0 };

void system_Motion(ECS* e, float dt)
{
	//this determines what type of entities the system works on
	system_Motion_blueprint[_Position_Component] = 1;
	system_Motion_blueprint[_Motion_Component] = 1;

	for (int i = 0; i < e->maxnentities; ++i)
	{
		if (e->used[i] == 0) continue;//skip unused
		//check blueprint
		if (compare_Blueprint(e->component_blueprint[i], system_Motion_blueprint, maxncomponents) == 0) continue;

		//filtering is done, now do your thing

		e->motion.vx[i] += e->motion.ax[i] * dt;
		e->motion.vy[i] += e->motion.ay[i] * dt;
		e->pos.px[i] += e->motion.vx[i] * dt;
		e->pos.py[i] += e->motion.vy[i] * dt;

		//reset forces
		e->motion.ax[i] = 0.0f;
		e->motion.ay[i] = 0.0f;
	}
}