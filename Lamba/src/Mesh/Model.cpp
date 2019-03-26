#include "Model.hpp"
void Model::HandleModelData(ModelData & data)
{
	uint64_t verticies_amount = data.verticies.size() * 3 * sizeof(float);
	uint64_t normals_amount = data.normals.size() * 3 * sizeof(float);
	uint64_t textures_amount = data.textures.size() * 2 * sizeof(float);
	 
	m_ArrayBuffer.SetData(verticies_amount + normals_amount + textures_amount, nullptr);
	m_ArrayBuffer.SetSubData(0, verticies_amount, data.verticies.data());
	m_ArrayBuffer.SetSubData(verticies_amount, normals_amount, data.normals.data());
	m_ArrayBuffer.SetSubData(verticies_amount + normals_amount, textures_amount , data.textures.data());

	m_VertexArray.glPushAttrib<float>(3, verticies_amount);
	m_VertexArray.glPushAttrib<float>(3, normals_amount);
	m_VertexArray.glPushAttrib<float>(2, textures_amount);

	for (auto & mesh_data : data.meshes) {
		m_Meshes.emplace_back(mesh_data);
	}

}
void Model::Draw() const noexcept
{
	m_ArrayBuffer.Bind();
	m_VertexArray.Bind();

	for (const auto & mesh : m_Meshes)
		mesh.Draw();
}