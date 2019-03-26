#include "AssimpLoader.hpp"

void AssimpLoader::ProcessNode(aiNode * node, const aiScene * scene)
{
	
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		auto * mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.emplace_back(ProcessRawMesh(mesh, scene));
	}

	for (std::uint32_t i = 0; i < node->mNumChildren; ++i) {
		ProcessNode(node->mChildren[i], scene);
	}
}

MeshData AssimpLoader::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{

	MeshData outMesh;
	outMesh.m_Normals = std::vector<glm::vec3>(mesh->mNumVertices);
	outMesh.m_Verticies = std::vector<glm::vec3>(mesh->mNumVertices);
	outMesh.m_Faces = std::vector<glm::vec<3, std::uint32_t>>(mesh->mNumFaces);

	for (std::uint64_t j = 0; j < mesh->mNumVertices; j++) {
		const auto & v = mesh->mVertices[j];
		outMesh.m_Verticies[j] = std::move(glm::vec3(v.x, v.y, v.z));
	}

	for (std::uint64_t j = 0; j < mesh->mNumVertices; j++) {
		const auto & v = mesh->mNormals[j];
		outMesh.m_Normals[j] = std::move(glm::vec3(v.x, v.y, v.z));
	}

	for (std::uint64_t j = 0; j < mesh->mNumFaces; j++) {
		const auto face = mesh->mFaces[j].mIndices;

		outMesh.m_Faces[j] = glm::vec<3, std::uint32_t>(face[0], face[1], face[2]);
	}
	return outMesh;
}
MeshRawData AssimpLoader::ProcessRawMesh(aiMesh * mesh, const aiScene * scene)
{

	MeshRawData outMesh;
	outMesh.verticies_count = mesh->mNumVertices * 3;
	outMesh.m_Normals = &mesh->mNormals[0].x;
	outMesh.m_Verticies = &mesh->mVertices[0].x;
	unsigned int faces_size = 0;
	for (int i = 0; i < mesh->mNumFaces; ++i)
		faces_size += mesh->mFaces[i].mNumIndices;

	outMesh.m_Faces = new unsigned int[faces_size];
	unsigned int face_index = 0;
	for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
		for (int i = 0; i < mesh->mFaces[j].mNumIndices; ++i) {
			outMesh.m_Faces[face_index] = mesh->mFaces[j].mIndices[i];
			face_index++;
		}
	}
	outMesh.faces_count = faces_size;
	auto a = outMesh.m_Faces[faces_size-1];
	return outMesh;
}


[[nodiscard]] std::vector<Mesh> & AssimpLoader::load(const std::string & filepath)
{
	Assimp::Importer importer;
	auto  * scene = importer.ReadFile(filepath, aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw std::runtime_error("Model not readed");

	ProcessNode(scene->mRootNode, scene);
	return m_Meshes;
}
