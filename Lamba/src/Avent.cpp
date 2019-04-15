#include "Avent.hpp"
#include"Mesh/Loaders/MyObjLoader.hpp"




Avent::Avent(const Camera& cam) : m_Drawer(&cam, nullptr)
{
	MyObjLoader loader;
	m_Drawer.SetModel(std::move(std::make_unique<Model>("CarModel/", "Cube.obj", loader)));
	m_LightSources.emplace_back(&cam);
	m_LightSources[0].ChangePosition({ 2,2,2 });
	m_Drawer.SetLights(&m_LightSources);
	m_LightSources.emplace_back(&cam);
	
	PointLight light;
	light.m_ambient = { 0,0.5,0.7 };
	light.m_diffuse = { 0,0.5,0.7 };
	light.m_specular = { 0,0.5,0.7 };

	light.m_quadratic = 0.032f;
	light.m_constant = 1.f;
	light.m_linear = 0.09f;


	m_LightSources[1].ChangeLight(light);
	m_LightSources[1].ChangePosition({ -2,1,-1 });
	m_Drawer.SetLights(&m_LightSources);
}

void Avent::Go()
{
	float deegres = 5;

	deegres = glm::radians(deegres);
	for (auto & m : m_Wheels) {
		auto midx = (m->m_Coordinates.maxX + m->m_Coordinates.minX) / 2;
		auto midy = (m->m_Coordinates.maxY + m->m_Coordinates.minY) / 2;
		auto midz = (m->m_Coordinates.maxZ + m->m_Coordinates.minZ) / 2;
	}

}


void Avent::AventModelDrawer::SetUpShaders()
{

	m_Shaders.emplace_back("shaders/point_light.shader");
}

void Avent::AventModelDrawer::SetUpToDraw(const Mesh& mesh) const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);


	const auto & material = mesh.GetMaterial();
	m_Shaders[0].Bind();
	m_Shaders[0].SetUnifrom3f("mat.ambient",&material.ambient[0]);
	m_Shaders[0].SetUnifrom3f("mat.specular",&material.specular[0]);
	m_Shaders[0].SetUniformf("mat.specCof", material.specularCof);


	if (!m_Lights)
		throw;


	m_Shaders[0].SetUniformui("lightsCount", m_Lights->size());
	for (unsigned int i = 0; i < m_Lights->size(); ++i) {
		const auto & lamp = (*m_Lights)[i];
		m_Setter.SetLightToShader(lamp.GetLight(), m_Shaders[0],i);
	}


	m_Shaders[0].SetUnifrom3f("mat.diffuse", &material.diffuse[0]);

	m_Shaders[0].SetUniformMat4("model", &m_ModelMatrix[0][0]);
	m_Shaders[0].SetUniformMat4("projection", &m_ProjectionMatrix[0][0]);
	m_Shaders[0].SetUniformMat4("view", &m_Camera->GetViewMatrix()[0][0]);

	GLCall(glDrawElements(GL_TRIANGLES, mesh.GetElementsArray().GetDataSize() * sizeof(uint32_t), GL_UNSIGNED_INT, nullptr));



	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

}


Avent::AventModelDrawer::AventModelDrawer(const Camera* cam, std::unique_ptr<Model> model) : ModelDrawer(std::move(model)), m_Camera(cam)
{
	SetUpShaders();
}