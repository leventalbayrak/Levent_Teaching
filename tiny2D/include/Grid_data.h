#pragma once

namespace Grid
{
	//grid data, 1D array representation
	//index by data[y*n_cols+x]
	struct Grid
	{
		int n_rows;
		int n_cols;
		int *data;
	};

	struct Point
	{
		int x, y;
	};

	//top left and bottom right cells in a grid
	struct Region
	{
		int x0, y0, x1, y1;
	};
}