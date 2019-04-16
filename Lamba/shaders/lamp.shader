#shader vertex
#version 330 core

layout(location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {

	gl_Position = projection * view * model * vec4(inPos, 1.f);
}

#shader fragment
#version 330 core

out vec4 color;

struct Light {
	vec3 specular;
};

uniform Light light;

void main() {
	color = vec4(light.specular,1.f);
}