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

	gl_Position = projection * view * model * vec4(mVertex, 1.f);
	fragmentPos = vec3(model * vec4(mVertex, 1.f));
	fNorm = mNormal;
}


#shader fragment
#version 330 core

in vec3 fNorm;
in vec3 fragmentPos;

uniform mat4 view;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specCof;
};
uniform Material mat;

struct PointLight {
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;

	float quadratic;
	float constant;
	float linear;
};


uniform unsigned int lightsCount;
uniform PointLight pointLights[100];
out vec4 COLOR;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPosition,vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPosition);
	float diff = max(dot(normal, lightDir), 0.f);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), mat.specCof);
	float distance = length(light.position - fragPosition);
	float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * mat.ambient;
	vec3 diffuse = light.diffuse * diff * mat.diffuse;
	vec3 specular = light.specular * spec * mat.specular;

	ambient *= attenuation;
	specular *= attenuation;
	diffuse *= attenuation;
	return (ambient + diffuse + specular);
}

void main() {


	vec3 viewPos = vec3(view[3]);
	
	vec3 viewDir = normalize(viewPos - fragmentPos);
	vec3 norm = normalize(fNorm);
	vec3 result;

	for (int i = 0; i < lightsCount; ++i)
		result += CalcPointLight(pointLights[i], norm, fragmentPos, viewDir);

	COLOR = vec4(result, 1.f);

}


