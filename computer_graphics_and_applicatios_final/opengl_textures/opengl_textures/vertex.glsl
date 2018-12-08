#version 330

uniform mat4 transform;

in vec3 pos;
in vec3 color;
in vec2 texcoord;

out vec3 v_out_color;
out vec2 v_out_texcoord;

void main()
{
	gl_Position = transform * vec4(pos, 1.0);
	v_out_color = color;
	v_out_texcoord = texcoord;
}
