#ifndef _MODEL_DRAWER_HPP_
#define _MODEL_DRAWER_HPP_ 

#include "Model.hpp"
#include "../Shader.h"
#include "../Drawable.hpp"

class ModelDrawer : public virtual Drawable {

protected:
	std::unique_ptr<Model> m_Model;
	std::vector<Shader> m_Shaders;
	virtual void SetUpToDraw(const Mesh& mesh) const = 0;
public:

	virtual void Draw() const noexcept override {
		m_Model->GetVertexArray().Bind();
		m_Model->GetArrayBuffer().Bind();
		for (const auto& mesh : m_Model->GetMeshes()) {
			mesh.GetElementsArray().Bind();
			SetUpToDraw(mesh);
		}
	}

	virtual ~ModelDrawer() {};
	ModelDrawer(std::unique_ptr<Model> mod) : m_Model(std::move(mod)) { }
	void SetModel(std::unique_ptr<Model> model) { m_Model = std::move(model); }
};

#endif //