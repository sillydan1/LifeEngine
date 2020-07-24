#include "VertexArray.hpp"
#include "utils.h"
#include <GL/glew.h>
#include <life_core.h>

namespace life { namespace kuma {

	VertexArray::VertexArray() { 
		GLCall(glGenVertexArrays(1, &m_renderID));
	}

	VertexArray::~VertexArray() {
        GLCall(glDeleteVertexArrays(1, &m_renderID));
	}
	
	void VertexArray::Bind() const {
		GLCall(glBindVertexArray(m_renderID));
	} 
	
	void VertexArray::Unbind() const {
		GLCall(glBindVertexArray(0));
	}
	
	void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) {
		Bind();
		buffer.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			if(element.type == GL_INT || element.type == GL_UNSIGNED_INT) {
                GLCall(glVertexAttribIPointer(i, element.count, element.type, layout.GetStride(), (const void*) offset));
			} else {
                GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
            }
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}
} }
