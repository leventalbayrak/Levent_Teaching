#version 330
out vec4 color;
uniform vec4 tint;
uniform sampler2D color_tex;

void main()
{
	color = tint;
}

