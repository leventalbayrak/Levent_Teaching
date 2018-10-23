#include "My_Game.h"

int main(int argc, char **argv)
{

	My_Game::init();
	My_Game::begin_Play();

	unsigned int last_frame_time = 0;
	for(;;)
	{
		unsigned int current_time = Engine::time();
		if (current_time - last_frame_time < 15) continue;
		float dt = (current_time - last_frame_time) / 1000.0;
		last_frame_time = current_time;

		My_Game::update(current_time, dt);

		My_Game::draw();
	}



	return 0;
}
