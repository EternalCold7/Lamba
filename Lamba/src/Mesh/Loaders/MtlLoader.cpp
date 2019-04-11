#include"MtlLoader.hpp"
#include<rxcpp/rx.hpp>
namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}


void MtlLoader::ParseString(const char * str)
{
	char first_word[100];
	sscanf(str, "%s", first_word);
	if (!strcmp(first_word, "newmtl")) {
		char name[100];
		sscanf(str, "%s %s", first_word, name);

		AddMaterial(name);
	}
	else if (!strcmp(first_word, "Kd"))
		SetDiffuse(str);
	else if (!strcmp(first_word, "Ka"))
		SetAmbient(str);
	else if (!strcmp(first_word, "Ks"))
		SetSpecular(str);
	else if (!strcmp(first_word, "Ns"))
		SetSpecCof(str);
	else if (!strcmp(first_word, "map_Kd"))
		LoadDiffuseTexture(str);
	else if (!strcmp(first_word, "map_Ks"))
		LoadSpecularTexture(str);
}

void MtlLoader::AddMaterial(char * name)
{
	m_Materials->emplace(name, std::make_shared<Material>());
	m_CurrentMaterial = (*m_Materials)[name];
}

void MtlLoader::load(const std::string & folder, const std::string & filepath)
{
	m_Folder = folder;
	FILE * file = fopen((folder + filepath).c_str(), "r");
	if (!file) {
		throw std::runtime_error("File not opened");
	}
	m_Materials = std::make_unique<std::unordered_map<Key, Value>>();
	char buff[1024];

	auto input_stream = Rx::observable<>::create<const char *>
		([&file, &buff](Rx::subscriber<const char *> s) {
		while (fgets(buff, sizeof(buff), file))
			s.on_next(buff);
		s.on_completed();
	});

	input_stream.subscribe([this](const char * str) {
		this->ParseString(str);
	});

}

void MtlLoader::SetAmbient(const char * str)
{
	auto & am = m_CurrentMaterial->ambient;
	char b[3];
	sscanf(str, "%s %f %f %f", b, &am.x, &am.y, &am.z);
}

void MtlLoader::SetDiffuse(const char * str)
{
	auto & am = m_CurrentMaterial->diffuse;
	char b[3];
	sscanf(str, "%s %f %f %f", b, &am.x, &am.y, &am.z);
}

void MtlLoader::SetSpecular(const char * str)
{
	auto & am = m_CurrentMaterial->specular;
	char b[3];
	sscanf(str, "%s %f % f %f", b, &am.x, &am.y, &am.z);
}

void MtlLoader::LoadDiffuseTexture(const char * str)
{
	char b[7];
	char name[100];
	sscanf(str, "%s %s", b, name);
	try {
		m_CurrentMaterial->diffuse_texture = std::make_shared<Texture>(m_Folder + name);
	}
	catch (std::runtime_error a) {
	}

}

void MtlLoader::LoadSpecularTexture(const char * str)
{
	char b[7];
	char name[100];
	sscanf(str, "%s %s", b, name);
	try {
		m_CurrentMaterial->specular_texture = std::make_shared<Texture>(m_Folder + name);
	}
	catch (std::runtime_error a) {
	}
}

void MtlLoader::SetSpecCof(const char * str)
{
	char s[100];
	sscanf(str, "%s %f", s, &m_CurrentMaterial->specularCof);
}
