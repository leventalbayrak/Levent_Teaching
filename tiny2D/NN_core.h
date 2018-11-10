#pragma once
#include "NN_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
namespace NN
{
	namespace Single
	{
		void init(Data *n, int n_input, int n_hidden, int n_output)
		{
			n->n_input = n_input;
			n->n_hidden = n_hidden;
			n->n_output = n_output;
			n->single_network_size = n_input * n_hidden + 2 * n_hidden;//all input to hidden, bias, hidden to output
			n->total_size = n->single_network_size * n_output;
			n->data = (double*)malloc(sizeof(double)*n->total_size);
		}

		void run(double *output, const double *input, const Data *n)
		{

			for (int o = 0; o < n->n_output; o++)
			{
				output[o] = 0;

				double *network = &n->data[n->single_network_size*o];
				double *bias = &network[n->n_input*n->n_hidden];
				double *hidden_to_output = &network[n->n_input*n->n_hidden + n->n_hidden];

				for (int i = 0; i < n->n_hidden; i++)
				{
					double sum = 0.0;
					double *input_to_hidden = &network[i*n->n_input];
					for (int j = 0; j < n->n_input; j++)
					{
						sum += input[j] * input_to_hidden[j];
					}
					sum += bias[i];

					double w = hidden_to_output[i];
					output[o] += 1.0 / (1.0 + exp(-sum * w));
				}
			}
		}

		void copy(Data *dest, const Data *src)
		{
			memcpy(dest->data, src->data, sizeof(double)*src->total_size);
		}

		void destroy(Data *dest)
		{
			free(dest->data);
			*dest = {};
		}
	}

	namespace internal
	{
		

		void randomize(Data *n, double range)
		{
			for (int i = 0; i < n->total_size; i++)
			{
				n->data[i] = range * (1.0 - 2.0*rand() / RAND_MAX);
			}
		}

		void modify(Data *n, int n_mod_count, double range)
		{
			for (int i = 0; i < n_mod_count; i++) n->data[rand() % n->total_size] += range * (1.0 - 2.0*rand() / RAND_MAX);
		}

		void evolve_Modify(Data *dest, const Data *src, int mod_count, double range)
		{
			Single::copy(dest, src);
			modify(dest, mod_count, range);
		}

		inline void evolve_Run(double *output, const double *input, const Data *n)
		{
			Single::run(output, input, n);
		}

		void evolve_Update(Temperature *t, Data *dest, Data *src, Evolve_In *e)
		{
			double pe = exp((e->fitness1 - e->fitness0) / t->temperature);
			double p = (double)rand() / RAND_MAX;

			if (p <= pe)
			{
				Single::copy(dest, src);
			}

			if (++t->internal.current_repeat >= t->internal.n_temperature_repeat)
			{
				t->internal.current_repeat = 0;
				t->temperature *= t->decay;
			}
		}
	}

	namespace Solution
	{
		void init(Solution *s, int n_input, int n_hidden, int n_output, double temperature, double decay, int n_temperature_repeat, double randomize_range, double modify_range, int n_modify_count)
		{
			Single::init(&s->current, n_input, n_hidden, n_output);
			Single::init(&s->tmp, n_input, n_hidden, n_output);

			s->temperature.temperature = temperature;
			s->temperature.decay = decay;
			s->temperature.internal.n_temperature_repeat = n_temperature_repeat;
			s->temperature.internal.current_repeat = 0;

			s->modify_range = modify_range;
			s->n_modify_count = n_modify_count;

			internal::randomize(&s->current, randomize_range);
		}

		inline void run_Main(double *output, const double *input, const Solution *s)
		{
			Single::run(output, input, &s->current);
		}
		inline void run_Alt(double *output, const double *input, const Solution *s)
		{
			Single::run(output, input, &s->tmp);
		}

		void learn(Solution *s, double fitness0, double fitness1)
		{
			internal::Evolve_In ein = { fitness0, fitness1 };
			evolve_Update(&s->temperature, &s->current, &s->tmp, &ein);
			internal::evolve_Modify(&s->tmp, &s->current, s->n_modify_count, s->modify_range);
		}

		double get_Temperature(Solution *s)
		{
			return s->temperature.temperature;
		}

		void create_Current_Copy(Data *dest,const Solution *s)
		{
			Single::init(dest, s->current.n_input, s->current.n_hidden, s->current.n_output);
			Single::copy(dest, &s->current);
		}

		void destroy(Solution *s)
		{
			Single::destroy(&s->current);
			Single::destroy(&s->tmp);
			*s = {};
		}
	}
}