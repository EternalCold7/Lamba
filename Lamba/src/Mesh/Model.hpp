#ifndef _model_hpp_
#define _model_hpp_


#include"Mesh.h"
#include<algorithm>

struct ModelData {
	std::vector < glm::vec3> verticies;
	std::vector < glm::vec3> normals;
	std::vector < glm::vec2> textures;
	std::vector<MeshData> meshes;
};

class Model : public Drawable {

private:
	VertexArray m_VertexArray;
	ArrayBuffer m_ArrayBuffer;
	std::vector<Mesh> m_Meshes;
	std::vector<Shader> m_Shaders;
	const Camera & m_Camera;

public:


	template<class Loader>
	Model(const std::string & folder,const std::string & filename, const Camera & camera,Loader & l) : m_Camera(camera) {

		m_Shaders.emplace_back("shaders/light.shader");
		m_Shaders.emplace_back( "shaders/textured_light.shader" );
		m_Shaders.emplace_back( "shaders/only_diffuse.shader" );
		l.load(folder, filename);
		HandleModelData(l.GetData());

		for (auto & mesh : m_Meshes) {
			mesh.SetCamera(&m_Camera);
			if (mesh.HasDiffuseTexture())
				mesh.SetShader(&m_Shaders[1]);
			else if (mesh.HasSpecularTexture())
				mesh.SetShader(&m_Shaders[2]);
			else mesh.SetShader(&m_Shaders[0]);
		}
		
	}
	void HandleModelData(ModelData & data);
	void Draw() const noexcept override;
	inline std::vector<Mesh> & GetMeshes() { return m_Meshes; }
};




#endif // !_model_hpp_



