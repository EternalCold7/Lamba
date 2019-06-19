#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_
#include"../Texture.hpp"
#include <glm/glm.hpp>
#include<memory>
struct Material {
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular = { 0.5,0.5,0.5 };
	float specularCof = 32.f;
	glm::vec3 filter;
	float opticalCof;

	std::shared_ptr<Texture> diffuse_texture;
	std::shared_ptr<Texture> specular_texture;
};

#endif // !_MATERIAL_HPP_

