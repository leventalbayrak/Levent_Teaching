#pragma once
#include "ECS.h"
#include "SDL2-2.0.9/include/SDL.h"

unsigned char system_Bounce_Off_Walls_component_blueprint[maxncomponents] = { 0 };
unsigned char system_Bounce_Off_Walls_tag_blueprint[maxntags] = { 0 };

void system_Bounce_Off_Walls(ECS* e, int scrw, int scrh)
{
	//this determines what type of entities the system works on
	system_Bounce_Off_Walls_component_blueprint[_Position_Component] = 1;
	system_Bounce_Off_Walls_component_blueprint[_Motion_Component] = 1;
	system_Bounce_Off_Walls_tag_blueprint[_Bounce_Off_Walls_Tag] = 1;

	for (int i = 0; i < e->maxnentities; ++i)
	{
		if (e->used[i] == 0) continue;//skip unused
		//check blueprint
		if (compare_Blueprint(e->component_blueprint[i], system_Bounce_Off_Walls_component_blueprint, maxncomponents) == 0) continue;
		if (compare_Blueprint(e->tag_blueprint[i], system_Bounce_Off_Walls_tag_blueprint, maxntags) == 0) continue;
		
		//filtering is done, now do your thing

		if (e->pos.px[i] > scrw || e->pos.px[i] < 0)
		{
			e->motion.vx[i] *= -1.0f;
		}
		if (e->pos.py[i] > scrh || e->pos.py[i] < 0)
		{
			e->motion.vy[i] *= -1.0f;
		}
	}
}