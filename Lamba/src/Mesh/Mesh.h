#ifndef _MESH_H_
#define _MESH_H_
#include "../Drawable.hpp"
#include "../OpenGLPrimitives/ArrayBuffer.hpp"
#include "../OpenGLPrimitives/ElementsArrayBuffer.hpp"
#include "../OpenGLPrimitives/VertexArray.hpp"
#include "../Camera.hpp"
#include "../Shader.h"
#include "../Texture.hpp"
#include "Material.hpp"
#include <numeric>
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



struct Mesh : public Drawable {
private:
	ElementsArrayBuffer m_ElementsArray;
	std::shared_ptr<Material> m_Material;
	const Camera * m_Camera;
	std::string m_Name = "default";
	glm::mat4 translation = glm::mat4(1);
	glm::mat4 scale = glm::mat4(1);
	glm::mat4 rotation = glm::mat4(1);

	glm::mat4 proj = glm::perspective(70.f, 1000.f / 600.f, 0.1f, 1000.f);;
	const Shader * m_Shader;
	void SetupFragmentShader() const;
public:
	MeshCoordinates m_Coordinates;
	inline const std::string & GetName() const noexcept { return m_Name; }
	void SetName(const std::string & string) noexcept { m_Name = string; }
	
	Mesh(MeshData && data, const Camera * cam = nullptr, Shader * sh = nullptr);
	Mesh(MeshData & data, const Camera * cam = nullptr, Shader * sh = nullptr);
	Mesh(MeshRawData && data, const Camera * cam = nullptr, Shader * sh = nullptr);
	Mesh(MeshRawData & data, const Camera * cam = nullptr, Shader * sh = nullptr);

	void Draw() const noexcept override;
	void SetCamera(const Camera * cam) { m_Camera = cam; }
	void SetShader(const Shader * shader) { m_Shader = shader; }
	inline bool HasSpecularTexture() const { return m_Material->specular_texture.operator bool(); };
	inline bool HasDiffuseTexture() const { return m_Material->diffuse_texture.operator bool(); }

	void Rotate(const glm::vec3 & on, float deegres) noexcept {
		float midX = (m_Coordinates.maxX + m_Coordinates.minX) / 2;
		float midY = (m_Coordinates.maxY + m_Coordinates.minY) / 2;
		float midZ = (m_Coordinates.maxZ + m_Coordinates.minZ) / 2;
		glm::vec3 toCenter = { 0,0,-midZ };

		rotation = glm::rotate(rotation, deegres, on);
		//model = glm::translate(model, -toCenter);
	}
	void Translate(const glm::vec3 & offset) noexcept { translation = glm::translate(translation, offset); }
	void Scale(const glm::vec3 & by) noexcept { scale = glm::scale(scale, by); };

private:
	class AttribsSetter {
	public:
		AttribsSetter();
		~AttribsSetter();
	};
};
#endif // mesh_hpp