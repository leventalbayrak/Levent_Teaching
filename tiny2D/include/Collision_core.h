#pragma once
#include "Collision_data.h"
#include "Shape_core.h"
#include "Grid_core.h"
#include "Vec_core.h"
#include  "Shape_core.h"

namespace Collision
{
	void impulse(Shape::Rect::Data *a_rect,Vec2D::Vec2D *a_vel, float a_inv_mass, Shape::Rect::Data *b_rect, Vec2D::Vec2D *b_vel,  float b_inv_mass)
	{
		Vec2D::Vec2D normal = { (a_rect->x + a_rect->w*0.5) - (b_rect->x + b_rect->w*0.5), (a_rect->y + a_rect->h*0.5) - (b_rect->y + b_rect->h*0.5) };
		Vec2D::norm(&normal);

		Vec2D::Vec2D relative_vel = *b_vel;
		Vec2D::sub(&relative_vel, a_vel);

		float d = Vec2D::dot(&relative_vel, &normal);

		if (d < 0) return;

		float e = 1.0;//1.0 is for elastic bounce, 0.0 for mud
		float j = -(1.0 + e) * d / (a_inv_mass + b_inv_mass);//mass of tile is infinity (1/ma+1/mb), if you use actual mass then objects will bounce
		Vec2D::Vec2D impulse_a = normal;
		Vec2D::scale(&impulse_a, j*a_inv_mass);
		Vec2D::Vec2D impulse_b = normal;
		Vec2D::scale(&impulse_b, j*b_inv_mass);

		Vec2D::sub(a_vel, &impulse_a);
		Vec2D::add(b_vel, &impulse_b);
	}

	//impulse against immovable object
	void impulse(Shape::Rect::Data *a_rect, Vec2D::Vec2D *a_vel, float a_inv_mass, Shape::Rect::Data *b_rect)
	{
		Vec2D::Vec2D normal = { (a_rect->x + a_rect->w*0.5) - (b_rect->x + b_rect->w*0.5), (a_rect->y + a_rect->h*0.5) - (b_rect->y + b_rect->h*0.5) };
		Vec2D::norm(&normal);

		Vec2D::Vec2D relative_vel = {-a_vel->x,-a_vel->y};

		float d = Vec2D::dot(&relative_vel, &normal);

		if (d < 0) return;

		float e = 1.0;//1.0 is for elastic bounce, 0.0 for mud
		float j = -(1.0 + e) * d / (a_inv_mass);//mass of tile is infinity (1/ma+1/mb), if you use actual mass then objects will bounce
		Vec2D::Vec2D impulse = normal;
		Vec2D::scale(&impulse, j);

		Vec2D::sub(a_vel, &impulse);
	}
}

