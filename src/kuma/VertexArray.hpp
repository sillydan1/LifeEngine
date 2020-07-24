#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP
#include "utils.h"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace life { namespace kuma {
	class VertexArray {
	private:
		GLuint m_renderID;
	public:
		// Constructor & Destructor
		VertexArray();
		~VertexArray();
		
		void Bind() const;
		void Unbind() const;
		
		void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
	};
}}
#endif //VERTEXARRAY_HPP
