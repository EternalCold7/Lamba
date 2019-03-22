#shader vertex 
#version 330 core

layout(location = 0) in vec3 mVertex;
layout(location = 1) in vec3 mNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNorm;
out vec3 fragmentPos;
void main() {

	gl_Position = projection * view * model * vec4(mVertex,1.f);
	fragmentPos = vec3(model * vec4(mVertex, 1.f));
	fNorm = mNormal;
}


#shader fragment
#version 330 core

in vec3 fNorm;
in vec3 fragmentPos;

out vec4 COLOR;
void main() {

	vec3 objectColor = vec3(0.2,0.7,0.5);
	float ambientStr = 0.1;
	vec3 ambient = objectColor * ambientStr;

	vec3 lightPos = vec3(0, 20, 0);
	vec3 lightDir = normalize(lightPos - fragmentPos);
	vec3 norm = normalize(fNorm);
	float diff = max(dot(lightDir,norm), 0);
	vec3 diffuse = objectColor * diff;
	
	COLOR = vec4(ambient + diffuse,1.f);

}


