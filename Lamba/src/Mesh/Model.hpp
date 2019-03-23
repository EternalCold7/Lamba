#ifndef _model_hpp_
#define _model_hpp_


#include"Mesh.h"
#include <chrono>
#include<algorithm>

struct ModelData {
	std::vector < glm::vec3> verticies;
	std::vector < glm::vec3> normals;
	std::vector < glm::vec2> textures;
	std::vector<std::vector<std::uint32_t>> meshes;
};

class Model : public Drawable {

private:
	VertexArray m_VertexArray;
	ArrayBuffer m_ArrayBuffer;
	std::vector<Mesh> m_Meshes;
	Shader m_Shader;
	const Camera & m_Camera;

public:


	template<class Loader>
	Model(const std::string & filepath, const std::string & shader_path, const Camera & camera,Loader & l) :
		m_Shader(shader_path), m_Camera(camera) {


		HandleModelData(l.load(filepath));

		for (auto & mesh : m_Meshes) {
			mesh.SetCamera(&m_Camera);
			mesh.SetShader(&m_Shader);
		}
		
	}
	void HandleModelData(ModelData & data);
	void Draw() const noexcept override;


	void Rotate(glm::vec3 & to, float angle) {
		for (auto & mesh : m_Meshes)
			mesh.Rotate(to, angle);
	}
	
};




#endif // !_model_hpp_



