#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

#define READ(variable) int variable = 0; fscanf(f_in,"%d", &variable)


int main(int argc, char **argv)
{
	FILE* f_out = stdout;// fopen(getenv("OUTPUT_PATH"), "w");
	FILE *f_in =  fopen("input.txt", "r");// stdin;

	int last_answer = 0;

	READ(n_lists);
	READ(n_queries);

	int **lists = (int**)malloc(sizeof(int*)*n_lists);
	for (int i = 0; i < n_lists; i++)
	{
		lists[i] = (int*)malloc(sizeof(int)*n_queries);
	}
	int *list_size = (int*)calloc(n_lists, sizeof(int));

	for (int i = 0; i < n_queries; i++)
	{
		READ(op);
		READ(x);
		READ(y);

		if (op == 1)
		{
			int list_index = (x ^ last_answer) % n_lists;

			lists[list_index][list_size[list_index]++] = y;
		}
		else if (op == 2)
		{
			int list_index = (x ^ last_answer) % n_lists;
			last_answer = lists[list_index][y % list_size[list_index]];
			fprintf(f_out, "%d\n", last_answer);
		}

	}

	getchar();
	return 0;
}