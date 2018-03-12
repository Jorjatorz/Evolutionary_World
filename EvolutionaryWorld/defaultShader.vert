#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 MVP;
uniform vec3 color;

varying vec3 frag_col;

void main()
{	
	frag_col = color;
	gl_Position =  MVP * vec4(position, 1.0);
}