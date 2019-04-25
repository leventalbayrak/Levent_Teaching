#pragma warning(disable:4996)
#include <stdio.h>

int capacity = 20;
const int max_n_items = 1000;
int n_items = 0;
int weight[max_n_items];
int value[max_n_items];
int best_possible[max_n_items];
int best_possible_used_capacity[max_n_items];

void load(const char *filename)
{
	FILE *f = fopen(filename, "r");
	for (;;)
	{
		int w, v;
		int r = fscanf(f,"%d%d", &w, &v);
		if (r != 2) break;
		weight[n_items] = w;
		value[n_items] = v;
		n_items++;
	}
	fclose(f);
}

int weight_Range(int a, int b)
{
	int sum = 0;
	for (int i = a; i <= b; i++)
	{
		sum += weight[i];
	}
	return sum;
}

int value_Range(int a, int b)
{
	int sum = 0;
	for (int i = a; i <= b; i++)
	{
		sum += value[i];
	}
	return sum;
}

int best()
{
	for (int i = 1; i < n_items; i++)
	{
		int k = n_items - i;
		best_possible[k] = value_Range(k, n_items - 1);
		best_possible_used_capacity[k] = weight_Range(k, n_items - 1);
		for (int j = 0; j < i-1; j++)
		{
			int current_weight = weight_Range(k, k + j) + best_possible_used_capacity[k+1];
			int current_best = value_Range(k, k + j) + best_possible[k+1];
			if (current_best > best_possible[k] && current_weight < capacity)
			{
				best_possible[k] = current_best;
				best_possible_used_capacity[k] = current_weight;
			}
		}

		printf("%d\n", best_possible[k]);
	}

	return best_possible[0];
}

int main()
{
	load("test.txt");

	int d = best();

	printf("best %d\n", d);

	getchar();
	return 0;
}