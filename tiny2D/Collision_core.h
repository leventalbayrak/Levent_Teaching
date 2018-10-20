#pragma once
#include "Collision_data.h"
#include "Shape_core.h"
#include "Grid_core.h"
#include "Vec2D_core.h"
namespace Collision
{
	void point_Feeler_Pos(Point_Feeler *p, Shape::Rect *r)
	{
		/*
		        top
			    x
		  x            x

    left x              x right

		  x	           x
				x
				bottom
        */

		float margin = 0.05;
		p->bottom_feeler = { (float)(r->x + 0.5*r->w),(float)(r->y + r->h + r->h*margin) };
		p->top_feeler = { p->bottom_feeler.x,(r->y - r->h*margin)};
		p->left_feeler = { (r->x - r->h*margin),(float)(r->y + 0.5*r->h) };
		p->right_feeler = { (float)(r->x + r->w + r->w*margin), p->left_feeler.y };
		p->mid_feeler = { p->bottom_feeler.x,p->left_feeler.y };

		float diagonal_margin = 0.1;
		p->topleft_feeler = { p->left_feeler.x + r->w*diagonal_margin,p->top_feeler.y + r->h*diagonal_margin };
		p->topright_feeler = { p->right_feeler.x - r->w*diagonal_margin,p->top_feeler.y - r->h*diagonal_margin };
		p->bottomleft_feeler = { p->left_feeler.x + r->w*diagonal_margin,p->bottom_feeler.y - r->h*diagonal_margin };
		p->bottomright_feeler = { p->right_feeler.x - r->w*diagonal_margin,p->bottom_feeler.y - r->h*diagonal_margin };
	}

}