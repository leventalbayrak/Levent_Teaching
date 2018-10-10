#pragma once

namespace Grid
{
	struct Grid
	{
		int n_rows;
		int n_cols;
		int *data;
	};

	struct Active_Index_Range_Out
	{
		int x0, y0, x1, y1;//top left and bottom right cells
	};
}