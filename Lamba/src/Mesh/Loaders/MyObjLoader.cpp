#include "MyObjLoader.hpp"
#include <cstdio>
#include <rxcpp/rx.hpp>

namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}


void MyObjLoader::ParseFace(char * str)
{
	if (strlen(str) > 100)
		std::cout << strlen(str) << std::endl;
	char * temp = _strdup(str);
	char * tmp = strtok(temp, " ");
	int i = 0;

	while ( tmp = strtok(nullptr, " "))
	{
		uint32_t first = ~0u;

		bool hasTexture = strstr(tmp, "//") == nullptr;
		
		
		auto res = strtok(tmp, "/");
		unsigned int converted;
		sscanf(res, "%u", &converted);
		glm::vec<3, std::uint32_t> vertex;
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
			auto last = m_MeshData.faces.back();
			m_MeshData.faces.push_back(first);
			m_MeshData.faces.push_back(last);
		}

		if (m_MeshMap.find(vertex) == m_MeshMap.end()) {
			m_MeshMap.insert(std::make_pair(vertex, m_MeshMap.size()));
			m_MeshData.verticies.push_back(m_ModelData.verticies[vertex.x - 1]);

			/*if (vertex.y != ~0u) 
				m_MeshData.textures.push_back(m_ModelData.verticies[vertex.y - 1]);*/

			m_MeshData.normals.push_back(m_ModelData.normals[vertex.z - 1]);
			m_MeshData.faces.push_back(m_MeshMap.size() - 1);
		}

		else {
			m_MeshData.faces.push_back(m_MeshMap[vertex]);
		}
		if (first == ~0u)
			first = m_MeshData.faces.back();

		++i;
		temp = _strdup(str);
		tmp = strtok(temp, " ");
		for(int j = 0; j< i;++j) 
			tmp = strtok(nullptr, " ");
	}


}

void MyObjLoader::toRawMeshData()
{
	m_MeshData.faces.shrink_to_fit();
	m_MeshData.normals.shrink_to_fit();
	m_MeshData.verticies.shrink_to_fit();
	m_MeshData.textures.shrink_to_fit();


	m_CurrMeshData.faces_count = m_MeshData.faces.size();
	m_CurrMeshData.m_Faces = m_MeshData.faces.data();

	m_CurrMeshData.verticies_count = m_MeshData.verticies.size() * 3;


	if (!m_MeshData.normals.empty())
		m_CurrMeshData.m_Normals = &m_MeshData.normals[0][0];

	m_MeshData.verticies.shrink_to_fit();
	m_CurrMeshData.m_Verticies = &m_MeshData.verticies[0][0];

	m_MeshData.textures.shrink_to_fit();
	if (!m_MeshData.textures.empty())
	{
		m_CurrMeshData.textures_coords_count = m_MeshData.textures.size() * 2;
		m_CurrMeshData.texture_coords = &m_MeshData.textures[0][0];
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

void MyObjLoader::NewMesh() {
	//if (!m_MeshData.verticies.empty()) {
	//	toRawMeshData();
	//	m_Meshes.emplace_back(m_CurrMeshData);
	//}

	//m_MeshData = mesh_data();
	//m_MeshMap = MehsDataMap();

	////m_MeshMap.reserve(10000);
	////m_MeshData.faces.reserve(100000);
	////m_MeshData.textures.reserve(100000);
	////m_MeshData.normals.reserve(100000);
	//m_MeshData.verticies.reserve(100000);
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
	else if (str[0] == 'g')
		NewMesh();
}
std::vector<Mesh>& MyObjLoader::load(const std::string & filepath)
{
	unsigned int reserve_amount = 10000000;
	m_MeshMap.reserve(reserve_amount);
	m_MeshData.faces.reserve(reserve_amount);
	m_MeshData.textures.reserve(reserve_amount);
	m_MeshData.normals.reserve(reserve_amount);
	m_MeshData.verticies.reserve(reserve_amount);
	FILE *file = fopen(filepath.c_str(), "r");
	if (file == nullptr)
		throw;
	char buff[2048];

	auto strings = Rx::observable<>::create<char*>([&buff, &file](Rx::subscriber<char *> s) {
		while (fgets(buff, sizeof(buff), file) != nullptr) {
			s.on_next(buff);
		}
		s.on_completed();
	});

	strings.filter([](char * str) { return str[0] != '#'; }).
		subscribe([this](char * str) {
		ParseString(str);
	}, []() {});


	if (!m_MeshData.verticies.empty()) {
		toRawMeshData();
		m_Meshes.emplace_back(m_CurrMeshData);
	}
	return m_Meshes;
}
