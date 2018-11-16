#pragma comment(linker,"/subsystem:console") //with console
//#pragma comment(linker,"/subsystem:windows") //without console

//load libraries
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"glew-2.1.0\\lib\\Release\\Win32\\glew32.lib")
#pragma comment(lib,"opengl32.lib")
//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include "glew-2.1.0/include/GL/glew.h"
#include "SDL2-2.0.9/include/SDL.h"
#include "SDL2-2.0.9/include/SDL_opengl.h"

#include <Windows.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;

//Graphics program
GLuint gProgramID = 0;
GLint vertexColorLocation = 0;
GLint vertex_pos = 0;
GLuint gVBO = 0;
GLuint gIBO = 0;

void initGL()
{
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));

	//Generate program
	gProgramID = glCreateProgram();

	int  success;
	static char info[512];

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource = 
		"#version 330\n\
		layout (location = 0) in vec3 aPos;\
		uniform vec3 vertex_pos;\
		out vec4 vertex_color;\
		void main() \
		{ \
			gl_Position = vec4(aPos + vertex_pos, 1.0);\
			vertex_color = vec4(1.0,1.0,1.0,1.0);\
		}";
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		printf("ERROR vertex shader:\n%s\n", info);
	}
	glAttachShader(gProgramID, vertexShader);

	//Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource = 
		"#version 330\n\
		out vec4 FragColor;\
		in vec4 vertex_color;\
		uniform vec4 my_color;\
		void main()\
		{\
			FragColor = my_color*vertex_color;\
		}";
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//Compile fragment source
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		printf("ERROR fragment shader:\n%s\n", info);
	}
	glAttachShader(gProgramID, fragmentShader);

	//Link program
	glLinkProgram(gProgramID);

	
	//Bind program
	glUseProgram(gProgramID);

	vertex_pos = glGetUniformLocation(gProgramID, "vertex_pos");
	vertexColorLocation = glGetUniformLocation(gProgramID, "my_color");
	
	//VBO data
	GLfloat vertexData[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };

	//Create VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	//Create IBO
	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	

}


void init()
{
	
	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	//Create context
	gContext = SDL_GL_CreateContext(gWindow);
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	SDL_GL_SetSwapInterval(1);

	

	initGL();
}

void render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform4f(vertexColorLocation, (double)rand()/RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX, 1.0f);

	glUniform3f(vertex_pos, 0.1*(double)rand() / RAND_MAX, 0.1*(double)rand() / RAND_MAX, 0.1*(double)rand() / RAND_MAX);

	//Set vertex data
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	//Set index data and render
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	
	
}

void close()
{
	//Deallocate program
	glDeleteProgram(gProgramID);

	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	init();

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//Render quad
		render();

		//Update screen
		SDL_GL_SwapWindow(gWindow);
	}

	//Free resources and close SDL
	close();

	return 0;
}
