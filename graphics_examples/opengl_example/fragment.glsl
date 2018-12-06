#version 330

in vec2 in_texcoord;

out vec4 color;

uniform vec4 tint;

uniform sampler2D color_tex;

void main()
{
	color = texture(color_tex, in_texcoord) * tint;  
}

