#include "Mesh.h"

Mesh::Mesh(MeshData && data, const Camera * cam, Shader * sh) :
	m_Camera(cam), m_Shader(sh) {
	SetName(std::move(data.name));
	m_ElementsArray.SetData(data.m_Faces.size() * sizeof(uint32_t), data.m_Faces.data());
	m_Material = data.mat;
	m_Coordinates = data.coords;
}

Mesh::Mesh(MeshData & data, const Camera * cam, Shader * sh) :
	m_Camera(cam),  m_Shader(sh)
{
	SetName(std::move(data.name));
	m_ElementsArray.SetData(data.m_Faces.size() * sizeof(uint32_t), data.m_Faces.data());
	m_Coordinates = data.coords;
	m_Material = data.mat;
}

Mesh::Mesh(MeshRawData && data, const Camera * cam, Shader * sh):
 m_Camera(cam),  m_Shader(sh) {
	SetName(std::move(data.name));
	m_ElementsArray.SetData(data.faces_count * sizeof(std::uint32_t), data.m_Faces);
	m_Coordinates = data.coords;
	m_Material = data.mat;
}

Mesh::Mesh(MeshRawData & data, const Camera * cam, Shader * sh) :
	m_Camera(cam), m_Shader(sh) {
	SetName(std::move(data.name));
	m_Material = data.mat;
	m_ElementsArray.SetData(data.faces_count * sizeof(std::uint32_t), data.m_Faces);
	m_Coordinates = data.coords;

}



void Mesh::Draw() const noexcept {
	

	AttribsSetter a;

	m_ElementsArray.Bind();

	m_Shader->Bind();
	m_Shader->SetUniformMat4("model", &(rotation*scale*translation)[0][0]);
	m_Shader->SetUniformMat4("projection", &proj[0][0]);
	m_Shader->SetUniformMat4("view", &m_Camera->GetViewMatrix()[0][0]);

	SetupFragmentShader();

	GLCall(glDrawElements(GL_TRIANGLES, m_ElementsArray.GetDataSize() * 4, GL_UNSIGNED_INT, nullptr));
	
}


void Mesh::SetupFragmentShader() const
{
	m_Shader->SetUnifrom3f("mat.ambient", &m_Material->ambient[0]);
	if (m_Material->diffuse_texture) {
		GLCall(glActiveTexture(GL_TEXTURE0));
		glBindTexture(GL_TEXTURE_2D,m_Material->diffuse_texture->GetTextureDescriptor());
		m_Shader->SetUniformui("mat.diffuse", 0);
	}
	else {
		m_Shader->SetUnifrom3f("mat.diffuse", &m_Material->diffuse[0]);
	}

	if (m_Material->specular_texture) {
		GLCall(glActiveTexture(GL_TEXTURE1));
		glBindTexture(GL_TEXTURE_2D, m_Material->specular_texture->GetTextureDescriptor());
		m_Shader->SetUniformui("mat.specular",1);
	}
	else {
		m_Shader->SetUnifrom3f("mat.specular", &m_Material->specular[0]);
	}
	m_Shader->SetUniformf("mat.specCof", m_Material->specularCof);

}


Mesh::AttribsSetter::AttribsSetter()
{
	GLCall(glEnable(GL_CULL_FACE));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glCullFace(GL_BACK));
}

Mesh::AttribsSetter::~AttribsSetter()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}
