#ifndef MESH_HPP
#define MESH_HPP
#include <utility>
#include <vector>
#include "Vertex.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include <memory>

namespace life::kuma {
	template<typename VertexType>
	class Mesh {
	public:
	    using VertexT = VertexType;
		VertexArray 	vao;
		VertexBuffer	vbo;
		IndexBuffer 	ibo;
		std::shared_ptr<Material> material;

	public:
		Mesh() = delete;
		Mesh(Mesh&&) noexcept = default;
		Mesh(   const std::vector<VertexType>& vertices,
		        const std::vector<unsigned int>& indices,
		        std::shared_ptr<Material> material)
         : vao(),
           vbo(&vertices[0], vertices.size() * sizeof(VertexType)),
           ibo(&indices[0],  indices.size()),
           material(std::move(material))
        {
            SetupVertexArrayObject();
        }
		~Mesh() = default;
		
		static std::shared_ptr<Mesh<VertexType>> Create(const std::vector<VertexType>& vertices,
		                                                const std::vector<unsigned int>& indices,
		                                                const std::shared_ptr<Material>& material)
        {
            return std::make_shared<Mesh<VertexType>>(vertices, indices, material);
		}
		
		void Bind() const {
            material->Bind();
            vao.Bind();
            ibo.Bind();
        }
		void Unbind() const {
            material->Unbind();
            vao.Unbind();
            ibo.Unbind();
        }
		unsigned int GetIBOCount() const { return ibo.GetCount(); }
		Material& GetMaterial() const { return *material; }

    private:
        void SetupVertexArrayObject() {
            vao.AddBuffer(vbo, VertexType::GetLayout());
            vao.Unbind();
        }
	};
    using StaticMesh    = Mesh<Vertex>;
    using AnimatedMesh  = Mesh<BaseAnimationVertex>;
    using HighresAnimatedMesh = Mesh<HighresAnimationVertex>;
}

#endif //CLASSNAME_HPP
