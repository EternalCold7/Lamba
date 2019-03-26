#include"ArrayBuffer.hpp"

ArrayBuffer::ArrayBuffer() : m_RendererID(0) {
	GLCall(glGenBuffers(1, &m_RendererID));
}

ArrayBuffer::~ArrayBuffer() {
	if(m_RendererID != 0)
		GLCall(glDeleteBuffers(1, &m_RendererID));
}

ArrayBuffer::ArrayBuffer(ArrayBuffer && ab)
{
	m_RendererID = ab.m_RendererID;
	ab.m_RendererID = 0;
}

void ArrayBuffer::SetData(unsigned int size, const void * data)
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void ArrayBuffer::SetSubData(unsigned int offset, unsigned int size, const void * data)
{
	Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
