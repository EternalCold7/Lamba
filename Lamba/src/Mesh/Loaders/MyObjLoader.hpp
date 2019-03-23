#ifndef _MY_OBJ_LOADER_HPP_
#define _MY_OBJ_LOADER_HPP_
#include<unordered_map>
#include "../Model.hpp"


struct ModelData;
class MyObjLoader {
	static const unsigned int reserve_amount;

	using Key = glm::vec<3, std::uint32_t>;
	using Val = std::uint32_t;

	
	struct Vec3UIntHash {
		size_t operator () (const Key & v) const noexcept {
			auto x = std::hash<std::uint32_t>()(v.x);
			auto y = std::hash<std::uint32_t>()(v.y);
			auto z = std::hash<std::uint32_t>()(v.z);
			return x ^ y | z;
		}
	};

	using MehsDataMap = std::unordered_map<Key,Val,Vec3UIntHash>;

private :

	char m_State;

	ModelData m_ModelData; 
	ModelData m_ReturnData;
	MehsDataMap m_MeshMap;


	void ParseString(char * str);
	void ParseVertex(char * vertex_str);
	void ParseNormal(char * normal_str);
	void ParseTexture(char * texture_str);
	void ParseFace(char * face_str);

	void NewMesh();

	void ReserveContainers();
public:
	[[nodiscard]] ModelData & load(const std::string & filepath);
};

#endif // !_MY_OBJ_LOADER_HPP_
