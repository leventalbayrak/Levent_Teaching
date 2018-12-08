#version 330

in vec3 v_out_color;
in vec2 v_out_texcoord;

out vec4 color;

uniform sampler2D color_texture;

void main()
{
	color = texture(color_texture,v_out_texcoord) * vec4(v_out_color, 1.0);
}