#pragma once

namespace Table_File
{
	struct Table_File
	{
		char ***table;
		unsigned int nrows;
		unsigned int *ncols;
	};
}