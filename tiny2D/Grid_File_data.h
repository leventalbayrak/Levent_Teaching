#pragma once

namespace Grid_File
{
	struct Grid_File
	{
		char ***table;
		unsigned int nrows;
		unsigned int *ncols;
	};
}