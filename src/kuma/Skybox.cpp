#include "Skybox.hpp"
#include "VertexBufferLayout.hpp"

namespace life::kuma {
	const float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f, // 0
		-1.0f, -1.0f, -1.0f, // 1
		 1.0f, -1.0f, -1.0f, // 2
		 1.0f, -1.0f, -1.0f, // 3
		 1.0f,  1.0f, -1.0f, // 4
		-1.0f,  1.0f, -1.0f, // 5
							 
		-1.0f, -1.0f,  1.0f, // 6
		-1.0f, -1.0f, -1.0f, // 7
		-1.0f,  1.0f, -1.0f, // 8
		-1.0f,  1.0f, -1.0f, // 9
		-1.0f,  1.0f,  1.0f, // 10
		-1.0f, -1.0f,  1.0f, // 11
							 
		 1.0f, -1.0f, -1.0f, // 12
		 1.0f, -1.0f,  1.0f, // 13
		 1.0f,  1.0f,  1.0f, // 14
		 1.0f,  1.0f,  1.0f, // 15
		 1.0f,  1.0f, -1.0f, // 16
		 1.0f, -1.0f, -1.0f, // 17
							 
		-1.0f, -1.0f,  1.0f, // 18
		-1.0f,  1.0f,  1.0f, // 19
		 1.0f,  1.0f,  1.0f, // 20
		 1.0f,  1.0f,  1.0f, // 21
		 1.0f, -1.0f,  1.0f, // 22
		-1.0f, -1.0f,  1.0f, // 23
							 
		-1.0f,  1.0f, -1.0f, // 24
		 1.0f,  1.0f, -1.0f, // 25
		 1.0f,  1.0f,  1.0f, // 26
		 1.0f,  1.0f,  1.0f, // 27
		-1.0f,  1.0f,  1.0f, // 28
		-1.0f,  1.0f, -1.0f, // 29
							 
		-1.0f, -1.0f, -1.0f, // 30
		-1.0f, -1.0f,  1.0f, // 31
		 1.0f, -1.0f, -1.0f, // 32
		 1.0f, -1.0f, -1.0f, // 33
		-1.0f, -1.0f,  1.0f, // 34
		 1.0f, -1.0f,  1.0f  // 35
	};
	const unsigned int skyboxIndices[] = {
		0,1,2,3,4,5,6,
		7,8,9,10,12,13,
		14,15,16,17,18,19,
		20,21,22,23,24,25,
		26,27,28,29,30,31,
		32,33,34,35
	};
	Skybox::Skybox() 
		: skybox_mat{Material::Create(SHADERDIR "/skybox.glsl")},
		  m_cubetex{std::make_shared<CubeTexture>()},
		  vao(),
		  vbo(&skyboxVertices[0], 108*sizeof(float)), 
		  ibo(&skyboxIndices[0], 36)
	{
		SetupSkybox();
	}
	Skybox::Skybox(CubeTexture& cube_map) 
		: skybox_mat(Material::Create(SHADERDIR "/skybox.glsl")),
          m_cubetex(std::make_shared<CubeTexture>(cube_map)),
		  vao(), 
		  vbo(&skyboxVertices[0], 108*sizeof(float)), 
		  ibo(&skyboxIndices[0], 36)
	{
		SetupSkybox();
	}
    Skybox::Skybox(CubeTexture&& cube_map)
        : skybox_mat(Material::Create(SHADERDIR "/skybox.glsl")),
          m_cubetex(std::make_shared<CubeTexture>(std::move(cube_map))),
          vao(),
          vbo(&skyboxVertices[0], 108*sizeof(float)),
          ibo(&skyboxIndices[0], 36)
    {
        SetupSkybox();
    }
	Skybox::~Skybox() {
		
	}
	void Skybox::Bind() const {
        skybox_mat->Bind();

		vao.Bind();
        ibo.Bind();
	}
	void Skybox::Unbind() const {
		skybox_mat->Unbind();
		vao.Unbind();
		ibo.Unbind();
        m_cubetex->Unbind();
    }
    static TYPE_PTR(glm::vec3) skybox_selector_color = std::make_shared<glm::vec3>(0,0,0);
	void Skybox::SetupSkybox() {
		VertexBufferLayout layout;
        layout.Push<float>(3);
        vao.AddBuffer(vbo, layout);
        vao.Unbind();

		skybox_mat->SetProperty("u_cubemap", m_cubetex);
		skybox_mat->SetProperty("u_selectorcolor", skybox_selector_color);
	}
}
