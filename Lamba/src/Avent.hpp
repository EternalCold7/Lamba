#ifndef _AVENT_HPP_
#define _AVENT_HPP_
#include "Mesh/ModelDrawer.hpp"
#include "Camera.hpp"
#include "Transformalbe.hpp"
class Avent : public Drawable {

	std::vector<Mesh *> m_Wheels;

	class AventModelDrawer final : public virtual ModelDrawer, public Transformable {
		const Camera* m_Camera;
	private:
		void SetUpShaders();
		virtual void SetUpToDraw(const Mesh& mesh) const override;
	public:
		AventModelDrawer(const Camera* cam, std::unique_ptr<Model> model = nullptr);
	};

	AventModelDrawer m_Drawer;
public:
	Avent(const Camera & cam);
	void Go();
	void Draw() const noexcept override {
		m_Drawer.Draw();
	}
};

#endif // !_AVENT_HPP_
