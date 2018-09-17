/*
AS IS
copyright levent albayrak 2018
*/

#pragma once
#include <assert.h>
#include <stdio.h>
/*-----------------------------------------------------------------------------
INTERFACE
-----------------------------------------------------------------------------*/


/*
windows only timer utility
static, single thread only
*/
#ifdef _WIN32
#include <windows.h>
namespace Stopwatch
{
	void init();
	void begin();
	void end();
	double elapsed(unsigned int which);
	void reset();
	void print();
}
#endif


/*-----------------------------------------------------------------------------
IMPLEMENTATION
-----------------------------------------------------------------------------*/

/*
windows only timer utility
static, single thread only
*/

#ifdef _WIN32
#include <windows.h>
namespace Stopwatch
{
	struct P
	{
		LARGE_INTEGER freq;
		LARGE_INTEGER t[2];
		double elapsed[256];
		int n_elapsed;
	};

	static P timer;

	void init()
	{
		QueryPerformanceFrequency(&timer.freq);
		timer.n_elapsed=0;
	}
	void begin()
	{
		QueryPerformanceCounter(&timer.t[0]);
	}
	void end()
	{
		if (timer.n_elapsed >= 256) return;
		QueryPerformanceCounter(&timer.t[1]);
		LARGE_INTEGER elapsed_mu;
		elapsed_mu.QuadPart = timer.t[1].QuadPart - timer.t[0].QuadPart;
		double elapsed = elapsed_mu.QuadPart*1000;//millisec //1000000//microsec
		elapsed /= timer.freq.QuadPart;
		timer.elapsed[timer.n_elapsed++] = elapsed;
	}
	double elapsed(unsigned int which)
	{
		if (which >= 256.0) return 0.0;
		return timer.elapsed[which];
	}

	void reset()
	{
		timer.n_elapsed = 0;
	}

	void print()
	{
		printf("%4u %8s\n", timer.n_elapsed, "tasks measured");
		printf("------------------|\n");
		printf("|%8s|%8s|\n", "task", "elapsed");
		printf("------------------|\n");
		for (unsigned int i = 0; i < timer.n_elapsed; i++)
		{
			printf("|%8u|%8.6f|\n", i, timer.elapsed[i]);
		}
		printf("------------------|\n");
	}
}
#endif
