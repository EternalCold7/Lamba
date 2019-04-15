#shader vertex
#version 330 core

layout(location = 0) in vec3 mPosition;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main() {
	gl_Position = projection * view * model * vec4(mPosition,1.f);
}

#shader fragment
#version 330 core

out vec4 color;

struct PointLight {
	vec3 specular;
};

uniform PointLight light;
void main() {
	
	color = vec4(light.specular,1.0);
}




