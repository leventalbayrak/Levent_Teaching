#version 330 core

uniform mat4 transform;
in vec3 v_in_pos;
in vec3 v_in_color;
in vec2 v_in_tex_coord;

out vec3 v_out_color;
out vec2 v_out_tex_coord;

void main()
{
	gl_Position = transform * vec4(v_in_pos, 1.0f);
	v_out_color = v_in_color;
	v_out_tex_coord = v_in_tex_coord;
}
