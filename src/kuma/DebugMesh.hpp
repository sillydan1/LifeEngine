#ifndef DEBUGMESH_HPP
#define DEBUGMESH_HPP
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include <vector>

namespace life { namespace kuma {
	/**
	 * A mesh container, that intentionally ONLY contain vertex position data
	 * in the form of floats. This data structure can't, and shouldn't use
	 * extra vertex data such as, color, normals and texture coordinates.
	 * */
	 struct DebugMesh {
		VertexArray 	vao;
		VertexBuffer	vbo;
		IndexBuffer 	ibo;
		
		DebugMesh(const std::vector<float> vertices, const std::vector<unsigned int> indices) 
			: vao(),
			  vbo(&vertices[0], vertices.size()*sizeof(float)),
			  ibo(&indices[0], indices.size())
		{
			SetupDebugMesh();
		}
		void SetupDebugMesh() {
			VertexBufferLayout layout;
			layout.Push<float>(3);
			vao.AddBuffer(vbo, layout);
			vao.Unbind();
		}
		void Bind() const {
			vao.Bind();
			ibo.Bind();
		}
		void Unbind() const {
			vao.Unbind();
			ibo.Unbind();
		}
		unsigned int GetIBOCount() { return ibo.GetCount(); }
	};
} }
#endif //DEBUGMESH_HPP
