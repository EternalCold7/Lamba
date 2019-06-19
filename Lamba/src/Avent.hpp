#ifndef _AVENT_HPP_
#define _AVENT_HPP_
#include "Mesh/ModelDrawer.hpp"
#include "Camera.hpp"
#include "Transformalbe.hpp"
#include "Light/Lamp.hpp"
class Avent : public Drawable {
	   	class AventModelDrawer final : public virtual ModelDrawer, public Transformable {
		const Camera* m_Camera;
		const std::vector<Lamp> * m_Lights = nullptr;
	private:
		
		void SetUpShaders();
		virtual void SetUpToDraw(const Mesh& mesh) const override;
		ShaderSetterForLight m_Setter;
	public:
		void SetLights(const std::vector<Lamp> * lights) { m_Lights = lights; }
		AventModelDrawer(const Camera* cam, std::unique_ptr<Model> model = nullptr);
	};

	std::vector<Lamp> m_LightSources;
	AventModelDrawer m_Drawer;
public:

	Avent(const Camera & cam);
	void Draw() const noexcept override {
		m_Drawer.Draw();
		for (auto const & lamp : m_LightSources)
			lamp.Draw();
	}
	 AventModelDrawer& GetModelDrawer()  { return m_Drawer; }
};

#endif // !_AVENT_HPP_
