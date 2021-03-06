#ifndef _MTL_LOADER_HPP_
#define _MTL_LOADER_HPP_

#include"../Material.hpp"
#include <unordered_map>
class MtlLoader {
	using Value = std::shared_ptr<Material>;
	using  Key = std::string;
	
	void ParseString(const char * str);
	void AddMaterial(char * name);
	std::shared_ptr<Material> m_CurrentMaterial;
	std::string m_Folder;
public:
	std::unique_ptr< std::unordered_map<Key, Value> >m_Materials;
	void load(const std::string & folder, const std::string & filepath);
private:
	void SetAmbient(const char * str);
	void SetDiffuse(const char * str);
	void SetSpecular(const char * str);
	void LoadDiffuseTexture(const char * str);
	void LoadSpecularTexture(const char * str);
	void SetSpecCof(const char * str);
};


#endif // !_MTL_LOADER_HPP_

