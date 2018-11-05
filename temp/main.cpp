#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct SA
{
	double cooling_rate;
	unsigned int n_current_iterations;

	double temperature;

	void *current;
	void *tmp;

	int element_size;
	int array_size;

	double(*eval)(void *solution, int array_size);
	void(*permute)(void *solution, int array_size);
	
};

void init(SA *s, void *s1, void *s2, int array_size, int element_size)
{
	s->current = s1;
	s->tmp = s2;
	s->array_size = array_size;
	s->temperature = 1.0;
	s->cooling_rate = 0.99;
	s->n_current_iterations = 0;
	s->element_size = element_size;
}

void update_Step(SA *s)
{
	memcpy(s->tmp, s->current, s->element_size*s->array_size);
	s->permute(s->tmp, s->array_size);
	double f0 = s->eval(s->current, s->array_size);
	double f1 = s->eval(s->tmp, s->array_size);
	double pe = exp((f0 - f1) / s->temperature);
	double p = (double)rand() / RAND_MAX;
	if (p <= pe)
	{
		void *t = s->current;
		s->current = s->tmp;
		s->tmp = t;
	}
	s->n_current_iterations++;
	s->temperature *= s->cooling_rate;
}


int main()
{




	return 0;
}