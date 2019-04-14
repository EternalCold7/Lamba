#ifndef _MESH_H_
#define _MESH_H_

#include "../OpenGLPrimitives/ArrayBuffer.hpp"
#include "../OpenGLPrimitives/ElementsArrayBuffer.hpp"
#include "../OpenGLPrimitives/VertexArray.hpp"

#include "Material.hpp"
#include <vector>
struct MeshCoordinates {
	float maxY, minY, maxX, minX, maxZ, minZ;
};

struct MeshData {
	std::string name;
	std::vector<uint32_t> m_Faces;
	std::shared_ptr<Material> mat;
	MeshCoordinates coords;
};

struct MeshRawData {

	std::string name;
	unsigned int faces_count;
	unsigned int * m_Faces;
	std::shared_ptr<Material> mat;
	MeshCoordinates coords;
};



struct Mesh {
private:
	ElementsArrayBuffer m_ElementsArray;
	std::shared_ptr<Material> m_Material;
	std::string m_Name = "default";
public:
	MeshCoordinates m_Coordinates;

	inline const std::string & GetName() const noexcept { return m_Name; }
	void SetName(const std::string & string) noexcept { m_Name = string; }

	Mesh(MeshData && data);
	Mesh(MeshData & data);
	Mesh(MeshRawData && data);
	Mesh(MeshRawData & data);

	inline const ElementsArrayBuffer& GetElementsArray() const { return m_ElementsArray; }
	inline const Material& GetMaterial() { return *m_Material.get(); }
	inline bool HasSpecularTexture() const { return m_Material->specular_texture.operator bool(); };
	inline bool HasDiffuseTexture() const { return m_Material->diffuse_texture.operator bool(); }

};
#endif // mesh_hpp