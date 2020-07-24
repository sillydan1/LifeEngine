#include "IndexBuffer.hpp"
#include "utils.h"

namespace life { namespace kuma {
	
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		: m_Count(count) 
	{
		GLCall(glGenBuffers(1, &m_renderID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
	IndexBuffer::~IndexBuffer() {
		GLCall(glDeleteBuffers(1, &m_renderID));
	}
	void IndexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID));
	}
	void IndexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
} }
