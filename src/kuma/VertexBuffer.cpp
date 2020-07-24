#include "VertexBuffer.hpp"
#include "utils.h"

namespace life::kuma {
	
	VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
		GLCall(glGenBuffers(1, &m_renderID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
	VertexBuffer::~VertexBuffer() {
		GLCall(glDeleteBuffers(1, &m_renderID));
	}
	void VertexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderID));
	}
	void VertexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}
