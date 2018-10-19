#pragma once
#include "Grid_data.h"
namespace Collision
{
	struct Point_Feeler
	{
		Grid::Point bottom_feeler;
		Grid::Point top_feeler;
		Grid::Point left_feeler;
		Grid::Point right_feeler;
		Grid::Point mid_feeler;
	};
}
