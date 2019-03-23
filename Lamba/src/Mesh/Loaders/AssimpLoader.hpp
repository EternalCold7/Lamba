#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include "../Mesh/Mesh.h"

class Model;
class AssimpLoader {
	std::vector<Mesh> m_Meshes;
	MeshData ProcessMesh(aiMesh * mesh, const aiScene * scene);
	MeshRawData ProcessRawMesh(aiMesh * mesh, const aiScene * scene);
	void ProcessNode(aiNode * node, const aiScene * scene);
public:
	std::vector<Mesh> & load(const std::string & filepath);
};


