#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "NN_core.h"

int main()
{
	NN::Solution solution;
	NN::init(&solution, 2, 2, 1, 0.95, 5, 0.5, 0.1, 10);

	int iteration = 0;

	for (;;)
	{
		if (NN::get_Temperature(&solution) < 0.000001) break;

		iteration++;

		double current_fitness = 0.0;
		double new_fitness = 0.0;

		{
			double input[2] = { 0,0 };

			double out0 = 0;
			double out1 = 0;
			NN::run(out0, out1, input, &solution);

			current_fitness -= (out0 - 0.0)*(out0 - 0.0);
			new_fitness -= (out1 - 0.0)*(out1 - 0.0);
		}
		{
			double input[2] = { 1,0 };

			double out0 = 0;
			double out1 = 0;
			NN::run(out0, out1, input, &solution);

			current_fitness -= (out0 - 1.0)*(out0 - 1.0);
			new_fitness -= (out1 - 1.0)*(out1 - 1.0);
		}
		{
			double input[2] = { 0,1 };

			double out0 = 0;
			double out1 = 0;
			NN::run(out0, out1, input, &solution);

			current_fitness -= (out0 - 1.0)*(out0 - 1.0);
			new_fitness -= (out1 - 1.0)*(out1 - 1.0);
		}
		{
			double input[2] = { 1,1 };

			double out0 = 0;
			double out1 = 0;
			NN::run(out0, out1, input, &solution);

			current_fitness -= (out0 - 0.0)*(out0 - 0.0);
			new_fitness -= (out1 - 0.0)*(out1 - 0.0);
		}
		
		NN::learn(&solution, current_fitness, new_fitness);

	}
	
	{
		double input[2] = { 1,1 };
		printf("%f\n", NN::run(&solution, input));
	}
	{
		double input[2] = { 0,1 };
		printf("%f\n", NN::run(&solution, input));
	}
	{
		double input[2] = { 1,0 };
		printf("%f\n", NN::run(&solution, input));
	}
	{
		double input[2] = { 0,0 };
		printf("%f\n", NN::run(&solution, input));
	}
	printf("%d\n", iteration);
	getchar();
	return 0;
}