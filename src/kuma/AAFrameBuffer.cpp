#include "AAFrameBuffer.hpp"

namespace life::kuma {

	AAFrameBuffer::AAFrameBuffer(unsigned int buffer_width, unsigned int buffer_height) 
		: m_width(buffer_width), m_height(buffer_height)
	{
		GLCall(glGenFramebuffers(1, &m_framebufferID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));

		GLCall(glGenTextures(1, &m_aaAttachmentID));
		// Multisampling testing (4x)
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_aaAttachmentID));
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, buffer_width, buffer_height, GL_TRUE));
		GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_aaAttachmentID, 0));
		
		GLCall(glGenRenderbuffers(1, &m_renderbufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID));

		GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, buffer_width, buffer_height));
		
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferID));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LIFE_ERR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	AAFrameBuffer::~AAFrameBuffer() {
		GLCall(glDeleteFramebuffers(1, &m_framebufferID));
		GLCall(glDeleteRenderbuffers(1, &m_renderbufferID));
		GLCall(glDeleteTextures(1, &m_aaAttachmentID));
	}
	void AAFrameBuffer::Blit(const FrameBuffer& target) const {
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferID));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.m_framebufferID));
        GLCall(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}
	void AAFrameBuffer::Bind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));
		// GLCall(glEnable(GL_DEPTH_TEST));
	}
	void AAFrameBuffer::Unbind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // Binds the normal frame buffer
	}
	
	void AAFrameBuffer::Resize(unsigned int width, unsigned int height) {
		m_width = width;
		m_height = height;
		Bind();
		GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_width, m_height, GL_TRUE));
	}
}
