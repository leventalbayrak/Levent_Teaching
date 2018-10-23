#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	const char *project_name = "My_Game";
	static char str[1024];
	sprintf(str, "main_%s.cpp", project_name);
	FILE *f = fopen(str, "w+");

	
	fprintf(f, "#include \"%s.h\"\n\n", project_name);
	fprintf(f, "int main(int argc, char **argv)\n{\n");
	fprintf(f, "\t%s::init();\n\t%s::begin_Play();\n", project_name, project_name);
	fprintf(f, "\tfor(;;)\n\t{\n");
	fprintf(f, "\t\tunsigned int current_time = Engine::time();\n", project_name);
	fprintf(f, "\t\t%s::update(current_time);\n\t\t%s::draw();\n\t}\n\treturn 0;\n}\n", project_name, project_name);
	
	fclose(f);

	sprintf(str, "%s.h", project_name);
	f = fopen(str, "w+");
	
const char *src = "\n\
#pragma once\n\
#include \"include/Engine_core.h\"\n\
namespace %s\n\
\
	namespace Command\n\
	{\n\
		int UP, DOWN, LEFT, RIGHT;\n\
	}\n\
	namespace Asset\n\
	{\n\
		//LOAD ASSETS HERE\n\
	}\n\
\n\
	namespace World\n\
	{\n\
	    //GLOBAL VARIABLES, ARRAYS\n\
		namespace Parameters\n\
		{\n\
			//WORLD PARAMETERS\n\
		}\n\
\n\
	}\n\
\n\
	void init(int screen_w, int screen_h)\n\
	{\n\
		Engine::init(\"%s\", screen_w, screen_h);\n\
        //OTHER INIT, ALLOCATIONS\n\
	}\n\
\n\
	void begin_Play()\n\
	{\n\
		//GAME BEGIN\n\
	}\n\
\n\
	void update(unsigned int current_time)\n\
	{\n\
        //INPUT\n\
		Engine::event_Loop();\n\
\n\
		Command::UP = 0;\n\
		Command::DOWN = 0;\n\
		Command::LEFT = 0;\n\
		Command::RIGHT = 0;\n\
\n\
		if (Engine::keys[SDL_SCANCODE_W] && Engine::prev_key_state[SDL_SCANCODE_W] == 0) Command::UP = 1;\n\
		if (Engine::keys[SDL_SCANCODE_SPACE] && Engine::prev_key_state[SDL_SCANCODE_SPACE] == 0) Command::UP = 1;\n\
		if (Engine::keys[SDL_SCANCODE_A]) Command::LEFT = 1;\n\
		if (Engine::keys[SDL_SCANCODE_D]) Command::RIGHT = 1;\n\
\n\
		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_A] && Engine::Input::prev_game_controller_buttons[0][SDL_CONTROLLER_BUTTON_A] == 0) Command::UP = 1;\n\
		if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX] < -5000) Command::LEFT = 1;\n\
		else if (Engine::Input::game_controller_sticks[0][SDL_CONTROLLER_AXIS_LEFTX] > 5000) Command::RIGHT = 1;\n\
	    \n\
		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_LEFT]) Command::LEFT = 1;\n\
		if (Engine::Input::game_controller_buttons[0][SDL_CONTROLLER_BUTTON_DPAD_RIGHT]) Command::RIGHT = 1;\n\
        \n\
        //UPDATE GAME\n\
	    \n\
	}\n\
\n\
	void draw()\n\
	{\n\
        //DRAW\n\
        \n\
		SDL_RenderPresent(Engine::renderer);\n\
	}\n\
	";
	fprintf(f, src, project_name, project_name);
	fclose(f);
	return 0;
}