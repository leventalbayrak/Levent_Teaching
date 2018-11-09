#pragma once
#include "NN_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
namespace NN
{
	namespace internal
	{
		void init(NN *n, int n_input, int n_hidden)
		{
			n->n_input = n_input;
			n->n_hidden = n_hidden;
			n->bias_offset = n_input * n_hidden;
			n->hidden_to_output_offset = n_input * n_hidden + n_hidden;
			n->total_size = n_input * n_hidden + 2 * n_hidden;
			n->data = (double*)malloc(sizeof(double)*n->total_size);
		}

		void randomize(NN *n, double range)
		{
			for (int i = 0; i < n->total_size; i++)
			{
				n->data[i] = range * (1.0 - 2.0*rand() / RAND_MAX);
			}
		}

		double run(NN *n, double *input)
		{
			double output_sum = 0.0;
			for (int i = 0; i < n->n_hidden; i++)
			{
				double sum = 0.0;
				double *input_to_hidden = &n->data[i * n->n_input];
				for (int j = 0; j < n->n_input; j++)
				{
					sum += input[j] * input_to_hidden[j];
				}
				sum += n->data[i + n->bias_offset];

				double w = n->data[i + n->hidden_to_output_offset];
				output_sum += 1.0 / (1.0 + exp(-sum * w));
			}

			return output_sum;
		}

		void copy(NN *dest, NN *src)
		{
			memcpy(dest->data, src->data, sizeof(double)*src->total_size);
		}

		void modify(NN *n, int n_mod_count, double range)
		{
			for (int i = 0; i < n_mod_count; i++) n->data[rand() % n->total_size] += range * (1.0 - 2.0*rand() / RAND_MAX);
		}

		void evolve_Modify(NN *dest, NN *src, int mod_count, double range)
		{
			copy(dest, src);
			modify(dest, mod_count, range);
		}

		void evolve_Run(Evolve_Out *e, NN *n0, NN *n1, double *input)
		{
			e->out0 = run(n0, input);
			e->out1 = run(n1, input);
		}

		void evolve_Update(Temperature *t, NN *dest, NN *src, Evolve_In *e)
		{
			double pe = exp((e->fitness1 - e->fitness0) / t->temperature);
			double p = (double)rand() / RAND_MAX;

			if (p <= pe)
			{
				copy(dest, src);
			}

			if (++t->internal.current_repeat >= t->internal.n_temperature_repeat)
			{
				t->internal.current_repeat = 0;
				t->temperature *= t->decay;
			}
		}
	}

	void init(Solution *s, int n_input, int n_hidden, double temperature, double decay,int n_temperature_repeat, double randomize_range, double modify_range, int n_modify_count)
	{
		internal::init(&s->current, n_input, n_hidden);
		internal::init(&s->tmp, n_input, n_hidden);

		s->temperature.temperature = temperature;
		s->temperature.decay = decay;
		s->temperature.internal.n_temperature_repeat = n_temperature_repeat;
		s->temperature.internal.current_repeat = 0;

		s->modify_range = modify_range;
		s->n_modify_count = n_modify_count;

		internal::randomize(&s->current, randomize_range);
	}

	void run(double &out0, double &out1, double *input, Solution *s)
	{
		internal::Evolve_Out eout;
		internal::evolve_Run(&eout, &s->current, &s->tmp, input);
		out0 = eout.out0;
		out1 = eout.out1;
	}

	inline double run(Solution *s, double *input)
	{
		return internal::run(&s->current, input);
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
}