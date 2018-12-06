#version 330
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;

uniform mat4 transform;

out vec2 out_texcoord;

void main()
{
	gl_Position = transform * vec4(pos, 1.0);

	out_texcoord = texcoord;
}
