#ifndef BUFFERLAYOUT_HPP
#define BUFFERLAYOUT_HPP
#include <vector>
#include <GL/glew.h>
#include <cassert>
#include <life_core.h>

namespace life::kuma {
	
	struct VertexBufferElement {
		unsigned int type;
		unsigned int count;
		unsigned int normalized;
		
		static unsigned int GetSizeOfType(unsigned int type) {
			switch(type) {
				case GL_FLOAT: 			return 4;
				case GL_UNSIGNED_INT: 	return 4;
				case GL_INT: 	        return 4;
				case GL_UNSIGNED_BYTE: 	return 1;
			}
			LIFE_ERR("VertexBufferElement type is not supported yet!");
			assert(false);
			return 0;
		}
	};
	
	class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		// Constructor & Destructor
		VertexBufferLayout()
			: m_Stride(0) {}
		
		template<typename T>
		void Push(unsigned int count);
		
		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};
}
#endif //CLASSNAME_HPP
