#include "My_Game.h"
#include <Windows.h>

int main(int argc, char **argv)
{
	LARGE_INTEGER t1, t2;
	LARGE_INTEGER cpu_freq;
	QueryPerformanceFrequency(&cpu_freq);

	QueryPerformanceCounter(&t1);

	My_Game::init(1280, 960);
	My_Game::begin_Play(Engine::time());

	int measure_freq = 100;
	int measure_frame = 0;

	unsigned int last_frame_time = 0;
	for(;;)
	{
		unsigned int current_time = Engine::time();
		if (current_time - last_frame_time < 15) continue;
		float dt = (current_time - last_frame_time) / 1000.0;
		last_frame_time = current_time;
		
		My_Game::update(current_time, dt);
		
		My_Game::draw(current_time);


		Audio::play_Queue();

		if (++measure_frame >= measure_freq)
		{
			QueryPerformanceCounter(&t2);
			float frame_time = 1000.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
			frame_time /= measure_freq;
			t1 = t2;

			measure_frame = 0;
			printf("frame time: %f msec\n", frame_time);
		}
	}

	return 0;
}
