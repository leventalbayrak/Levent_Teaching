#pragma once
#include "Grid_data.h"
namespace Collision
{
	struct Point_Feeler
	{
		Vec2D::Vec2D bottom_feeler;
		Vec2D::Vec2D top_feeler;
		Vec2D::Vec2D left_feeler;
		Vec2D::Vec2D right_feeler;
		Vec2D::Vec2D mid_feeler;
	};
}
