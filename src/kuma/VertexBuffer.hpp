#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP
#include <GL/glew.h>

namespace life::kuma {

	class VertexBuffer {
	private:
		GLuint m_renderID;
		
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();
		
		void Bind() const;
		void Unbind() const;
	};
}
#endif //BUFFER_HPP
