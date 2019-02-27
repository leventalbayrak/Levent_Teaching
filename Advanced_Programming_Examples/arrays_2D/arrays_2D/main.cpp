#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

void A()
{
	FILE *f = fopen("data.txt", "r");
	int n_rows = 0;
	int n_cols = 0;
	fscanf(f, "%d%d", &n_rows, &n_cols);

	static int a[1000][1000];

	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			fscanf(f, "%d", &a[i][j]);
		}
	}
	fclose(f);

	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			printf("\t%d", a[i][j]);
		}
		printf("\n");
	}
}

void B()
{
	FILE *f = fopen("data.txt", "r");
	int n_rows = 0;
	int n_cols = 0;
	fscanf(f, "%d%d", &n_rows, &n_cols);

	int *a = (int*)malloc(sizeof(int)*n_rows*n_cols);

	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			fscanf(f, "%d", &a[i*n_cols+j]);
		}
	}
	fclose(f);

	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			printf("\t%d", a[i*n_cols + j]);
		}
		printf("\n");
	}

	free(a);
}

void C()
{

	FILE *f = fopen("data.txt", "r");
	int n_rows = 0;
	int n_cols = 0;
	fscanf(f, "%d%d", &n_rows, &n_cols);

	int **a = (int**)malloc(sizeof(int*)*n_rows);

	for (int i = 0; i < n_rows; i++)
	{
		a[i] = (int *)malloc(sizeof(int)*n_cols);
		for (int j = 0; j < n_cols; j++)
		{
			fscanf(f, "%d", &a[i][j]);
		}
	}
	fclose(f);

	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_cols; j++)
		{
			printf("\t%d", a[i][j]);
		}
		printf("\n");
	}

	for (int i = 0; i < n_rows; i++)
	{
		free(a[i]);
	}
	free(a);
}


int main()
{
	A();
	B();
	C();
	getchar();
	return 0;
}