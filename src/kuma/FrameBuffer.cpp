#include "FrameBuffer.hpp"
#include "utils.h"
#include <iostream>
#include "default_shapes.hpp"

namespace life::kuma {
    using namespace default_shapes;
	FrameBuffer::FrameBuffer(unsigned int buffer_width, unsigned int buffer_height, const std::string& shader_filepath) 
		: m_width(buffer_width), m_height(buffer_height),
		 m_scr_vb(&screen_quad[0], 6 * 4 * sizeof(float)), m_scr_va(), m_scr_ib(&quad_indices[0], 6), m_scr_mat{Material::Create(shader_filepath)}, m_scr_layout()
	{
		m_scr_layout.Push<float>(2); // position
		m_scr_layout.Push<float>(2); // tex coords
		m_scr_va.AddBuffer(m_scr_vb, m_scr_layout);
		m_scr_mat->GetShader().Bind();
		m_scr_mat->SetProperty("u_screenTexture", 0);
		m_scr_mat->Bind();
		
		GLCall(glGenFramebuffers(1, &m_framebufferID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));

		GLCall(glGenTextures(1, &m_colorattachmentID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_colorattachmentID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer_width, buffer_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorattachmentID, 0));
		
		GLCall(glGenRenderbuffers(1, &m_renderbufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID));
		// use a single renderbuffer object for both a depth AND stencil buffer.
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, buffer_width, buffer_height)); 
		// now actually attach it
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferID)); 
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LIFE_ERR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	FrameBuffer::~FrameBuffer() {
		// Delete the Buffer objects.
		GLCall(glDeleteFramebuffers(1, &m_framebufferID));
		GLCall(glDeleteRenderbuffers(1, &m_renderbufferID));
		GLCall(glDeleteTextures(1, &m_colorattachmentID));
	}
	void FrameBuffer::BindResources() const {
		Unbind();
		GLCall(glDisable(GL_DEPTH_TEST)); // We disable depth test, so the screen mesh won't get occluded.
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		// m_scr_mat.GetShader()->Bind();
		m_scr_mat->Bind();
		m_scr_va.Bind();
		m_scr_ib.Bind();
		BindColorBuffer();
	}
	void FrameBuffer::BindColorBuffer() const {
		GLCall(glBindTexture(GL_TEXTURE_2D, m_colorattachmentID));
	}
	void FrameBuffer::Bind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));
		// GLCall(glEnable(GL_DEPTH_TEST));
	}
	void FrameBuffer::Unbind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // Binds the normal frame buffer
	}
	void FrameBuffer::Resize(unsigned int width, unsigned int height) {
		m_width = width;
		m_height = height;
		Bind();
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	}
	glm::vec3 FrameBuffer::SampleColor(unsigned int x, unsigned int y) const {
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferID));
		GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
		
		glm::vec3 Pixel;
		GLCall(glReadPixels(x, m_height-y, 1, 1, GL_RGB, GL_FLOAT, &Pixel));
		
		GLCall(glReadBuffer(GL_NONE));
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
		return Pixel;
	}
	
	
}
