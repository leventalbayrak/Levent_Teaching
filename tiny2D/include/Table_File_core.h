#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include <cfloat>
#include "Table_File_data.h"

namespace Table_File
{

	void read(const char *filename, Table_File *t);

	void clean(Table_File *t);

}

namespace Table_File
{

	void read(const char *filename, Table_File *t)
	{
		FILE *f = fopen(filename, "rb");
		assert(f != NULL);

		fseek(f, 0, SEEK_END);
		long long int filesize = ftell(f);
		assert(filesize > 0);

		fseek(f, 0, SEEK_SET);
		char *buffer = new char[filesize + 1];
		assert(buffer != NULL);

		fread(buffer, filesize, sizeof(char), f);
		assert(filesize > 0);
		buffer[filesize] = 0;
		fclose(f);

		long long int newline_count = 0;

		char *format_str[2] = { "unix","dos" };
		const char *line_ending[2] = { "\n","\r\n" };

		int format = 0; //0 unix, 1 dos

		for (long long int i = 0; i < filesize; i++)
		{
			if (buffer[i] == '\r')
			{
				format = 1; //dos uses \r\n
			}
			if (buffer[i] == '\n')
			{
				newline_count++;//linux uses \n
			}

		}

		if (buffer[filesize - 1] != '\n' && buffer[filesize - 1] != '\r')
		{
			newline_count++;
		}

		t->nrows = newline_count;

		t->ncols = new unsigned int[t->nrows];
		assert(t->ncols != NULL);

		long long int rows_read = 0;
		char **lines = new char *[t->nrows];
		assert(lines != NULL);

		char *line = strtok(buffer, line_ending[format]);
		while (line != NULL)
		{
			lines[rows_read++] = line;
			line = strtok(NULL, line_ending[format]);
		}
		
		assert(rows_read == newline_count);

		t->table = new char **[t->nrows];
		assert(t->table != NULL);

		for (long long int i = 0; i < t->nrows; i++)
		{
			unsigned int line_length = strlen(lines[i]);
			char *s = lines[i];
			unsigned int k = 0;
			for (unsigned int j = 0; j < line_length; j++)
			{
				if (s[j] == '\t' || s[j] == ',') k++;
			}
			long long int possible_number_of_columns = k + 1;

			t->table[i] = new char *[possible_number_of_columns];
			assert(t->table[i] != NULL);

			long long int cols_read = 0;
			char *st = strtok(lines[i], "\t,");
			while (st != NULL)
			{
				t->table[i][cols_read] = new char[strlen(st) + 1];
				assert(t->table[i][cols_read] != NULL);
				strcpy(t->table[i][cols_read], st);
				cols_read++;
				st = strtok(NULL, "\t,");
			}

			t->ncols[i] = cols_read;
		}

		delete[] buffer;
		delete[] lines;

	}

	void clean(Table_File *t)
	{
		for (unsigned int i = 0; i < t->nrows; i++)
		{
			for (unsigned int j = 0; j < t->ncols[i]; j++)
			{
				delete[] t->table[i][j];
			}
			delete[] t->table[i];
		}
		delete[] t->ncols;
		delete[] t->table;
		memset(t, 0, sizeof(Table_File));
	}

}