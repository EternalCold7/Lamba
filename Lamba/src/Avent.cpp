#include "Avent.hpp"
#include"Mesh/Loaders/MyObjLoader.hpp"


class CommonBlueLight final : public PointLightBuilder {
protected:
	virtual glm::vec3 SetAmbient() override {
		return { 0.f,0.5f,0.7f };
	}
	virtual glm::vec3 SetDiffuse() override {
		return { 0.f,0.5f,0.7f };
	}
	virtual glm::vec3 SetSpecular() override { return { 0.f,0.5f,0.7f }; }
	virtual float SetLinear() override { return 0.09f; }
	virtual float SetConstant() override { return 1.f; }
	virtual float SetQuadratic() override { return 0.032f; }
};


class GreenLight final : public PointLightBuilder {
protected:
	virtual glm::vec3 SetAmbient() override {
		return { 0.4f,0.9f,0.3f };
	}
	virtual glm::vec3 SetDiffuse() override {
		return SetAmbient();
	}
	virtual glm::vec3 SetSpecular() override { return SetAmbient(); }
	virtual float SetLinear() override { return 0.09f; }
	virtual float SetConstant() override { return 1.f; }
	virtual float SetQuadratic() override { return 0.032f; }
};

Avent::Avent(const Camera& cam) : m_Drawer(&cam, nullptr)
{
	MyObjLoader loader;
	m_Drawer.SetModel(std::move(std::make_unique<Model>("CarModel/earth/", "Earth.obj", loader)));

	
	m_LightSources.emplace_back(&cam);
	m_LightSources.emplace_back(&cam);
	m_LightSources.emplace_back(&cam);



	m_LightSources[0].ChangePosition({ 2.f,2.f,2.f });
	m_LightSources[1].ChangePosition({ -1.f,-1.f,-1.f });
	m_LightSources[2].ChangePosition({ -1.f,0.f,3.f });
	CommonBlueLight m;
	GreenLight gL;
	for (auto & l : m_LightSources)
		l.ChangeLight(std::ref(m));

	m_LightSources[1].ChangeLight(gL);
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
	m_Shaders.emplace_back("shaders/point_light_textured.shader");
}

void Avent::AventModelDrawer::SetUpToDraw(const Mesh& mesh) const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);


	const auto & material = mesh.GetMaterial();

	enum SHADER_TYPE {
		NONE = -1,COMMON,TEXTURED
	};

	SHADER_TYPE shaderType = NONE;
	shaderType = material.diffuse_texture.operator bool() ? TEXTURED : COMMON;
	
	const auto & currentShader = m_Shaders[shaderType];
	currentShader.Bind();

	if (shaderType) {
		GLCall( glActiveTexture(GL_TEXTURE0));
		material.diffuse_texture->Bind();
		currentShader.SetUniformui("mat.diffuse", 0);

	}
	else {

		currentShader.SetUnifrom3f("mat.diffuse", &material.ambient[0]);
		currentShader.SetUnifrom3f("mat.ambient", &material.ambient[0]);
		
	}

	currentShader.SetUnifrom3f("mat.specular", &material.specular[0]);
	currentShader.SetUniformf("mat.specCof", material.specularCof);


	if (!m_Lights)
		throw;


m_Shaders[shaderType].SetUniformui("lightsCount", m_Lights->size());
	for (unsigned int i = 0; i < m_Lights->size(); ++i) {
		const auto & lamp = (*m_Lights)[i];
		m_Setter.SetLightToShader(lamp.GetLight(), currentShader,i);
	}



	currentShader.SetUniformMat4("model", &m_ModelMatrix[0][0]);
	currentShader.SetUniformMat4("projection", &m_ProjectionMatrix[0][0]);
	currentShader.SetUniformMat4("view", &m_Camera->GetViewMatrix()[0][0]);

	GLCall(glDrawElements(GL_TRIANGLES, mesh.GetElementsArray().GetDataSize() * sizeof(uint32_t), GL_UNSIGNED_INT, nullptr));



	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

}


Avent::AventModelDrawer::AventModelDrawer(const Camera* cam, std::unique_ptr<Model> model) : ModelDrawer(std::move(model)), m_Camera(cam)
{
	Scale({ 0.3f,0.3f ,0.3f });
	SetUpShaders();
}