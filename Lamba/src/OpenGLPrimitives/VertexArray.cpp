#include "VertexArray.hpp"

VertexArray::VertexArray() : m_RendererID(0),m_AttribIndex(0),m_Attrib_Offset(0)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));

}
VertexArray::~VertexArray() {
	if(m_RendererID != 0)
		GLCall(glDeleteVertexArrays(1, &m_RendererID));

}

VertexArray::VertexArray(VertexArray && va):m_RendererID(va.m_RendererID)
{
	va.m_RendererID = 0;
}
