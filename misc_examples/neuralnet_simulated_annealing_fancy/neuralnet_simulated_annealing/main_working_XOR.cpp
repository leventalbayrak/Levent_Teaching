#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "NN_core.h"

int main()
{
	NN::Solution::Solution solution;
	NN::Solution::init(&solution, 2, 2, 1, 1, 0.95, 5, 0.5, 0.1, 10);

	int iteration = 0;

	for (;;)
	{
		if (NN::Solution::get_Temperature(&solution) < 0.000001) break;

		iteration++;

		double current_fitness = 0.0;
		double new_fitness = 0.0;

		{
			double input[2] = { 0,0 };

			double out[2][1];
			NN::Solution::run(out[0], out[1], input, &solution);

			current_fitness -= (out[0][0] - 0.0)*(out[0][0] - 0.0);
			new_fitness -= (out[1][0] - 0.0)*(out[1][0] - 0.0);
		}
		{
			double input[2] = { 1,1 };

			double out[2][1];
			NN::Solution::run(out[0], out[1], input, &solution);

			current_fitness -= (out[0][0] - 0.0)*(out[0][0] - 0.0);
			new_fitness -= (out[1][0] - 0.0)*(out[1][0] - 0.0);
		}
		{
			double input[2] = { 0,1 };

			double out[2][1];
			NN::Solution::run(out[0], out[1], input, &solution);

			current_fitness -= (out[0][0] - 1)*(out[0][0] - 1);
			new_fitness -= (out[1][0] - 1)*(out[1][0] - 1);
		}
		{
			double input[2] = { 1,0 };

			double out[2][1];
			NN::Solution::run(out[0], out[1], input, &solution);

			current_fitness -= (out[0][0] - 1)*(out[0][0] - 1);
			new_fitness -= (out[1][0] - 1)*(out[1][0] - 1);
		}

		NN::Solution::learn(&solution, current_fitness, new_fitness);

	}

	NN::Data n;
	NN::Solution::create_Current_Copy(&n, &solution);

	{
		double input[2] = { 0,0 };
		double output[1];
		NN::Single::run(output, input, &n);
		printf("%f\n", output[0]);
	}
	{
		double input[2] = { 0,1 };
		double output[1];
		NN::Single::run(output, input, &n);
		printf("%f\n", output[0]);
	}
	{
		double input[2] = { 1,0 };
		double output[1];
		NN::Single::run(output, input, &n);
		printf("%f\n", output[0]);
	}
	{
		double input[2] = { 1,1 };
		double output[1];
		NN::Single::run(output, input, &n);
		printf("%f\n", output[0]);
	}

	printf("%d\n", iteration);
	getchar();
	return 0;
}