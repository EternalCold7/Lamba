#include "Lamp.hpp"
#include "../Mesh/Loaders/MyObjLoader.hpp"



void Lamp::SetupLight()
{
	m_Light.m_ambient = { 1.f,1.f,1.f };
	m_Light.m_diffuse = { 1.f,1.f,1.f };
	m_Light.m_specular = { 1.f,0.f,0.f };

	m_Light.m_quadratic = 0.032f;
	m_Light.m_constant = 1.f;
	m_Light.m_linear = 0.09f;
}

Lamp::Lamp(const Camera * cam) : m_Camera(cam),m_Shader("shaders/lamp.shader")
{
	MyObjLoader loader;
	m_Model = std::make_unique<Model>("CarModel/", "Cube.obj", loader);
	std::shared_ptr<Material> material = std::make_shared<Material>();
	SetupLight();
	material->ambient = m_Light.m_ambient;
	material->diffuse = m_Light.m_diffuse;
	material->specular = m_Light.m_specular;
	material->specularCof = 0.2;
	m_Model->GetMeshes()[0].SetMaterial(material);
}
