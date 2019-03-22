#ifndef _MY_OBJ_LOADER_HPP_
#define _MY_OBJ_LOADER_HPP_
#include"../Mesh.h"
#include<unordered_map>



class MyObjLoader {
	using Key = glm::vec<3, std::uint32_t>;
	using Val = std::uint32_t;

	using VerticiesContainer = std::vector < float>;
	using NormalsContainer = std::vector < float>;
	using TexturesContainer = std::vector < float>;
	using FacesContainer = std::vector<std::uint32_t>;
	using MeshesData = std::vector<FacesContainer>;

	struct ModelData {
		VerticiesContainer verticies;
		NormalsContainer normals;
		TexturesContainer textures;
	};

	struct Vec3UIntHash {
		size_t operator () (const Key & v) const noexcept {
			auto x = std::hash<std::uint32_t>()(v.x);
			auto y = std::hash<std::uint32_t>()(v.y);
			auto z = std::hash<std::uint32_t>()(v.z);
			return x ^ y | z;
		}
	};

	using MehsDataMap = std::unordered_map<Key,Val,Vec3UIntHash>;

	struct mesh_data {
		std::vector<glm::vec3> verticies;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textures;
		std::vector<unsigned int> faces;
		mesh_data() {
			verticies.reserve(1000);
			normals.reserve(1000);
			textures.reserve(1000);
			faces.reserve(1000);
		}
	};
private :
	std::vector<Mesh> m_Meshes;
	char m_State;

	ModelData m_ModelData;

	MeshRawData m_CurrMeshData;


	void parse(char * str);
	void parseFace(char * str);

	mesh_data m_MeshData;

	MehsDataMap m_MeshMap;
	void toRawMeshData();
public:
	std::vector<Mesh> & load(const std::string & filepath);
};

#endif // !_MY_OBJ_LOADER_HPP_
