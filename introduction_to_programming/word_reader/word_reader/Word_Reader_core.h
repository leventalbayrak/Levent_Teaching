#pragma once
#include "Word_Reader_data.h"
#include "String_core.h"
#include <stdio.h>
#include <stdlib.h>

namespace WR
{

	namespace internal
	{
		void resize(Data *data)
		{
			data->array_size += data->array_size >> 1;
			data->words = (char**)realloc(data->words, sizeof(char*)*data->array_size);
		}

	}

	void load_File(Data *data, const char *filename)
	{
		FILE *f = fopen(filename, "r");

		data->n_words = 0;
		data->array_size = 1024;
		data->words = (char**)malloc(sizeof(char*)*data->array_size);

		for (;;)
		{
			char tmp_str[256];

			int r = fscanf(f, "%s", tmp_str);

			if (r != 1) break;

			if (data->n_words >= data->array_size)
			{
				internal::resize(data);
			}

			data->words[data->n_words] = (char*)malloc(sizeof(char) * 256);

			Str_Ops::copy(data->words[data->n_words], tmp_str);

			data->n_words++;

		}

	}

}
