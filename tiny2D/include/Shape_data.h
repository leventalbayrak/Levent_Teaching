#pragma once

namespace Shape
{
	namespace Rect
	{
		struct Data
		{
			float x, y, w, h;
		};

		struct Factory
		{
			Data *rect;
			int n_rect;
			int array_size;
		};
	}
}

