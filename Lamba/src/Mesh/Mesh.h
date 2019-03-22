#ifndef _MESH_H_
#define _MESH_H_
#include "../Drawable.hpp"
#include "../OpenGLPrimitives/ArrayBuffer.hpp"
#include "../OpenGLPrimitives/ElementsArrayBuffer.hpp"
#include "../OpenGLPrimitives/VertexArray.hpp"
#include "../Camera.hpp"
#include "../Shader.h"

struct MeshData {
	std::vector<glm::vec3> m_Verticies;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_Textures;

	std::vector<glm::vec<3, std::uint32_t>> m_Faces;
};
struct MeshRawData {
	unsigned int verticies_count;
	float * m_Normals;
	float * m_Verticies;
	unsigned int faces_count;
	unsigned int * m_Faces;
	unsigned int textures_coords_count;
	float * texture_coords;

};

struct Mesh : public Drawable {
	ArrayBuffer m_ArrayBuffer;
	VertexArray m_VertexArray;
	ElementsArrayBuffer m_ElementsArray;

	const Camera * m_Camera;
	glm::mat4 model;
	glm::mat4 proj;
	const Shader * m_Shader;
public:
	Mesh(MeshData && data, const Camera * cam = nullptr, Shader * sh = nullptr);
	Mesh(MeshRawData && data, const Camera * cam = nullptr, Shader * sh = nullptr);
	Mesh(MeshRawData & data, const Camera * cam = nullptr, Shader * sh = nullptr);
	void Rotate(glm::vec3 & to, float angle) { model = glm::rotate(model, angle, to); };
	void Draw() const noexcept override;
	void SetCamera(const Camera * cam) { m_Camera = cam; }
	void SetShader(const Shader * shader) { m_Shader = shader; }
};
#endif // mesh_hpp