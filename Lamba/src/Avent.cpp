#include "Avent.hpp"
#include"Mesh/Loaders/MyObjLoader.hpp"


class FirstLight final : public LightBuilder {
protected:
	virtual glm::vec4 SetPosition() override {
		return { 0.f,1.f,10.f ,1.f};
	}
	virtual glm::vec4 SetDiffuse() override {
		return { 0.3f,1.f,1.f,1.f };
	}
	virtual glm::vec4 SetSpecular() override { return { 0.1f,0.1f,0.1f,1.f }; }
	virtual float SetLinearAttenuation() override { return 0.1f; }
	virtual float SetConstantAttenuation() override { return 0.f; }
	virtual float SetQuadraticAttenuation() override { return 0.f; }
	virtual float SetSpotCutoff() override { return 180.f; }
	virtual float SetSpotExponent() override { return 10.f; }
	virtual glm::vec3 SetSpotDirection() override { return { 1.f,0.f,0.f }; }
};

class SecondLight final : public LightBuilder {
protected:
	virtual glm::vec4 SetPosition() override {
		return { 0.f,-1.f,-5.f ,1.f };
	}
	virtual glm::vec4 SetDiffuse() override {
		return { 2.f,0.f,0.f,1.f };
	}
	virtual glm::vec4 SetSpecular() override { return {0.1f,0.1f,0.1f,1.f};}
	

	virtual float SetConstantAttenuation() override { return 0.f; }
	virtual float SetLinearAttenuation() override { return 0.6f; }
	virtual float SetQuadraticAttenuation() override { return 0.f; }
	virtual float SetSpotCutoff() override { return 180.f; }
	virtual float SetSpotExponent() override { return 10.f; }
	virtual glm::vec3 SetSpotDirection() override { return { 0.f,1.f,0.f }; }
};
Avent::Avent(const Camera& cam) : m_Drawer(&cam, nullptr)
{
	MyObjLoader loader;
	m_Drawer.SetModel(std::move(std::make_unique<Model>("CarModel/earth/", "Earth.obj", loader)));
//	m_Drawer.SetModel(std::move(std::make_unique<Model>("CarModel/", "untitled.obj", loader)));

	
	m_LightSources.emplace_back(&cam);
	m_LightSources.emplace_back(&cam);




	FirstLight firstLight;
	SecondLight secondLight;
	
	m_LightSources[0].ChangeLight(firstLight);
	m_LightSources[1].ChangeLight(secondLight);
	
	m_Drawer.SetLights(&m_LightSources);

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
		
		
	}
	currentShader.SetUnifrom3f("mat.ambient", &material.ambient[0]);
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
	glm::mat4 v_inv = glm::inverse(m_Camera->GetViewMatrix());
	currentShader.SetUniformMat4("v_inv", &v_inv[0][0]);
	glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(m_ModelMatrix)));
	//currentShader.SetUniformMat3("m_3x3_inv_transp", &m_3x3_inv_transp[0][0]);
	(glm::mat4(1.f),1,34,2,12321,2134,213,4,2134,123,4,231,4,2134,2314,123,4,231,4,123,4,123,4,"asdfasdf");
	GLCall(glDrawElements(GL_TRIANGLES, mesh.GetElementsArray().GetDataSize() * sizeof(uint32_t), GL_UNSIGNED_INT, nullptr));



	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

}


Avent::AventModelDrawer::AventModelDrawer(const Camera* cam, std::unique_ptr<Model> model) : ModelDrawer(std::move(model)), m_Camera(cam)
{
	Scale({ 0.3f,0.3f ,0.3f });
	SetUpShaders();
}