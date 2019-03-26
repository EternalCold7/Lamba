#include "MyObjLoader.hpp"
#include <cstdio>
#include <rxcpp/rx.hpp>
#include "../Model.hpp"

namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}

const unsigned int MyObjLoader::reserve_amount = 100000;

void MyObjLoader::ParseFace(char * str)
{
	char * temp = _strdup(str);
	char * tmp = strtok(temp, " ");
	int i = 0;
	uint32_t first = ~0u;
	while ( tmp = strtok(nullptr, " "))
	{
		
		auto & currMesh = m_ReturnData.meshes.back().m_Faces;
		bool hasTexture = strstr(tmp, "//") == nullptr;
		
		
		auto res = strtok(tmp, "/");
		unsigned int converted;
		sscanf(res, "%u", &converted);
		glm::vec<3, uint32_t> vertex;
		vertex.x = converted;


		res = strtok(nullptr, "/");
		if (hasTexture) {
			sscanf(res, "%u", &converted);
			vertex.y = converted;
		}
		else {
			vertex.y = ~0u;
			sscanf(res, "%u", &converted);
			vertex.z = converted;
		}



		res = strtok(nullptr, "/");

		if (res != nullptr) {
			sscanf(res, "%u", &converted);
			vertex.z = converted;
		}



		if (i > 2) {
			auto last = currMesh.back();
			currMesh.push_back(first);
			currMesh.push_back(last);
		}

		if (m_MeshMap.find(vertex) == m_MeshMap.end()) {
			m_MeshMap.insert(std::make_pair(vertex, m_MeshMap.size()));
			m_ReturnData.verticies.push_back(m_ModelData.verticies[vertex.x - 1]);

			if (vertex.y != ~0u) 
			 	m_ReturnData.textures.push_back(m_ModelData.textures[vertex.y - 1]);

			m_ReturnData.normals.push_back(m_ModelData.normals[vertex.z - 1]);
			currMesh.push_back(m_MeshMap.size() - 1);
		}

		else {
			currMesh.push_back(m_MeshMap[vertex]);
		}
		if (first == ~0u)
			first = currMesh.back();

		++i;
		strcpy(temp, str);
		tmp = strtok(temp, " ");
		for(int j = 0; j< i;++j) 
			tmp = strtok(nullptr, " ");
	}
}

void MyObjLoader::SetCurrentMaterial(char * str)
{
	
	char t[10];
	char name[100];
	sscanf(str, "%s %s", t, name);
	auto it = m_MaterialLoader.getMaterials().find(std::string(name));
	if (it != m_MaterialLoader.getMaterials().end()) {
		m_CurrentMaterial = it->second;
	}
	else {
		throw "error";
	}
}


void MyObjLoader::ParseVertex(char * vertex_str)
{
	glm::vec3 v{};
	char s[3];
	sscanf(vertex_str,"%s %f %f %f", s, &v.x, &v.y, &v.z);
	m_ModelData.verticies.push_back(std::move(v));

}

void MyObjLoader::ParseNormal(char * normal_str) {
	glm::vec3 n{};
	char s[3];
	sscanf(normal_str,"%s %f %f %f", s, &n.x, &n.y, &n.z);
	m_ModelData.normals.push_back(std::move(n));
}
void MyObjLoader::ParseTexture(char * texture_str) {

	glm::vec2 t{};
	char s[3];
	sscanf(texture_str,"%s %f %f", s, &t.x, &t.y);
	m_ModelData.textures.push_back(std::move(t));
}

void MyObjLoader::NewMesh(const char * str) {
	char a[2];
	char name[100];
	sscanf(str, "%s %s", a, name);
	if (m_ReturnData.meshes.size() > 0) {
		if(m_ReturnData.meshes.back().m_Faces.empty())
			m_ReturnData.meshes.pop_back();
		else {
			m_ReturnData.meshes.back().mat = m_CurrentMaterial;

			MeshCoordinates coords;
			auto & faces = m_ReturnData.meshes.back().m_Faces;
			coords.maxX = m_ReturnData.verticies[faces.back()].x;
			coords.minX = m_ReturnData.verticies[faces.back()].x;
			coords.maxY = m_ReturnData.verticies[faces.back()].y;
			coords.minY = m_ReturnData.verticies[faces.back()].y;
			coords.maxZ = m_ReturnData.verticies[faces.back()].z;
			coords.minZ = m_ReturnData.verticies[faces.back()].z;
			for (auto num : faces) {

				auto x = m_ReturnData.verticies[num].x;
				auto y = m_ReturnData.verticies[num].y;
				auto z = m_ReturnData.verticies[num].z;
				if (x > coords.maxX)
					coords.maxX = x;
				else if (x < coords.minX)
					coords.minX = x;

				if (y > coords.maxY)
					coords.maxY = y;
				else if (x < coords.minY)
					coords.minY = y;

				if (z > coords.maxZ)
					coords.maxZ = z;
				else if (z < coords.minZ)
					coords.minZ = z;

			}
			m_ReturnData.meshes.back().coords = coords;

		}
			
	}
	
	
	m_ReturnData.meshes.push_back({});
	m_ReturnData.meshes.back().name = name;
}

void MyObjLoader::ParseString(char * str) {


	if (str[0] == 'v') {
		if (str[1] == 't')
			ParseTexture(str);
		else if (str[1] == 'n')
			ParseNormal(str);
		else if (str[1] == ' ')
			ParseVertex(str);
	}
	else if (str[0] == 'f')
		ParseFace(str);
	else if (str[0] == 'g' || str[0] == 'o')
		NewMesh(str);
	else {
		char s[100];
		sscanf(str, "%s", s);
		if (!strcmp(s, "usemtl"))
			SetCurrentMaterial(str);
		else if (!strcmp(s, "mtllib"))
		{
			char mtlib[7];
			char filename[100];
			sscanf(str, "%s %s", mtlib, filename);
			m_MaterialLoader.load(m_Folder, filename);
		}
	}
		
	
}


[[nodiscard]] ModelData& MyObjLoader::load(const std::string & folder, const std::string & filepath)
{

	m_Folder = folder;
	auto f = folder + filepath;
	FILE *file = fopen(f.c_str(), "r");
	if (file == nullptr)
		throw;
	char buff[2048];
	ReserveContainers();
	auto strings = Rx::observable<>::create<char*>([&buff, &file](Rx::subscriber<char *> s) {
		while (fgets(buff, sizeof(buff), file) != nullptr) {
			s.on_next(buff);
		}
		s.on_completed();
	});

	strings.filter([](char * str) { return str[0] != '#'; }).
		subscribe([this](char * str) {
		ParseString(str);
	}, [this]() {
		m_ReturnData.meshes.back().mat = m_CurrentMaterial;
	});
	return m_ReturnData;
}

void MyObjLoader::ReserveContainers() {

	m_MeshMap.reserve(reserve_amount);
	m_ReturnData.normals.reserve(reserve_amount);
	m_ReturnData.verticies.reserve(reserve_amount);
	m_ReturnData.textures.reserve(reserve_amount);
	 
	
	m_ModelData.normals.reserve(reserve_amount);
	m_ModelData.verticies.reserve(reserve_amount);
	m_ModelData.textures.reserve(reserve_amount);
}
