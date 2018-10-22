#include "My_Game.h"

int main(int argc, char **argv)
{

	My_Game::init();
	My_Game::begin();

	bool done = false;
	while (!done)
	{
		unsigned int current_time = SDL_GetTicks();

		My_Game::update(current_time);

		My_Game::draw();
	}



	return 0;
}
