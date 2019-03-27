#pragma warning(disable:4996)
#pragma comment(linker,"/subsystem:console")

//LOAD LIBRARIES
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2_image.lib")

#pragma comment(lib,"glew-2.1.0\\lib\\Release\\Win32\\glew32.lib")
#pragma comment(lib,"opengl32.lib")

//INCLUDE HEADERS
#include "glew-2.1.0\include\GL\glew.h"
#include "SDL2-2.0.9\include\SDL.h"
#include "SDL2-2.0.9\include\SDL_opengl.h"
#include "SDL2-2.0.9\include\SDL_image.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>


unsigned int vertex_array = 0;
SDL_Window *window = NULL;
int transform_id = 0;
unsigned int texture_id = 0;
int program_id = 0;

int load(char *buffer,int buffer_size, const char *filename)
{
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	if (size > buffer_size) size = buffer_size;
	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, size, f);
	buffer[size] = 0;
	fclose(f);
	return size;
}

void init_OpenGL()
{
	const int buffer_size = 4096 * 4;
	const char *buffer = (char*)malloc(buffer_size);

	printf("%s\n", glGetString(GL_VERSION));

	glEnable(GL_TEXTURE_2D);

	program_id = glCreateProgram();
	int compile_status = 0;

	unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	load((char*)buffer, buffer_size, "vertex.glsl");
	glShaderSource(vertex_shader_id, 1, &buffer, NULL);
	glCompileShader(vertex_shader_id);
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == 0)
	{
		static char info[1024];
		glGetShaderInfoLog(vertex_shader_id, 1024, NULL, info);
		printf("vertex shader compiler error: %s\n", info);
	}
	glAttachShader(program_id, vertex_shader_id);

	unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	load((char*)buffer, buffer_size, "fragment.glsl");
	glShaderSource(fragment_shader_id, 1, &buffer, NULL);
	glCompileShader(fragment_shader_id);
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == 0)
	{
		static char info[1024];
		glGetShaderInfoLog(fragment_shader_id, 1024, NULL, info);
		printf("fragment shader compiler error: %s\n", info);
	}
	glAttachShader(program_id, fragment_shader_id);

	glLinkProgram(program_id);

	glUseProgram(program_id);

	float vertex_data[] = 
	{
		// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};



	unsigned int index_data[] =
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int vertex_buffer_object = 0;
	unsigned int index_buffer_object = 0;

	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &vertex_buffer_object);
	glGenBuffers(1, &index_buffer_object);

	glBindVertexArray(vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4 * sizeof(float), vertex_data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

	int attribute_location = glGetAttribLocation(program_id, "v_in_pos");
	glVertexAttribPointer(attribute_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attribute_location);

	attribute_location = glGetAttribLocation(program_id, "v_in_color");
	glVertexAttribPointer(attribute_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(attribute_location);

	attribute_location = glGetAttribLocation(program_id, "v_in_tex_coord");
	glVertexAttribPointer(attribute_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(attribute_location);

	transform_id = glGetUniformLocation(program_id, "transform");

	SDL_Surface *surface = IMG_Load("wall.jpg");

	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);

	////int number_of_mipmaps = 2;
	////glTexStorage2D(GL_TEXTURE_2D, 2, GL_RGB8, surface->w, surface->h);
	////glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	////glGenerateMipmap(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	printf("w: %d h: %d bits: %d\n", surface->w, surface->h, surface->format->BitsPerPixel);

	SDL_FreeSurface(surface);

	glUniform1i(glGetUniformLocation(program_id, "color_tex"), 0);
	
}

void init()
{
	SDL_Init(SDL_INIT_VIDEO);

	//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("opengl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	init_OpenGL();
}


int main(int argc, char **argv)
{
	init();

	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}
		
		glUseProgram(program_id);

		//render
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CAMERA
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0));//move camera -4 units in Z axis

		//MODEL
		glm::mat4 model(1.0f);//identity matrix (1.0s in the diagonal)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));//no translation/movement
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));//reduce original size by half
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotate 45 degrees on Z axis
	
		//multiply projection view model
		glm::mat4 transform = projection * view * model;

		glUniformMatrix4fv(transform_id, 1, GL_TRUE, glm::value_ptr(transform));


		//set active texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		//bind buffers and draw
		glBindVertexArray(vertex_array);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);


		SDL_GL_SwapWindow(window);
	}


	return 0;
}