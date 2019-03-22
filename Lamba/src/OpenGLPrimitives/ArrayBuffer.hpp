#ifndef _ARRAY_BUFFER_HPP_
#define _ARRAY_BUFFER_HPP_
#include"../Renderer.hpp"

class ArrayBuffer {

private:
	unsigned int m_RendererID;
public:
	ArrayBuffer();
	~ArrayBuffer();
	ArrayBuffer(ArrayBuffer && ab);
	ArrayBuffer(const ArrayBuffer & buf) = delete;
	void SetData(unsigned int size,const void * data);
	void SetSubData(unsigned int offset, unsigned int size, const void * data);
	inline void Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); }
	inline void Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

};

#endif // _ARRAY_BUFFER_HPP_
