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

class Model {

private:
	VertexArray m_VertexArray;
	ArrayBuffer m_ArrayBuffer;
	std::vector<Mesh> m_Meshes;
public:


	template<class Loader>
	Model(const std::string & folder, const std::string & filename, Loader & l) {

		l.load(folder, filename);
		HandleModelData(l.GetData());
	}

	Model(Model&& mod) : m_VertexArray(std::move(mod.m_VertexArray)), m_ArrayBuffer(std::move(mod.m_ArrayBuffer)),
		m_Meshes(std::move(mod.m_Meshes))
	{}
	inline const VertexArray& GetVertexArray() const { return m_VertexArray; }
	inline const ArrayBuffer& GetArrayBuffer() const { return m_ArrayBuffer; }

	void HandleModelData(ModelData & data);
	inline std::vector<Mesh> & GetMeshes() { return m_Meshes; }
};




#endif // !_model_hpp_



