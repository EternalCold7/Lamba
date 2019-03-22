#ifndef _VERTEX_ARRAY_HPP_
#define _VERTEX_ARRAY_HPP_
#include"../Renderer.hpp"

class VertexArray {

private:
	unsigned int m_RendererID;
	unsigned int m_AttribIndex;
	unsigned int m_Attrib_Offset;
public:
	VertexArray();
	~VertexArray();
	VertexArray(VertexArray && va);
	VertexArray(const VertexArray & va) = delete;

	template <typename T>
	void glPushAttrib(unsigned int size, unsigned int data_size_in_bytes) {
		Bind();
		int a = size * sizeof(T);
		throw "Not implemented type";
	}
	template<>
	void glPushAttrib<float>(unsigned int size,unsigned int data_size_in_bytes) {
		Bind();
		GLCall(glVertexAttribPointer(m_AttribIndex, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (const void *)m_Attrib_Offset));
		GLCall(glEnableVertexAttribArray(m_AttribIndex));
		m_Attrib_Offset += data_size_in_bytes;
		m_AttribIndex++;
	}
	inline void Bind() const { GLCall(glBindVertexArray(m_RendererID));}
	inline void Unbind() const { GLCall(glBindVertexArray(0)); }
};
#endif // !_VERTEX_ARRAY_HPP_
