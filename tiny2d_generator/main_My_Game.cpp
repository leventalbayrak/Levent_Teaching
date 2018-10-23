#include "My_Game.h"

int main(int argc, char **argv)
{
	My_Game::init();
	My_Game::begin_Play();
	for(;;)
	{
		unsigned int current_time = Engine::time();
		My_Game::update(current_time);
		My_Game::draw();
	}
	return 0;
}
