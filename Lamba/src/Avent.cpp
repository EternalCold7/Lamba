#include "Avent.hpp"
#include"Mesh/Loaders/MyObjLoader.hpp"




Avent::Avent(const Camera& cam) : m_Drawer(&cam, nullptr)
{
	MyObjLoader loader;
	m_Drawer.SetModel(std::move(std::make_unique<Model>("CarModel/", "Cube.obj", loader)));
	/*for (auto & mesh : m_Model->GetMeshes()) {
		if (mesh.GetName().find("wheel") != std::string::npos)
			m_Wheels.push_back(&mesh);
	}*/
}

void Avent::Go()
{
	float deegres = 5;

	deegres = glm::radians(deegres);
	for (auto & m : m_Wheels) {
		auto midx = (m->m_Coordinates.maxX + m->m_Coordinates.minX) / 2;
		auto midy = (m->m_Coordinates.maxY + m->m_Coordinates.minY) / 2;
		auto midz = (m->m_Coordinates.maxZ + m->m_Coordinates.minZ) / 2;

		/*m->Translate({ midx,midy,midz });
		m->Rotate({ 0,0,1 }, deegres);
		m->Translate({ -midx,-midy,-midz });*/
	}

}

void Avent::AventModelDrawer::SetUpShaders()
{
	m_Shaders.emplace_back("shaders/light.shader");
}

void Avent::AventModelDrawer::SetUpToDraw(const Mesh& mesh) const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	m_Shaders[0].Bind();
	m_Shaders[0].SetUniformMat4("model", &m_ModelMatrix[0][0]);
	m_Shaders[0].SetUniformMat4("projection", &m_ProjectionMatrix[0][0]);
	m_Shaders[0].SetUniformMat4("view", &m_Camera->GetViewMatrix()[0][0]);
	GLCall(glDrawElements(GL_TRIANGLES, mesh.GetElementsArray().GetDataSize() * sizeof(uint32_t), GL_UNSIGNED_INT, nullptr));
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

}


Avent::AventModelDrawer::AventModelDrawer(const Camera* cam, std::unique_ptr<Model> model) : ModelDrawer(std::move(model)), m_Camera(cam)
{

	m_ProjectionMatrix = glm::perspective(70.f, 1000.f / 600.f, 0.1f, 1000.f);
	SetUpShaders();
}