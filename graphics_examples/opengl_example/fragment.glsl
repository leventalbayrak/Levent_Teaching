#version 330 core

in vec3 v_out_color;
in vec2 v_out_tex_coord;

out vec4 f_out_color;
uniform sampler2D color_tex;
void main()
{
	f_out_color = texture(color_tex, v_out_tex_coord);// vec4(v_out_color, 1.0f);
}

