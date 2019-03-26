#include "Avent.hpp"
#include"Mesh/Loaders/MyObjLoader.hpp"

Avent::Avent(const Camera & cam)
{
	MyObjLoader loader;
	m_Model = std::make_unique<Model>("CarModel/Another/","Avent.obj",cam,loader);
	for (auto & mesh : m_Model->GetMeshes()) {
		if (mesh.GetName().find("wheel") != std::string::npos)
			m_Wheels.push_back(&mesh);
	}
}

void Avent::Go()
{
	float deegres = 5;

	deegres = glm::radians(deegres);
	for (auto & m : m_Wheels)
		m->Rotate({0,0,1}, deegres);	
}
