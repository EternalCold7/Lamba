#include "Mesh.h"

Mesh::Mesh(MeshData && data, const Camera * cam, Shader * sh) :
	m_Camera(cam), model(1.f), m_Shader(sh) {

	proj = glm::perspective(70.f, 1000.f / 600.f, 0.1f, 1000.f);


	m_ElementsArray.SetData(data.m_Faces.size() * sizeof(glm::vec<3, std::uint32_t>), data.m_Faces.data());

}

Mesh::Mesh(MeshRawData && data, const Camera * cam, Shader * sh):
 m_Camera(cam), model(1.f), m_Shader(sh) {
	proj = glm::perspective(70.f, 1000.f / 600.f, 0.1f, 1000.f);

	m_ElementsArray.SetData(data.faces_count * sizeof(std::uint32_t), data.m_Faces);

}

Mesh::Mesh(MeshRawData & data, const Camera * cam, Shader * sh) :
	m_Camera(cam), model(1.f), m_Shader(sh) {
	proj = glm::perspective(70.f, 1000.f / 600.f, 0.1f, 1000.f);
	m_ElementsArray.SetData(data.faces_count * sizeof(std::uint32_t), data.m_Faces);

}



void Mesh::Draw() const noexcept {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);


	m_ElementsArray.Bind();

	m_Shader->Bind();
	m_Shader->SetUniformMat4("model", &model[0][0]);
	m_Shader->SetUniformMat4("projection", &proj[0][0]);
	m_Shader->SetUniformMat4("view", &m_Camera->GetViewMatrix()[0][0]);

	glDrawElements(GL_TRIANGLES, m_ElementsArray.GetDataSize() * 4, GL_UNSIGNED_INT, nullptr);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}