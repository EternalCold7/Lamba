#include "Mesh.h"

Mesh::Mesh(MeshData && data) {
	SetName(std::move(data.name));
	m_ElementsArray.SetData(data.m_Faces.size() * sizeof(uint32_t), data.m_Faces.data());
	m_Material = data.mat;
	m_Coordinates = data.coords;
}

Mesh::Mesh(MeshData & data)
{
	SetName(std::move(data.name));
	m_ElementsArray.SetData(data.m_Faces.size() * sizeof(uint32_t), data.m_Faces.data());
	m_Coordinates = data.coords;
	m_Material = data.mat;
}

Mesh::Mesh(MeshRawData && data) {
	SetName(std::move(data.name));
	m_ElementsArray.SetData(data.faces_count * sizeof(std::uint32_t), data.m_Faces);
	m_Coordinates = data.coords;
	m_Material = data.mat;
}

Mesh::Mesh(MeshRawData & data) {
	SetName(std::move(data.name));
	m_Material = data.mat;
	m_ElementsArray.SetData(data.faces_count * sizeof(std::uint32_t), data.m_Faces);
	m_Coordinates = data.coords;

}



