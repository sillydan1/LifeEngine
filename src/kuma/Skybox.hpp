#ifndef SKYBOX_HPP
#define SKYBOX_HPP
#include "IKumaBindable.hpp"
#include "CubeTexture.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Material.hpp"
#include <string>

namespace life::kuma {
	class Skybox {
	public:
		std::shared_ptr<Material> skybox_mat;
        TYPE_PTR(CubeTexture)  m_cubetex;
	    VertexArray  vao;
		VertexBuffer vbo;
		IndexBuffer  ibo;
		
	public:
		Skybox();
		// Skybox(const std::string& alternate_shader);
		Skybox(CubeTexture& cube_map);
		Skybox(CubeTexture&& cube_map);
		~Skybox();
		
		void Bind() const;
		void Unbind() const;
	private:
		void SetupSkybox();
	};
}
#endif //CLASSNAME_HPP
