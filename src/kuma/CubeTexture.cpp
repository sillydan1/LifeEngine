#include "CubeTexture.hpp"
#include "utils.h"
#include <stb/stb_image.h>
#include <iostream>
#include <life_core.h>

namespace life { namespace kuma {
	
	CubeTexture::CubeTexture() 
		: m_filepaths { 
			getrespath()+"/textures/skybox/right.jpg",
			getrespath()+"/textures/skybox/left.jpg",
			getrespath()+"/textures/skybox/top.jpg",
			getrespath()+"/textures/skybox/bottom.jpg",
			getrespath()+"/textures/skybox/front.jpg",
			getrespath()+"/textures/skybox/back.jpg"  }
	{
		m_RendererID = LoadCubeMap();
	}
	CubeTexture::CubeTexture(const std::string& prefix, const std::string& right, 
							 const std::string& left,   const std::string& top, 
							 const std::string& bottom, const std::string& front,
							 const std::string& back ) 
		: m_filepaths { 
			prefix + right, 
			prefix + left,
			prefix + top,
			prefix + bottom,
			prefix + front,
			prefix + back }
	{
		m_RendererID = LoadCubeMap();
	}
	CubeTexture::CubeTexture(const std::string& prefix, const std::string& postfix) 
		: m_filepaths { 
			prefix + "right" + postfix, 
			prefix + "left"  + postfix,
			prefix + "top"   + postfix,
			prefix + "bottom"+ postfix,
			prefix + "front" + postfix,
			prefix + "back"  + postfix}
	{
		m_RendererID = LoadCubeMap();
		if(m_RendererID == -1)
		    LIFE_ERR("Loading cube map didn't go well...");
	}
	CubeTexture::~CubeTexture() {
		ClearCubeMap();
		m_filepaths.clear();
	}
	unsigned int CubeTexture::LoadCubeMap() {
		GLuint texID;
		// Generate the texture
		GLCall(glGenTextures(1, &texID));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, texID));
		
		// Load the textures
		int width, height, nrChannels;
		unsigned char* data;
		stbi_set_flip_vertically_on_load(0);
		for (unsigned int i = 0; i < m_filepaths.size(); i++) {
			data = stbi_load(m_filepaths[i].c_str(), &width, &height, &nrChannels, 0);
			// Note: We're using GL_RGB here and not GL_RGBA8, since cubemaps can't be transparent anyway.
			GLCall(glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));

			if (data)
				stbi_image_free(data); // Free the databuffer from the heap.
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        return texID;
	}
	void CubeTexture::ChangeTexture(unsigned char tex_index, const std::string& file_path) {
		// We only need to check for max, since unsigned can't be negative.
		if (tex_index < m_filepaths.size())
			m_filepaths[tex_index] = file_path;
		
		// Reload
		ClearCubeMap();
		LoadCubeMap();
	}
	void CubeTexture::ClearCubeMap() const {
		GLCall(glDeleteTextures(1, &m_RendererID));
	}
	void CubeTexture::Bind(unsigned int slot) const {
		GLCall(glActiveTexture(GL_TEXTURE0 + slot + m_texid));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
	}
	
	void CubeTexture::Unbind() const {
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
} }
