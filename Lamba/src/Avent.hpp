#ifndef _AVENT_HPP_
#define _AVENT_HPP_
#include "Drawable.hpp"
#include "Mesh/Model.hpp"
class Avent : public Drawable {

	std::unique_ptr<Model> m_Model;
	std::vector<Mesh *> m_Wheels;


public:
	Avent(const Camera & cam);
	void Go();
	void Draw() const noexcept override { m_Model->Draw(); }
};

#endif // !_AVENT_HPP_
