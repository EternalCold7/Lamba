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

const unsigned int MyObjLoader::reserve_amount = 10000;


class MyObjLoader::FaceParsingStrategy : public MyObjLoader::ParsingStrategy {
private:
	void InsertInMeshFacesMap(glm::vec<3,uint32_t> face,MyObjLoader * loader) {
		auto & currMesh = loader->m_ReturnData.meshes.back().m_Faces;
		if (loader->m_MeshFacesMap.find(face) == loader->m_MeshFacesMap.end()) {
			loader->m_MeshFacesMap.insert(std::make_pair(face, loader->m_MeshFacesMap.size()));
			loader->m_ReturnData.verticies.push_back(loader->m_Verticies[face.x - 1]);

			if (face.y != ~0u)
				loader->m_ReturnData.textures.push_back(loader->m_TextureCoordinates[face.y - 1]);

			loader->m_ReturnData.normals.push_back(loader->m_Normals[face.z - 1]);
			currMesh.push_back(loader->m_MeshFacesMap.size() - 1);
		}
		else {
			currMesh.push_back(loader->m_MeshFacesMap[face]);
		}
	}
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		char * temp = _strdup(str);
		char * tmp = strtok(temp, " ");
		int i = 0;
		uint32_t first = ~0u;
		while (tmp = strtok(nullptr, " "))
		{
			if (tmp[0] == '\n')
				continue;
			auto & currMesh = loader->m_ReturnData.meshes.back().m_Faces;
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

			InsertInMeshFacesMap(vertex, loader);

			if (first == ~0u)
				first = currMesh.back();

			++i;
			strcpy(temp, str);
			tmp = strtok(temp, " ");
			for (int j = 0; j < i; ++j)
				tmp = strtok(nullptr, " ");
		}
	}


};

class MyObjLoader::VertexParsingStrategy : public MyObjLoader::ParsingStrategy {
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		glm::vec3 v{};
		char s[3];
		sscanf(str, "%s %f %f %f", s, &v.x, &v.y, &v.z);
		loader->m_Verticies.push_back(std::move(v));
	}
};

class MyObjLoader::NormalParsingStrategy : public MyObjLoader::ParsingStrategy {
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		glm::vec3 n{};
		char s[3];
		sscanf(str, "%s %f %f %f", s, &n.x, &n.y, &n.z);
		loader->m_Normals.push_back(std::move(n));
	}
};

class MyObjLoader::TextureParsingStrategy : public MyObjLoader::ParsingStrategy {
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		glm::vec2 t{};
		char s[3];
		sscanf(str, "%s %f %f", s, &t.x, &t.y);
		loader->m_TextureCoordinates.push_back(std::move(t));
	}
};

class MyObjLoader::MeshCreatingParsingStrategy : public MyObjLoader::ParsingStrategy {
private:
	MeshCoordinates GetMeshCoordinates(MyObjLoader * loader) {
		MeshCoordinates coords;
		const auto & vert = loader->m_ReturnData.verticies;
		auto & faces = loader->m_ReturnData.meshes.back().m_Faces;
		coords.maxX = vert[faces.back()].x;
		coords.minX = vert[faces.back()].x;
		coords.maxY = vert[faces.back()].y;
		coords.minY = vert[faces.back()].y;
		coords.maxZ = vert[faces.back()].z;
		coords.minZ = vert[faces.back()].z;
		for (auto num : faces) {

			auto x = vert[num].x;
			auto y = vert[num].y;
			auto z = vert[num].z;
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
		return coords;
	}

public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		char a[2];
		char name[100];
		auto & retData = loader->m_ReturnData;
		sscanf(str, "%s %s", a, name);
		if (retData.meshes.size() > 0) {
			if (retData.meshes.back().m_Faces.empty())
				retData.meshes.pop_back();
			else {
				retData.meshes.back().mat = loader->m_CurrentMaterial;
				retData.meshes.back().coords = GetMeshCoordinates(loader);
			}

		}

		retData.meshes.push_back({});
		retData.meshes.back().name = name;
	}
};

class MyObjLoader::MaterialsParsingStrategy : public MyObjLoader::ParsingStrategy {
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		char mtlib[7];
		char filename[100];
		sscanf(str, "%s %s", mtlib, filename);
		MtlLoader mtlLoader;
		try
		{
			mtlLoader.load(loader->m_Folder, filename);
			loader->m_Materilas = std::move(mtlLoader.m_Materials);
		}
		catch (const std::exception&){}
	}
};

class MyObjLoader::MaterialSetParsingStrategy : public MyObjLoader::ParsingStrategy {
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
		if (!loader->m_Materilas) {
			if (!loader->m_CurrentMaterial) {
				loader->m_CurrentMaterial = std::make_shared<Material>();
				loader->m_CurrentMaterial->ambient = glm::vec3{ 0.135 , 0.2225 , 0.1575 };
				loader->m_CurrentMaterial->diffuse = glm::vec3{ 0.54 , 0.89 , 0.63 };
				loader->m_CurrentMaterial->specular = glm::vec3{ 0.316228 , 0.316228 , 0.316228 };
				loader->m_CurrentMaterial->specularCof = 0.1;
			}
			return;
		}
			



		char t[10];
		char name[100];
		sscanf(str, "%s %s", t, name);





		auto it = loader->m_Materilas->find(std::string(name));
		if (it != loader->m_Materilas->end()) {
			loader->m_CurrentMaterial = it->second;
		}
		else {
			throw "error";
		}
	}
};
class MyObjLoader::DoNothingParsingStrategy : public MyObjLoader::ParsingStrategy {
public:
	virtual void HandleString(MyObjLoader * loader, char * str) override {
	
	}
};



void MyObjLoader::load(const std::string & folder, const std::string & filepath)
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
		m_ParsingFactory.CreateStrategy(str)->HandleString(this, str);

	}, [this]() {
		m_ReturnData.meshes.back().mat = m_CurrentMaterial;
	});

}

void MyObjLoader::ReserveContainers() {

	m_MeshFacesMap.reserve(reserve_amount);
	m_ReturnData.normals.reserve(reserve_amount);
	m_ReturnData.verticies.reserve(reserve_amount);
	m_ReturnData.textures.reserve(reserve_amount);

	m_Verticies.reserve(reserve_amount);
	m_Normals.reserve(reserve_amount);
	m_TextureCoordinates.reserve(reserve_amount);
}



MyObjLoader::ParsingStrategy * MyObjLoader::StrategyFactory::CreateStrategy(char * str)
{
	if (!str)
		throw std::runtime_error("String is NULL");


	char first2[3];

	first2[0] = str[0];
	first2[1] = str[1];
	first2[2] = '\0';

	if(!strcmp(first2,m_State))
		return m_CurrentStrategy;
	delete m_CurrentStrategy;

	m_State[0] = first2[0];
	m_State[1] = first2[1];

	if (!strcmp(first2, "v "))
		m_CurrentStrategy = new VertexParsingStrategy();
	else if (!strcmp(first2, "vt"))
		m_CurrentStrategy = new TextureParsingStrategy();
	else if (!strcmp(first2, "vn"))
		m_CurrentStrategy = new NormalParsingStrategy();
	else if (!strcmp(first2, "f "))
		m_CurrentStrategy = new FaceParsingStrategy();
	else if (!strcmp(first2, "o ") || !strcmp(first2, "g "))
		m_CurrentStrategy = new MeshCreatingParsingStrategy();
	else if (!strcmp(first2, "mt"))
		m_CurrentStrategy = new MaterialsParsingStrategy();
	else if (!strcmp(first2, "us"))
		m_CurrentStrategy = new MaterialSetParsingStrategy();
	else m_CurrentStrategy = new DoNothingParsingStrategy();

	return m_CurrentStrategy;
}
