#pragma once
#include "ECS.h"
#include "SDL2-2.0.9/include/SDL.h"

unsigned char system_Render_Color_blueprint[maxncomponents] = { 0 };

void system_Render_Color(ECS* e, SDL_Renderer* renderer)
{
	//this determines what type of entities the system works on
	system_Render_Color_blueprint[_Position_Component] = 1;
	system_Render_Color_blueprint[_Size_Component] = 1;
	system_Render_Color_blueprint[_Color_Component] = 1;

	for (int i = 0; i < e->maxnentities; ++i)
	{
		if (e->used[i] == 0) continue;//skip unused
		//check blueprint
		if (compare_Blueprint(e->component_blueprint[i], system_Render_Color_blueprint, maxncomponents) == 0) continue;

		//filtering is done, now do your thing

		SDL_Rect rect;
		rect.x = e->pos.px[i];
		rect.y = e->pos.py[i];
		rect.w = e->size.w[i];
		rect.h = e->size.h[i];
		SDL_SetRenderDrawColor(renderer, e->color.r[i], e->color.g[i], e->color.b[i], 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}