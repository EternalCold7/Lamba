#include "ElementsArrayBuffer.hpp"

ElementsArrayBuffer::ElementsArrayBuffer()
{
	GLCall(glGenBuffers(1, &m_RendererID));
}

ElementsArrayBuffer::~ElementsArrayBuffer()
{
	if(m_RendererID != 0)
		GLCall(glDeleteBuffers(1, &m_RendererID));
}

void ElementsArrayBuffer::SetData(unsigned int size, const void * data)
{
	Bind();
	m_DataSize = size / sizeof(unsigned int);
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size , data, GL_STATIC_DRAW));
}

ElementsArrayBuffer::ElementsArrayBuffer(ElementsArrayBuffer && eab) : 
	m_RendererID(eab.m_RendererID),m_DataSize(eab.m_DataSize) {
	eab.m_RendererID = 0;
}