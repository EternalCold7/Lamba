#shader vertex
#version 330 core

layout(location = 0) in vec4 mPosition;

uniform mat4 u_MVP;


void main() {
	gl_Position = u_MVP * mPosition;
}


#shader fragment
#version 330 core


out vec4 outColor;

void main() {
	outColor = vec4(1, 1, 1, 1);
}