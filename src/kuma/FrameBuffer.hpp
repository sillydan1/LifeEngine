#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP
#include <GL/glew.h>
#include "openglmath"
// kuma
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"

namespace life::kuma {
	/* A framebuffer for postprocess things */
	class FrameBuffer {
		friend class AAFrameBuffer;
	private:
		unsigned int m_width, m_height;
	
		GLuint m_framebufferID;
		GLuint m_colorattachmentID;
		GLuint m_renderbufferID;
		
		VertexBuffer 	m_scr_vb;
		VertexArray 	m_scr_va;
		IndexBuffer 	m_scr_ib;
		std::shared_ptr<Material> m_scr_mat;
		VertexBufferLayout 	m_scr_layout;
		
	public:
		FrameBuffer(unsigned int buffer_width, unsigned int buffer_height,
					const std::string& shader_filepath);
		~FrameBuffer();
		
		Material& GetMaterial() const { return *m_scr_mat; }
		
		void BindResources() const;
		void BindColorBuffer() const;
		void Bind() const;
		void Unbind() const;
		void Resize(unsigned int width, unsigned int height);
		unsigned int GetWidth() const { return m_width; }
		unsigned int GetHeight() const { return m_height; }
		GLuint* GetFBO() { return &m_framebufferID; }
		GLuint* GetColorAttachment() { return &m_colorattachmentID; }
		glm::vec3 SampleColor(unsigned int x, unsigned int y) const;
	};
}
#endif //FRAMEBUFFER_HPP
