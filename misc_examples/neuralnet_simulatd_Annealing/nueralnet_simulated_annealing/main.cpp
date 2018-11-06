#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>

namespace NN
{
	const int max_nodes = 10;

	struct NN
	{
		double w_input_to_hidden[max_nodes*max_nodes];//[i_hidden*n_input + j_input]
		double w_hidden_to_output[max_nodes];//n_hidden
		double b_hidden[max_nodes];//n_hidden
		int n_input, n_hidden;
	};

	void init(NN *n, int n_input, int n_hidden, double alpha)
	{
		n->n_input = n_input;
		n->n_hidden = n_hidden;

		int k = n_input*n_hidden;
		for (int i = 0; i < k; i++)
		{
			n->w_input_to_hidden[i] = alpha*(1.0 - 2.0*rand() / RAND_MAX);
		}
		for (int i = 0; i < n_hidden; i++)
		{
			n->w_hidden_to_output[i] = alpha*(1.0 - 2.0*rand() / RAND_MAX);
			n->b_hidden[i] = alpha*(1.0 - 2.0*rand() / RAND_MAX);
		}
	}

	double run(NN *n, double *input)
	{
		double output = 0.0;
		for (int i = 0; i < n->n_hidden; i++)
		{
			double sum_hidden = 0.0;
			for (int j = 0; j < n->n_input; j++)
			{
				sum_hidden += n->w_input_to_hidden[i*n->n_hidden + j] * input[i];
			}
			sum_hidden += n->b_hidden[i];
			if (sum_hidden < 0.0) sum_hidden = 0.0;

			output += sum_hidden;
		}

		//no bias in output
		if (output < 0.0) output = 0.0;

		return output;
	}

	void copy(NN *dest, NN*src)
	{
		dest->n_input = src->n_input;
		dest->n_hidden = src->n_hidden;
		memcpy(dest->w_input_to_hidden, src->w_input_to_hidden, sizeof(double)*src->n_hidden*src->n_input);
		memcpy(dest->w_hidden_to_output, src->w_hidden_to_output, sizeof(double)*src->n_hidden);
		memcpy(dest->b_hidden, src->b_hidden, sizeof(double)*src->n_hidden);
	}
	
}

namespace NN_Ext
{
	void modify(NN::NN *n, double alpha)
	{
		//for now modify each array

		int w = rand() % (n->n_hidden*n->n_input);
		n->w_input_to_hidden[w] += alpha*(1.0 - 2.0*rand() / RAND_MAX);

		int h = rand() % n->n_hidden;
		n->w_hidden_to_output[h] += alpha*(1.0 - 2.0*rand() / RAND_MAX);

		int b = rand() % n->n_hidden;
		n->b_hidden[b] += alpha*(1.0 - 2.0*rand() / RAND_MAX);
	}
}

namespace SA
{
	double update(NN::NN *n,NN::NN *tmp,double input[4][2],double *expected_output,int n_tests, double temperature)
	{
		NN::copy(tmp, n);
		NN_Ext::modify(tmp, 0.1);
		double f0 = 0.0;
		double f1 = 0.0;
		for (int i = 0; i < n_tests; i++)
		{
			double out0 = NN::run(n, input[i]);
			double out1 = NN::run(tmp, input[i]);

			f0 += (expected_output[i] - out0)*(expected_output[i] - out0);
			f1 += (expected_output[i] - out1)*(expected_output[i] - out1);
		}
		double pe = exp((f0-f1)/temperature);
		double p = (double)rand() / RAND_MAX;
		if (p <= pe)
		{
			NN::copy(n, tmp);
			return f1;
		}
		return f0;
	}

}

int main()
{
	double input[4][2] = {
		{0,0}, {0,1},{1,0},{1,1}
	};
	double output[4] = { 0,1,1,0 };

	NN::NN solution;
	NN::NN tmp;
	double temperature = 1.0;
	double decay = 0.998;

	printf("init\n");
	
	NN::init(&solution, 2, 2, 2.0);
	NN::init(&tmp, 2, 2, 2.0);
	
	printf("run\n");
	for (;;)
	{
		if (temperature < 0.0000001) break;
		
		double error = SA::update(&solution, &tmp, input, output,4, temperature);
		temperature *= decay;
		for (int k = 0; k < 4; k++)
		{
			double r = NN::run(&solution, input[k]);
			printf("%f %f -> %f\n", input[k][0], input[k][1], r);
		}
		printf("temp %f error %f\n", temperature, error);


	}

	getchar();
	return 0;
}

