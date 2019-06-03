#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_
#include <glm/vec3.hpp>

#include "../Shader.h"

#include<sstream>

struct DirectionalLight {
	glm::vec3 m_direction;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

struct PointLight {
	glm::vec3 m_position;
	
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	
	float m_constant;
	float m_linear;
	float m_quadratic;
};

class LightShaderSetter {
public:
	void SetLightToShader(const DirectionalLight & light, const Shader & shader) const {
		shader.SetUnifrom3f("dirLight.direction", &light.m_direction[0]);
		shader.SetUnifrom3f("dirLight.ambient", &light.m_ambient[0]);
		shader.SetUnifrom3f("dirLight.diffuse", &light.m_diffuse[0]);
		shader.SetUnifrom3f("dirLight.specular", &light.m_specular[0]);
	}
	void SetLightToShader(const PointLight & light, const Shader & shader) const {
		shader.SetUnifrom3f("pointLight.position", &light.m_position[0]);
		shader.SetUnifrom3f("pointLight.ambient", &light.m_ambient[0]);
		shader.SetUnifrom3f("pointLight.diffuse", &light.m_diffuse[0]);
		shader.SetUnifrom3f("pointLight.specular", &light.m_specular[0]);
		shader.SetUniformf("pointLight.constant", light.m_constant);
		shader.SetUniformf("pointLight.linear", light.m_linear);
		shader.SetUniformf("pointLight.quadratic", light.m_quadratic);
	}

	void SetLightToShader(const DirectionalLight & light, const Shader & shader, unsigned int index) const {
		std::stringstream i;
		i << index;
		std::string sIndex = i.str();
		shader.SetUnifrom3f("dirLight[" + sIndex + "].direction", &light.m_direction[0]);
		shader.SetUnifrom3f("dirLight[" + sIndex + "].ambient", &light.m_ambient[0]);
		shader.SetUnifrom3f("dirLight[" + sIndex + "].diffuse", &light.m_diffuse[0]);
		shader.SetUnifrom3f("dirLight[" + sIndex + "].specular", &light.m_specular[0]);
	}
	void SetLightToShader(const PointLight & light, const Shader & shader, unsigned int index) const {
		std::stringstream i;
		i << index;
		std::string sIndex = i.str();
		shader.SetUnifrom3f("pointLights[" + sIndex + "].position", &light.m_position[0]);
		shader.SetUnifrom3f("pointLights[" + sIndex + "].ambient", &light.m_ambient[0]);
		shader.SetUnifrom3f("pointLights[" + sIndex + "].diffuse", &light.m_diffuse[0]);
		shader.SetUnifrom3f("pointLights[" + sIndex + "].specular", &light.m_specular[0]);
		shader.SetUniformf("pointLights[" + sIndex + "].constant", light.m_constant);
		shader.SetUniformf("pointLights[" + sIndex + "].linear", light.m_linear);
		shader.SetUniformf("pointLights[" + sIndex + "].quadratic", light.m_quadratic);
	}
};
class PointLightBuilder {
	
protected:
	PointLight m_Light{};
	virtual glm::vec3 SetAmbient() = 0;
	virtual glm::vec3 SetDiffuse() = 0;
	virtual glm::vec3 SetSpecular() = 0;
	virtual float SetLinear() = 0;
	virtual float SetConstant() = 0;
	virtual float SetQuadratic() = 0;

public:
	
	PointLight GetLight() { 
		m_Light.m_ambient = SetAmbient();
		m_Light.m_diffuse = SetDiffuse();
		m_Light.m_specular = SetSpecular();
		m_Light.m_linear = SetLinear();
		m_Light.m_constant = SetConstant();
		m_Light.m_quadratic = SetQuadratic();
		return m_Light; 
	};
};
#endif // _LIGHT_HPP_
