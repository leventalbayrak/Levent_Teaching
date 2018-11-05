#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

namespace SA
{
	const int n_cities = 8;
	double city_x[n_cities];
	double city_y[n_cities];


	void init()
	{
		for (int i = 0; i < n_cities; i++)
		{
			city_x[i] = rand() % 100;
			city_y[i] = rand() % 100;
		}
	}

	//Durstenfeld shuffle
	void generate_New_Path(int *path, int array_size)
	{
		for (int i = 0; i < array_size; i++) path[i] = i;

		unsigned int n = array_size - 1;
		for (int i = 0; i < n; i++)
		{
			int index = i + rand() % (n - i + 1);

			int tmp = path[index];
			path[index] = path[i];
			path[i] = tmp;
		}

		for (int i = 0; i < array_size; i++)
		{
			if (path[i] == 0)
			{
				int tmp = path[0];
				path[0] = path[i];
				path[i] = tmp;
				break;
			}
		}
		
	}

	void modify_Path(int *path, int array_size)
	{
		int indexA = 1 + rand() % (array_size - 1);
		int indexB = 1 + rand() % (array_size - 1);

		int tmp = path[indexA];
		path[indexA] = path[indexB];
		path[indexB] = tmp;
	}

	double evaluate_Path(int *path, int array_size)
	{
		double sum = 0.0;
		for (int i = 1; i < array_size; i++)
		{
			double dx = city_x[path[i]] - city_x[path[i - 1]];
			double dy = city_y[path[i]] - city_y[path[i - 1]];
			double distance = dx*dx + dy*dy;
			sum += distance;
		}
		return sum;
	}

	double update(int *path, int* tmp_path, int array_size, double temperature)
	{
		memcpy(tmp_path, path, sizeof(int)*array_size);
		modify_Path(tmp_path, array_size);
		double current_cost = evaluate_Path(path, array_size);
		double new_cost = evaluate_Path(tmp_path, array_size);

		double pe = exp((current_cost - new_cost) / temperature);
		double p = (double)rand() / RAND_MAX;

		if (p <= pe)
		{
			memcpy(path, tmp_path, sizeof(int)*array_size);
			return new_cost;
		}

		return current_cost;
	}

}

int main()
{
	srand(time(0));
	int path[SA::n_cities];
	int tmp_path[SA::n_cities];

	SA::init();

	double temperature = 1.0;
	double temperature_decay = 0.98;
	SA::generate_New_Path(path, SA::n_cities);

	int counter = 0;
	for (;;)
	{
		double cost = SA::update(path, tmp_path, SA::n_cities, temperature);
		/*printf("%d temperature = %f current_cost = %lf\n",counter, temperature, cost);
		for (int i = 0; i < SA::n_cities; i++)
		{
			printf("%d ", path[i]);
		}
		printf("\n");*/
		temperature *= temperature_decay;

		counter++;

		if (temperature <= 0.0000001) break;
	}
	
	for (int i = 0; i < SA::n_cities; i++)
	{
		printf("%f %f\n", SA::city_x[i], SA::city_y[i]);
	}

	for (int i = 0; i < SA::n_cities; i++)
	{
		printf("%d ", path[i]);
	}
	printf("\n");

	getchar();
	return 0;
}