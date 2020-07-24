#include "Texture.hpp"
#include "utils.h"
#include <stb/stb_image.h>
#include <iostream>
#include <unordered_map>
#include <life_core.h>

namespace life::kuma {

	Texture::Texture(const std::string& path, int x_wrapping, int y_wrapping)
		: m_RendererID(0), m_filepath(path),
		m_width(0), m_height(0), m_BPP(0), m_texid(0),
		m_databuffer(nullptr)
	{
		if(!LoadTexture(path, x_wrapping, y_wrapping)) {
			LIFE_ERR("Unable to load Texture '%s'", m_filepath.c_str());
		}
	}
	
	Texture::~Texture() {
		GLCall(glDeleteTextures(1, &m_RendererID));
	}
    std::unordered_map<std::string, std::shared_ptr<kuma::Texture>> texture_pool{};
	std::shared_ptr<Texture> Texture::FromFilePath(const std::string& filepath, int x_wrapping, int y_wrapping) {
		auto it = texture_pool.find(filepath);
		if(it != texture_pool.end())
		    return it->second;
		auto res = std::make_shared<Texture>(filepath, x_wrapping, y_wrapping);
        texture_pool[filepath] = res;
	    return res;
	}
	
	bool Texture::LoadTexture(const std::string& path, int x_wrapping, int y_wrapping) {		
		if(!path.empty()) {
			// Load the image into the databuffer
			stbi_set_flip_vertically_on_load(1);
			m_databuffer = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_BPP, 4); // TODO: 4? (4 channels (rgba))
			
			GLCall(glGenTextures(1, &m_RendererID));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
			
			// Set magnification & minification settings
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); 
			// Set the s,t wrapping settings
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, x_wrapping)); 
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, y_wrapping)); 
			
			// Upload the data to the GPU
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_databuffer));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D)); // TODO: We don't need mipmaps for every single texture...
			Unbind();

			if(m_databuffer) {
				// Free the databuffer from the heap.
				stbi_image_free(m_databuffer);
				return true;
			} else
				return false;
		}
		return false;
	}
	
	void Texture::Bind(unsigned int slot) const {
		GLCall(glActiveTexture(GL_TEXTURE0 + slot + m_texid));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}
	
	void Texture::Unbind() const {
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

    std::unordered_map<std::string, std::shared_ptr<kuma::Texture>>& Texture::GetTexturePool() {
        return texture_pool;
    }
}
