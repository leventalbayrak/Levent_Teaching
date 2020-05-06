#pragma once
#include "ECS.h"
#include "SDL2-2.0.9/include/SDL.h"

unsigned char system_Pawn_Input_component_blueprint[maxncomponents] = { 0 };
unsigned char system_Pawn_Input_tag_blueprint[maxntags] = { 0 };

void system_Pawn_Input(ECS* e, int horizontal, int vertical, float forceamount)
{
	//this determines what type of entities the system works on
	system_Pawn_Input_component_blueprint[_Motion_Component] = 1;
	system_Pawn_Input_tag_blueprint[_Pawn_Input_Tag] = 1;

	for (int i = 0; i < e->maxnentities; ++i)
	{
		if (e->used[i] == 0) continue;//skip unused
		//check blueprint
		if (compare_Blueprint(e->component_blueprint[i], system_Pawn_Input_component_blueprint, maxncomponents) == 0) continue;
		if (compare_Blueprint(e->tag_blueprint[i], system_Pawn_Input_tag_blueprint, maxntags) == 0) continue;

		//filtering is done, now do your thing
		float dx = horizontal;
		float dy = vertical;
		float mag = dx * dx + dy * dy;
		if (mag != 0.0f)
		{
			mag = sqrt(mag);
			dx /= mag;
			dy /= mag;		
		}

		e->motion.ax[i] += dx * forceamount;
		e->motion.ay[i] += dy * forceamount;
	}
}