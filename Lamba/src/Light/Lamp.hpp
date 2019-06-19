#ifndef _LAMP_HPP_
#define _LAMP_HPP_
#include "Light.hpp"
#include "../Drawable.hpp"
#include "../Transformalbe.hpp"
#include "../OpenGLPrimitives/ArrayBuffer.hpp"
#include "../OpenGLPrimitives/VertexArray.hpp"
#include "../OpenGLPrimitives/ElementsArrayBuffer.hpp"

#include "../Mesh/Model.hpp"
#include "../Camera.hpp"
class Lamp : public Drawable,public Transformable {

private:
	Shader m_Shader;
	LightSource m_Light;
	std::unique_ptr<Model> m_Model;
	void Translate(const glm::vec3 &) {};
	const Camera * m_Camera;
public:

	void ChangeLight(LightBuilder & builder) {
	
	m_Light = builder.GetLight();
	std::shared_ptr<Material> material = std::make_shared<Material>();
	
	material->ambient = m_Light.diffuse;
	material->diffuse = m_Light.diffuse;
	material->specular = m_Light.specular;
	material->specularCof = 0.2;
	m_Model->GetMeshes()[0].SetMaterial(material);
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(m_Light.position));
	}
	const LightSource & GetLight() const noexcept { return m_Light; }
	Lamp(const Camera * cam);
	virtual void Draw() const noexcept override {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);

		m_Model->GetArrayBuffer().Bind();
		m_Model->GetVertexArray().Bind();
		m_Model->GetMeshes()[0].GetElementsArray().Bind();
		m_Shader.Bind();
		m_Shader.SetUniformMat4("model", &m_ModelMatrix[0][0]);
		m_Shader.SetUniformMat4("view", &m_Camera->GetViewMatrix()[0][0]);
		m_Shader.SetUniformMat4("projection", &m_ProjectionMatrix[0][0]);
		m_Shader.SetUnifrom3f("light.diffuse", &m_Light.diffuse[0]);
		glDrawElements(GL_TRIANGLES, m_Model->GetMeshes()[0].GetElementsArray().GetDataSize() * sizeof(uint32_t), GL_UNSIGNED_INT, nullptr);


		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

	}

	
};



#endif // !_LAMP_HPP_
