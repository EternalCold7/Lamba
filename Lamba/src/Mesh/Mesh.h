#ifndef _MESH_H_
#define _MESH_H_
#include "../Drawable.hpp"
#include "../OpenGLPrimitives/ArrayBuffer.hpp"
#include "../OpenGLPrimitives/ElementsArrayBuffer.hpp"
#include "../OpenGLPrimitives/VertexArray.hpp"
#include "../Camera.hpp"
#include "../Shader.h"

struct MeshData {
	std::vector<glm::vec<3, std::uint32_t>> m_Faces;
};
struct MeshRawData {


	unsigned int faces_count;
	unsigned int * m_Faces;


};

struct Mesh : public Drawable {
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