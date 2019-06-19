#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../Shader.h"

#include<sstream>

struct LightSource {
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	glm::vec3 spotDirection;
};


class ShaderSetterForLight {
public:
	void SetLightToShader(const LightSource& light, const Shader & shader) const {
		throw "not impelemented";
	}


	void SetLightToShader(const LightSource& light, const Shader & shader, unsigned int index) const {
		std::stringstream i;
		i << index;
		std::string stringIndex = i.str();
		shader.SetUnifrom4f("lightSources[" + stringIndex + "].position", &light.position[0]);
		shader.SetUnifrom4f("lightSources[" + stringIndex + "].diffuse", &light.diffuse[0]);
		shader.SetUnifrom4f("lightSources[" + stringIndex + "].specular", &light.specular[0]);
		shader.SetUnifrom3f("lightSources[" + stringIndex + "].spotDirection", &light.spotDirection[0]);
		shader.SetUniformf("lightSources[" + stringIndex + "].constantAttenuation", light.constantAttenuation);
		shader.SetUniformf("lightSources[" + stringIndex + "].linearAttenuation", light.linearAttenuation);
		shader.SetUniformf("lightSources[" + stringIndex + "].quadraticAttenuation", light.quadraticAttenuation);
		shader.SetUniformf("lightSources[" + stringIndex + "].spotCutoff", light.spotCutoff);
		shader.SetUniformf("lightSources[" + stringIndex + "].spotExponent", light.spotExponent);
	}
};
class LightBuilder {
	
protected:
	LightSource m_Light{};
	virtual glm::vec4 SetPosition() = 0;
	virtual glm::vec4 SetDiffuse() = 0;
	virtual glm::vec4 SetSpecular() = 0;
	virtual float SetLinearAttenuation() = 0;
	virtual float SetConstantAttenuation() = 0;
	virtual float SetQuadraticAttenuation() = 0;
	virtual float SetSpotCutoff() = 0;
	virtual float SetSpotExponent() = 0;
	virtual glm::vec3 SetSpotDirection() = 0;

public:
	
	LightSource GetLight() { 
		m_Light.position = SetPosition();
		m_Light.diffuse = SetDiffuse();
		m_Light.specular = SetSpecular();
		m_Light.linearAttenuation = SetLinearAttenuation();
		m_Light.constantAttenuation = SetConstantAttenuation();
		m_Light.quadraticAttenuation = SetQuadraticAttenuation();
		m_Light.spotCutoff = SetSpotCutoff();
		m_Light.spotExponent = SetSpotExponent();
		m_Light.spotDirection = SetSpotDirection();
		return m_Light; 
	};
};
#endif // _LIGHT_HPP_
