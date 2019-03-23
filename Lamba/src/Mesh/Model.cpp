#include "Model.hpp"
void Model::HandleModelData(ModelData & data)
{
	uint64_t verticies_amount = data.verticies.size() * 3;
	uint64_t normals_amount = data.normals.size() * 3;
	uint64_t textures_amount = data.textures.size() * 2;
	 
	m_ArrayBuffer.SetData((verticies_amount + normals_amount) * sizeof(float), nullptr);
	m_ArrayBuffer.SetSubData(0, verticies_amount * sizeof(float), data.verticies.data());
	m_ArrayBuffer.SetSubData(verticies_amount, normals_amount * sizeof(float), data.verticies.data());

	m_VertexArray.glPushAttrib<float>(3, verticies_amount * sizeof(float));
	m_VertexArray.glPushAttrib<float>(3, normals_amount* sizeof(float));


	for (auto & mesh_data : data.meshes) {
		m_Meshes.emplace_back(MeshRawData{mesh_data.size(),mesh_data.data()});
	}

}
void Model::Draw() const noexcept
{
	m_ArrayBuffer.Bind();
	m_VertexArray.Bind();

	for (const auto & mesh : m_Meshes)
		mesh.Draw();
}