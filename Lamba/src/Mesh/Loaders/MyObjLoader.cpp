#include "MyObjLoader.hpp"
#include <cstdio>
void MyObjLoader::parse(char * str)
{
	if (m_State == 'v')
		m_ModelData.verticies.push_back(atof(str));
	else if (m_State  == 't')
		m_ModelData.textures.push_back(atof(str));
	else if (m_State  == 'n')
		m_ModelData.normals.push_back(atof(str));
	else if (m_State == 'f')
		parseFace(str);


}

void MyObjLoader::parseFace(char * str)
{
	auto res = strtok(str, "/");
	auto vNum = atoi(res);
	glm::vec<3, std::uint32_t> vertex;
	vertex.x = vNum;
	res = strtok(nullptr, "/");
	auto nTex = atoi(res);
	vertex.y = nTex;

	res = strtok(nullptr, "/");
	auto nNum = atoi(res);
	vertex.z =nNum;

	if (m_MeshMap.find(vertex) == m_MeshMap.end()) {
		m_MeshMap.insert(std::make_pair(vertex, m_MeshMap.size()));
		auto vIndex = (vertex.x - 1) * 3;
		m_MeshData.verticies.push_back({ m_ModelData.verticies[vIndex],
			m_ModelData.verticies[vIndex + 1], m_ModelData.verticies[vIndex + 2] });
		auto tIndex = (vertex.y - 1) * 2;
		m_MeshData.textures.push_back({m_ModelData.verticies[tIndex],m_ModelData.verticies[tIndex+1] });
		auto nIndex = (vertex.z - 1) * 3;
		m_MeshData.normals.push_back({ m_ModelData.normals[nIndex],
			m_ModelData.normals[nIndex + 1], m_ModelData.normals[nIndex + 2] });
		m_MeshData.faces.push_back(m_MeshMap.size() - 1);
	}
	else {
		m_MeshData.faces.push_back(m_MeshMap[vertex]);
	}
		


	

}

void MyObjLoader::toRawMeshData()
{
	m_MeshData.faces.shrink_to_fit();
	m_CurrMeshData.faces_count = m_MeshData.faces.size();
	m_CurrMeshData.m_Faces = m_MeshData.faces.data();

	m_MeshData.normals.shrink_to_fit();
	m_CurrMeshData.verticies_count = m_MeshData.normals.size() * 3;
	m_CurrMeshData.m_Normals = &m_MeshData.normals[0][0];

	m_MeshData.verticies.shrink_to_fit();
	m_CurrMeshData.m_Verticies = &m_MeshData.verticies[0][0];

	m_MeshData.textures.shrink_to_fit();
	m_CurrMeshData.textures_coords_count = m_MeshData.textures.size() * 2;
	m_CurrMeshData.texture_coords = &m_MeshData.textures[0][0];


}

std::vector<Mesh>& MyObjLoader::load(const std::string & filepath)
{
	FILE *file = fopen(filepath.c_str() ,"r");
	if (file == nullptr)
		throw;
	char buff[1024];
	memset(buff, 0, 1024);
	
	while (fscanf(file, "%s\n", buff) != EOF) {
		
		if (buff[0] == 'v') {
			if (buff[1] == 't') m_State = 't';
			else if (buff[1] == 'n') m_State = 'n';
			else m_State = 'v';
			continue;
		}
		else if (buff[0] == 'f') {
			m_State = 'f';
			continue;
		}
		else if (buff[0] == 'g') {
			if (!m_MeshData.verticies.empty()) {
				toRawMeshData();
				m_Meshes.emplace_back(m_CurrMeshData);
			}
				
			m_MeshData = mesh_data();
			m_MeshMap = MehsDataMap();
			continue;
		}
		else if(buff[0] == 's'){
			m_State = 's';
		}
		else {
			parse(buff);
		}
	}
	if (!m_MeshData.verticies.empty()) {
		toRawMeshData();
		m_Meshes.emplace_back(m_CurrMeshData);
	}
	return m_Meshes;
}
