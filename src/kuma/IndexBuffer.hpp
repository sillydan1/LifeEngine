#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP
#include <GL/glew.h>

namespace life { namespace kuma {

	class IndexBuffer {
	private:
		GLuint m_renderID;
		GLuint m_Count;
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();
		
		void Bind() const;
		void Unbind() const;
		
		inline unsigned int GetCount() const { return m_Count; }
	};
} }
#endif //INDEXBUFFER_HPP
