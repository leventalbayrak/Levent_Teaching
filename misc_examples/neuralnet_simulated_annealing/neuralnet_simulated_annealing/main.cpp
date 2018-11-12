#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

struct NN
{
	int n_input;
	int n_hidden;
	double *input_to_hidden_weights;//n_input*n_hidden
	double *hidden_to_output_weights;//n_hidden
	double *hidden_unit_biases;//n_hidden
};

void init(NN *n, int n_input, int n_hidden)
{
	n->n_input = n_input;
	n->n_hidden = n_hidden;

	n->input_to_hidden_weights = (double*)malloc(sizeof(double)*n_input*n_hidden);
	n->hidden_to_output_weights = (double*)malloc(sizeof(double)*n_hidden);
	n->hidden_unit_biases = (double*)malloc(sizeof(double)*n_hidden);
}

void randomize(NN *n, double range)
{
	int k = n->n_input*n->n_hidden;
	for (int i = 0; i < k; i++)
	{
		n->input_to_hidden_weights[i] = range*(1.0 - 2.0*rand() / RAND_MAX);
	}
	for (int i = 0; i < n->n_hidden; i++)
	{
		n->hidden_to_output_weights[i] = range*(1.0 - 2.0*rand() / RAND_MAX);
		n->hidden_unit_biases[i] = range*(1.0 - 2.0*rand() / RAND_MAX);
	}
}

double run(NN *n, double *input)
{
	double output_sum = 0.0;
	for (int i = 0; i < n->n_hidden; i++)
	{
		double sum = 0.0;
		for (int j = 0; j < n->n_input; j++)
		{
			sum += input[j] * n->input_to_hidden_weights[i*n->n_input + j];
		}
		sum += n->hidden_unit_biases[i];
		double squeezed_sum = 1.0 / (1.0 + exp(-sum));//sigmoid

		squeezed_sum *= n->hidden_to_output_weights[i];

		output_sum += squeezed_sum;
	}

	return output_sum;
}

void copy(NN *dest, NN *src)
{
	dest->n_input = src->n_input;
	dest->n_hidden = src->n_hidden;
	memcpy(dest->input_to_hidden_weights, src->input_to_hidden_weights, sizeof(double)*src->n_input*src->n_hidden);
	memcpy(dest->hidden_to_output_weights, src->hidden_to_output_weights, sizeof(double)*src->n_hidden);
	memcpy(dest->hidden_unit_biases, src->hidden_unit_biases, sizeof(double)*src->n_hidden);
}

void modify(NN *n,double range)
{
	n->input_to_hidden_weights[rand()%(n->n_input*n->n_hidden)] += range*(1.0 - 2.0*rand() / RAND_MAX);
	n->hidden_to_output_weights[rand() % n->n_hidden] += range*(1.0 - 2.0*rand() / RAND_MAX);
	n->hidden_unit_biases[rand() % n->n_hidden] += range*(1.0 - 2.0*rand() / RAND_MAX);
}

int main()
{
	NN my_network;
	NN tmp_network;
	init(&my_network, 2, 2);
	init(&tmp_network, 2, 2);
	
	/*my_network.input_to_hidden_weights[0] = 20;
	my_network.input_to_hidden_weights[1] = 20;
	my_network.input_to_hidden_weights[2] = -20;
	my_network.input_to_hidden_weights[3] = -20;

	my_network.hidden_unit_biases[0] = -10;
	my_network.hidden_unit_biases[1] = 30;

	my_network.hidden_to_output_weights[0] = 20;
	my_network.hidden_to_output_weights[1] = 20;*/
	double temperature = 1.0;
	double decay = 0.998;
	randomize(&my_network, 0.05);
	for (;;)
	{
		if (temperature < 0.000001) break;
		
		printf("temperature %f\n", temperature);

		copy(&tmp_network, &my_network);
		modify(&tmp_network, 0.05);

		double current_error = 0.0;
		double new_error = 0.0;

		{
			double input[2] = { 1,1 };

			double output = run(&my_network, input);

			current_error += (output - 0.0)*(output - 0.0);

			double new_output = run(&tmp_network, input);

			new_error += (new_output - 0.0)*(new_output - 0.0);
		}

		{
			double input[2] = { 1,0 };

			double output = run(&my_network, input);

			current_error += (output - 1.0)*(output - 1.0);

			double new_output = run(&tmp_network, input);

			new_error += (new_output - 1.0)*(new_output - 1.0);
		}

		{
			double input[2] = { 0,1 };

			double output = run(&my_network, input);

			current_error += (output - 1.0)*(output - 1.0);

			double new_output = run(&tmp_network, input);

			new_error += (new_output - 1.0)*(new_output - 1.0);
		}

		{
			double input[2] = { 0,0 };

			double output = run(&my_network, input);

			current_error += (output - 0.0)*(output - 0.0);

			double new_output = run(&tmp_network, input);

			new_error += (new_output - 0.0)*(new_output - 0.0);
		}
		
		double pe = exp((current_error - new_error) / temperature);
		double p = (double)rand() / RAND_MAX;

		if (p <= pe)
		{
			copy(&my_network, &tmp_network);
		}


		temperature *= decay;

	}
	
	{
		double input[2] = { 1,1 };
		printf("%f\n", run(&my_network, input));
	}
	{
		double input[2] = { 0,1 };
		printf("%f\n", run(&my_network, input));
	}
	{
		double input[2] = { 1,0 };
		printf("%f\n", run(&my_network, input));
	}
	{
		double input[2] = { 0,0 };
		printf("%f\n", run(&my_network, input));
	}
	
	getchar();
	return 0;
}