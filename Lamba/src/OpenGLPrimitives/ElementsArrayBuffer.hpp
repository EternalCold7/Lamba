#ifndef _ELEMENTS_ARRAY_BUFFER_HPP
#define _ELEMENTS_ARRAY_BUFFER_HPP
#include"../Renderer.hpp"

class ElementsArrayBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_DataSize;
public:
	inline const int GetDataSize() const { return m_DataSize; }
	ElementsArrayBuffer();
	ElementsArrayBuffer(const ElementsArrayBuffer & eab) = delete;
	ElementsArrayBuffer(ElementsArrayBuffer && eab);
	~ElementsArrayBuffer();
	void SetData(unsigned int size,const void * data);
	inline void Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }
	inline void Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
};
#endif // !_ELEMENTS_ARRAY_BUFFER_HPP
