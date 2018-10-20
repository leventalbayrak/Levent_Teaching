#pragma once
#include "Collision_data.h"
#include "Shape_core.h"
#include "Grid_core.h"
#include "Vec2D_core.h"
namespace Collision
{
	void point_Feeler_Pos(Point_Feeler *p, Shape::Rect *r)
	{
		p->bottom_feeler = { (float)(r->x + 0.5*r->w),(float)(r->y + r->h) };
		p->top_feeler = { p->bottom_feeler.x,(r->y)};
		p->left_feeler = { (r->x),(float)(r->y + 0.5*r->h) };
		p->right_feeler = { (float)(r->x + 0.5*r->w), p->left_feeler.y };
		p->mid_feeler = { p->bottom_feeler.x,p->left_feeler.y };
	}

}