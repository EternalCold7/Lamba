#include "Model.hpp"
void Model::HandleModelData(ModelData & data)
{
	uint64_t verticies_amount = data.verticies.size() * 3 * sizeof(float);
	uint64_t normals_amount = data.normals.size() * 3 * sizeof(float);
	uint64_t textures_amount = data.textures.size() * 2 * sizeof(float);

	uint64_t entire_amount = 0;


	if (verticies_amount)
		entire_amount += verticies_amount;
	if (normals_amount)
		entire_amount += normals_amount;
	if (textures_amount)
		entire_amount += textures_amount;


	m_ArrayBuffer.SetData(entire_amount, nullptr);

	uint64_t offset = 0;

	if (verticies_amount) {
		m_ArrayBuffer.SetSubData(0, verticies_amount, data.verticies.data());
		offset += verticies_amount;
		m_VertexArray.glPushAttrib<float>(3, verticies_amount);
	}

	if (normals_amount) {
		m_ArrayBuffer.SetSubData(offset, normals_amount, data.normals.data());
		offset += normals_amount;
		m_VertexArray.glPushAttrib<float>(3, normals_amount);
	}
	if (textures_amount) {
		m_ArrayBuffer.SetSubData(offset, textures_amount, data.textures.data());
		m_VertexArray.glPushAttrib<float>(2, textures_amount);
	}



	for (auto & mesh_data : data.meshes) {
		m_Meshes.emplace_back(mesh_data);
	}

}