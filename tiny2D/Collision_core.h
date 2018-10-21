#pragma once
#include "Collision_data.h"
#include "Shape_core.h"
#include "Grid_core.h"
#include "Vec2D_core.h"
namespace Collision
{
	void box(Shape::Rect *out, const Shape::Rect *in, float shrink)
	{
		out->x = in->x + shrink * in->w;
		out->y = in->y + shrink * in->h;
		out->w = in->w - 2.0 * shrink * in->w;
		out->h = in->h - 2.0 * shrink * in->h;
	}

	void point_Feeler_Pos(Point_Feeler *p, const Shape::Rect *r, float t_margin, float x_margin)
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

		p->bottom_feeler = { (float)(r->x + 0.5*r->w),(float)(r->y + r->h + r->h*t_margin) };
		p->top_feeler = { p->bottom_feeler.x,(r->y - r->h*t_margin)};
		p->left_feeler = { (r->x - r->h*t_margin),(float)(r->y + 0.5*r->h) };
		p->right_feeler = { (float)(r->x + r->w + r->w*t_margin), p->left_feeler.y };
		p->mid_feeler = { p->bottom_feeler.x,p->left_feeler.y };

		p->topleft_feeler = { p->left_feeler.x + r->w*x_margin,p->top_feeler.y + r->h*x_margin };
		p->topright_feeler = { p->right_feeler.x - r->w*x_margin,p->top_feeler.y - r->h*x_margin };
		p->bottomleft_feeler = { p->left_feeler.x + r->w*x_margin,p->bottom_feeler.y - r->h*x_margin };
		p->bottomright_feeler = { p->right_feeler.x - r->w*x_margin,p->bottom_feeler.y - r->h*x_margin };
	}

}