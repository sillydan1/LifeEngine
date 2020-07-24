#ifndef CUBE_TEXTURE_HPP
#define CUBE_TEXTURE_HPP
#include "Texture.hpp"
#include "IKumaBindable.hpp"
#include <vector>

namespace life { namespace kuma {
	class CubeTexture : IKumaBindable {
	private:
		GLuint m_RendererID;
		std::vector<std::string> m_filepaths;
		int m_texid;
		
	public:
		CubeTexture();
		CubeTexture(const std::string& prefix, const std::string& right, 
					const std::string& left,   const std::string& top, 
					const std::string& bottom, const std::string& front,
					const std::string& back );
		CubeTexture(const std::string& prefix, const std::string& postfix);
		~CubeTexture();
		
		void ChangeTexture(unsigned char tex_index, const std::string& file_path);
		
		void Bind(unsigned int slot) const override;
		void Unbind() const override;
        inline GLuint GetRendererID() const { return m_RendererID; }
        inline void SetTextureID(int id) { m_texid = id; }
        inline int GetTextureID() const { return m_texid; }
	private:
		void ClearCubeMap() const;
		unsigned int LoadCubeMap();
	};
} }
#endif //TEXTURE_HPP
