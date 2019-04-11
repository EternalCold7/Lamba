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
	for (auto & m : m_Wheels) {
		auto midx = (m->m_Coordinates.maxX + m->m_Coordinates.minX ) / 2;
		auto midy = (m->m_Coordinates.maxY + m->m_Coordinates.minY ) / 2;
		auto midz = (m->m_Coordinates.maxZ + m->m_Coordinates.minZ ) / 2;

		m->Translate({ midx,midy,midz });
		m->Rotate({ 0,0,1 }, deegres);
		m->Translate({ -midx,-midy,-midz });
	}
		
}
