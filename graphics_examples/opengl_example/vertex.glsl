#version 330
layout(location = 0) in vec3 pos;
uniform vec3 wiggle;
uniform vec2 size;
void main()
{
	gl_Position = vec4(vec3(size , 1.0)*(pos + wiggle), 1.0);
}
