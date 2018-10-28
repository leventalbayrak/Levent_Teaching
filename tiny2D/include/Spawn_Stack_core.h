#pragma once
#include "Spawn_Stack_data.h"
#include <stdio.h>
#include <stdlib.h>

namespace Spawn_Stack
{
	void init(Spawn_Stack *s, int array_size)
	{
		s->array_size = array_size;
		s->n_spawned = 0;
		s->n_unspawned = 0;
		s->spawned = (int*)malloc(sizeof(int)*s->array_size);
		s->unspawned = (int*)malloc(sizeof(int)*s->array_size);

		s->n_unspawned = s->array_size;
		for (int i = 0; i < s->n_unspawned; i++) s->unspawned[i] = i;
	}

	int make(Spawn_Stack *s)
	{
		if (s->n_unspawned <= 0) return -1;
		int k = s->unspawned[--s->n_unspawned];
		s->spawned[s->n_spawned++] = k;
		return k;
	}

	int destroy(Spawn_Stack *s)
	{
		if (s->n_spawned <= 0) return -1;
		int k = s->spawned[--s->n_spawned];
		s->unspawned[s->n_unspawned++] = k;
		return k;
	}

}