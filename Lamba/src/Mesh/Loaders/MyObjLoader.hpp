#ifndef _MY_OBJ_LOADER_HPP_
#define _MY_OBJ_LOADER_HPP_
#include<unordered_map>
#include "../Model.hpp"
#include"MtlLoader.hpp"


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

	std::string m_Folder;

	char m_State;
	MtlLoader m_MaterialLoader;
	ModelData m_ModelData; 
	ModelData m_ReturnData;
	MehsDataMap m_MeshMap;
	std::shared_ptr<Material > m_CurrentMaterial;

	void ParseString(char * str);
	void ParseVertex(char * vertex_str);
	void ParseNormal(char * normal_str);
	void ParseTexture(char * texture_str);
	void ParseFace(char * face_str);

	void SetCurrentMaterial(char * str);

	void NewMesh(const char * str);

	void ReserveContainers();
public:
	[[nodiscard]] ModelData & load(const std::string & folder,const std::string & filepath);
};

#endif // !_MY_OBJ_LOADER_HPP_
