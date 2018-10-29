#pragma once
#include "Spawn_Stack_data.h"
#include <stdio.h>
#include <stdlib.h>

namespace Spawn_Stack
{
	void init(Spawn_Stack *s, int array_size)
	{
		s->array_size = array_size;
		s->n_unspawned = 0;
		s->spawned = (unsigned char*)malloc(sizeof(unsigned char)*s->array_size);
		s->unspawned = (int*)malloc(sizeof(int)*s->array_size);

		s->n_unspawned = s->array_size;
		for (int i = 0; i < s->n_unspawned; i++)
		{
			s->spawned[i] = 0;
			s->unspawned[i] = s->n_unspawned - i - 1;
		}
	}

	int make(Spawn_Stack *s)
	{
		if (s->n_unspawned <= 0) return -1;
		int k = s->unspawned[--s->n_unspawned];
		
		s->spawned[k] = 1;
		return k;
	}

	void destroy(int which, Spawn_Stack *s)
	{
		s->spawned[which] = 0;
		s->unspawned[s->n_unspawned++] = which;
	}

}