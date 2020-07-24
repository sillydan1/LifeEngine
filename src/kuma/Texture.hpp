#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#define TEXTUREDIR "./res/textures"
#include <string>
#include <memory>
#include <unordered_map>
#include "IKumaBindable.hpp"
#include "utils.h"

namespace life::kuma {
	class Texture : IKumaBindable {
	private:
		GLuint m_RendererID;
		std::string m_filepath;
		int m_width, m_height, m_BPP, m_texid;
		unsigned char* m_databuffer;
		
		bool LoadTexture(const std::string& path, int x_wrapping, int y_wrapping);
	public:
		Texture(const std::string& path, int x_wrapping = GL_CLAMP_TO_BORDER, int y_wrapping = GL_CLAMP_TO_BORDER);
		~Texture();
		
		static std::shared_ptr<Texture> FromFilePath(const std::string& filepath, int x_wrapping = GL_CLAMP_TO_BORDER, int y_wrapping = GL_CLAMP_TO_BORDER);
		static auto Create(const std::string& filepath, int x_wrapping = GL_CLAMP_TO_BORDER, int y_wrapping = GL_CLAMP_TO_BORDER) { return FromFilePath(filepath,x_wrapping,y_wrapping); }
		
		void Bind(unsigned int slot) const override;
		void Unbind() const override;

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
		inline GLuint GetRendererID() const { return m_RendererID; }
		inline void SetTextureID(int id) { m_texid = id; }
		inline int GetTextureID() const { return m_texid; }
		static std::unordered_map<std::string, std::shared_ptr<kuma::Texture>>& GetTexturePool();
	};
}

#endif //TEXTURE_HPP
