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

	
	class ParsingStrategy {
	public:
		virtual void HandleString(MyObjLoader * loader, char * str) = 0;
	};

	class FaceParsingStrategy;
	class VertexParsingStrategy;
	class NormalParsingStrategy;
	class TextureParsingStrategy;
	class MeshCreatingParsingStrategy;
	class MaterialsParsingStrategy;
	class MaterialSetParsingStrategy;
	class DoNothingParsingStrategy;
	friend ParsingStrategy;

	class StrategyFactory {
		char m_State[3];
		ParsingStrategy * m_CurrentStrategy = nullptr;

	public:
		ParsingStrategy * CreateStrategy(char * str);
	};


private :

	std::string m_Folder;

	std::vector<glm::vec3> m_Verticies;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_TextureCoordinates;

	ModelData m_ReturnData;
	MehsDataMap m_MeshFacesMap;

	std::shared_ptr<Material> m_CurrentMaterial;
	StrategyFactory m_ParsingFactory;

	std::unique_ptr< std::unordered_map<std::string, std::shared_ptr<Material>>> m_Materilas;


	void ReserveContainers();
public:
	void load(const std::string & folder,const std::string & filepath);
	[[nodiscard]]ModelData & GetData() { return m_ReturnData; };
};

#endif // !_MY_OBJ_LOADER_HPP_
