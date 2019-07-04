#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

class Drawable {
public:
	virtual void Draw() const noexcept = 0;
	virtual ~Drawable() {};
};


#endif //_DRAWABLE_HPP_