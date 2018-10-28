#pragma once
#include "Tileset_data.h"
#include "Shape_data.h"
namespace Font
{
	struct Font
	{
		Shape::Rect::Data screen_coord;
		Tileset::Tileset tileset;
	};
}