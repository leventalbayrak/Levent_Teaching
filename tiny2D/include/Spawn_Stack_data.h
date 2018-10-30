#pragma once


namespace Spawn_Stack
{
	struct Spawn_Stack
	{
		unsigned char *spawned;//0 or 1 for spawned/unspawned
		int *unspawned;
		int n_unspawned;
		int array_size;
	};


}