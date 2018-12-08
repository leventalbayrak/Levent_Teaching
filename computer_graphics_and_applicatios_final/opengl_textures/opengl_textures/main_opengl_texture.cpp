#pragma warning(disable:4996)
#pragma comment(linker,"/subsystem:console")

//LOAD LIBRARIES
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

#pragma comment(lib,"glew-2.1.0\\lib\\Release\\Win32\\glew32.lib")
#pragma comment(lib,"opengl32.lib")

//INCLUDE HEADERS
#include "glew-2.1.0\include\GL\glew.h"
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include "SDL2-2.0.8\include\SDL_opengl.h"

#include "glm\glm.hpp"
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int vertex_array = 0;
unsigned int texture_id = 0;
unsigned int transform_id = 0;

SDL_Window *window = NULL;

struct Model
{
	float x, y, z;
	float rx, ry, rz;
	float sx, sy, sz;
};

Model *boxes = NULL;
int n_boxes = 100;

//for loading shader files
int load(char *buffer, int buffer_size, const char *filename)
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

void init_Boxes()
{
	boxes = (Model*)malloc(sizeof(Model)*n_boxes);
	for (int i = 0; i < n_boxes; i++)
	{
		boxes[i].x = 1.0 - 2.0*rand() / RAND_MAX;
		boxes[i].y = 1.0 - 2.0*rand() / RAND_MAX;
		boxes[i].z = 1.0 - 2.0*rand() / RAND_MAX;

		boxes[i].rx = 180.0*rand() / RAND_MAX;
		boxes[i].ry = 180.0*rand() / RAND_MAX;
		boxes[i].rz = 180.0*rand() / RAND_MAX;

		boxes[i].sx = 0.2*rand() / RAND_MAX;
		boxes[i].sy = 0.2*rand() / RAND_MAX;
		boxes[i].sz = 0.2*rand() / RAND_MAX;
	}
}

void draw(glm::mat4 &projection, glm::mat4 &view)
{
	for (int i = 0; i < n_boxes; i++)
	{
		glm::mat4 model_matrix(1.0f);
		
		model_matrix = glm::rotate(model_matrix, glm::radians(boxes[i].rx), glm::vec3(1.0f, 0.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(boxes[i].ry), glm::vec3(0.0f, 1.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(boxes[i].rz), glm::vec3(0.0f, 0.0f, 1.0f));
		model_matrix = glm::translate(model_matrix, glm::vec3(boxes[i].x, boxes[i].y, boxes[i].z));
		model_matrix = glm::rotate(model_matrix, glm::radians(boxes[i].rx), glm::vec3(1.0f, 0.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(boxes[i].ry), glm::vec3(0.0f, 1.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(boxes[i].rz), glm::vec3(0.0f, 0.0f, 1.0f));
		model_matrix = glm::scale(model_matrix, glm::vec3(boxes[i].sx, boxes[i].sy, boxes[i].sz));

		glm::mat4 transform = projection * view * model_matrix;

		
		glUniformMatrix4fv(transform_id, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(vertex_array);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		boxes[i].ry += 1.2;
		
	}
}

void init_OpenGL()
{

	printf("%s\n", glGetString(GL_VERSION));

	int program_id = glCreateProgram();
	int compile_status = 0;

	const int buffer_size = 4096 * 4;
	const char *buffer = (char*)malloc(buffer_size);

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
		//position        //color         //texture coord
		 0.5,  0.5, 0.0,    1.0, 0.0, 0.0,  1.0, 1.0,
		 0.5, -0.5, 0.0,    0.0, 1.0, 0.0,  1.0, 0.0,
		 -0.5,-0.5, 0.0,    0.0, 0.0, 1.0,  0.0, 0.0,
		-0.5,  0.5, 0.0,    1.0, 1.0, 0.0,  0.0, 1.0
	};

	unsigned int index_data[] =
	{
		0, 1, 3, 1, 2, 3
	};

	unsigned int vertex_buffer_object = 0;
	unsigned int index_buffer_object = 0;
	glGenVertexArrays(1, &vertex_array);
	glGenBuffers(1, &vertex_buffer_object);
	glGenBuffers(1, &index_buffer_object);

	glBindVertexArray(vertex_array);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, 4 * 8 * sizeof(float), vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index_data, GL_STATIC_DRAW);

	int attribute_index_pos = glGetAttribLocation(program_id, "pos");
	glVertexAttribPointer(attribute_index_pos, 3, GL_FLOAT, 0, sizeof(float) * 8, 0);
	glEnableVertexAttribArray(attribute_index_pos);

	int attribute_index_color = glGetAttribLocation(program_id, "color");
	glVertexAttribPointer(attribute_index_color, 3, GL_FLOAT, 0, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(attribute_index_color);

	int attribute_index_texcoord = glGetAttribLocation(program_id, "texcoord");
	glVertexAttribPointer(attribute_index_texcoord, 2, GL_FLOAT, 0, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(attribute_index_texcoord);

	transform_id = glGetUniformLocation(program_id, "transform");

	SDL_Surface *surface = IMG_Load("test.bmp");

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);

}

void init()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("opengl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	init_OpenGL();
}


int main(int argc, char **argv)
{
	init();

	init_Boxes();

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


		//render
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);


		glBindTexture(GL_TEXTURE_2D, texture_id);		
		
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 camera_matrix(1.0f);
		glm::mat4 view = glm::translate(camera_matrix, glm::vec3(0.0f, 0.0f, -2.0f));


		draw(projection, view);

	
		SDL_GL_SwapWindow(window);
	}


	return 0;
}