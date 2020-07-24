#ifndef AAFRAMEBUFFER_HPP
#define AAFRAMEBUFFER_HPP
#include "FrameBuffer.hpp"

namespace life::kuma {
	class AAFrameBuffer {
	private:
		unsigned int m_width;
		unsigned int m_height;
		
		GLuint m_framebufferID;
		GLuint m_aaAttachmentID;
		GLuint m_renderbufferID;
		
	public:
		AAFrameBuffer(unsigned int buffer_width, unsigned int buffer_height);
		~AAFrameBuffer();
		
		void Bind() const;
		void Unbind() const;
		void Blit(const FrameBuffer& target) const;
		void Resize(unsigned int width, unsigned int height);
		unsigned int GetWidth() const { return m_width; }
		unsigned int GetHeight() const { return m_height; }
		GLuint* GetFBO() { return &m_framebufferID; }
	};
}
#endif //AAFRAMEBUFFER_HPP
