#shader vertex
#version 330 core

layout(location = 0) in vec3 mVertex;
layout(location = 1) in vec3 mNormal;
layout(location = 2) in vec2 mTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNorm;
out vec3 fragmentPos;
out vec2 texCoord;
void main() {

	gl_Position = projection * view * model * vec4(mVertex, 1.f);
	fragmentPos = vec3(model * vec4(mVertex, 1.f));
	fNorm = mNormal;
	texCoord = mTexCoord;
}


#shader fragment
#version 330 core

in vec3 fNorm;
in vec3 fragmentPos;

uniform mat4 view;

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	vec3 specular;
	float specCof;
};
uniform Material mat;

in vec2 texCoord;
out vec4 COLOR;
void main() {


	float shininess = 0.8;

	vec3 lightColor = vec3(1.f, 1.f, 1.f);

	vec3 ambient = lightColor * mat.ambient;



	vec3 lightPos = vec3(-3, 4, 4);
	vec3 lightDir = normalize(lightPos - fragmentPos);
	vec3 norm = normalize(fNorm);
	float diff = max(dot(lightDir, norm), 0);
	vec3 diffuse = lightColor * (diff * vec3(texture(mat.diffuse, texCoord)));



	vec3 viewPos = view[0].xyz;
	vec3 viewDir = normalize(viewPos - fragmentPos);

	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.specCof);


	vec3 specular = lightColor * (spec * mat.specular);

	COLOR = vec4(ambient + diffuse + specular, 1.f);

}


