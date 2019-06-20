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
	fNorm = normalize(mNormal);
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

struct LightSource
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};
uniform mat4 v_inv;
uniform unsigned int lightsCount;
uniform LightSource lightSources[10];
out vec4 COLOR;


vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);
void main() {

	

	vec3 normalDirection = normalize(fNorm);
	vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - vec4(fragmentPos,1.f)));
	vec3 lightDirection;
	float attenuation;

	// initialize total lighting with ambient lighting
	vec3 totalLighting = vec3(scene_ambient) * mat.ambient;

	for (int index = 0; index < lightsCount; index++) // for all light sources
	{
		if (lightSources[index].position.w == 0) // directional light?
		{
			attenuation = 1.0; // no attenuation
			lightDirection = normalize(vec3(lightSources[index].position));
		}
		else // point light or spotlight (or other kind of light) 
		{

			vec3 positionToLightSource = vec3(lightSources[index].position) - fragmentPos;
			float distance = length(positionToLightSource);
			lightDirection = normalize(positionToLightSource);
			attenuation = 1.0 / (lightSources[index].constantAttenuation
				+ lightSources[index].linearAttenuation * distance
				+ lightSources[index].quadraticAttenuation * distance * distance);

			if (lightSources[index].spotCutoff <= 90.0) // spotlight?
			{
				vec4 debugcolor = vec4(0.f, 0.f, 0.f, 0.f);

				float clampedCosine = max(0.0, dot(-lightDirection, normalize(lightSources[index].spotDirection)));
				if (clampedCosine < cos(radians(lightSources[index].spotCutoff))) // outside of spotlight cone?
				{
					attenuation = 0.0;
				}
				else
				{
					attenuation = attenuation * pow(clampedCosine, lightSources[index].spotExponent);
				}
			}
		}


		vec3 diffuseReflection = attenuation
			* vec3(lightSources[index].diffuse) * vec3(mat.diffuse)
			* max(0.0, dot(normalDirection, lightDirection));

		vec3 specularReflection;
		if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
		{

			specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
		}
		else // light source on the right side
		{

			specularReflection = attenuation * vec3(lightSources[index].specular) * vec3(mat.specular)
				* pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), mat.specCof);
		}

		totalLighting = totalLighting + diffuseReflection + specularReflection;

	}

	COLOR = vec4(totalLighting,1.0);
}




