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
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

void initGL()
{
	//Generate program
	gProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource = 
		"#version 140\n\
		in vec2 LVertexPos2D;\
		void main() \
		{ \
			gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 );\
		}";
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);
	glAttachShader(gProgramID, vertexShader);

	//Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource = "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 );}";
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	//Compile fragment source
	glCompileShader(fragmentShader);
	glAttachShader(gProgramID, fragmentShader);

	//Link program
	glLinkProgram(gProgramID);

	//Bind program
	glUseProgram(gProgramID);

	//Get vertex attribute location
	gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
	//Enable vertex position
	glEnableVertexAttribArray(gVertexPos2DLocation);
	//VBO data
	GLfloat vertexData[] =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };

	//Create VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_DYNAMIC_DRAW);

	//Create IBO
	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
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

	

	//Set vertex data
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	//GLfloat pos[2] = {1,1 };
	//glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), pos);
	glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

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
