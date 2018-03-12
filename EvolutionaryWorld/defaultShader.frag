#version 430 core

layout (location = 0) out vec4 outColor;

varying vec3 frag_col;

void main()
{	
	outColor = vec4(frag_col, 1.0);
}