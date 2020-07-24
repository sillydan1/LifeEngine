#ifndef RENDERER_HPP
#define RENDERER_HPP

namespace life {
    class ParticleEmitterComponent;
}
#include "Mesh.hpp"
#include "Skybox.hpp"
#include <layers/staging/ComponentStaging.hpp>

namespace life::kuma {
	/*  */
	class Renderer {
	public:
	    template<typename T>
	    void Draw(const kuma::Mesh<T>& mesh) const {
            mesh.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, mesh.GetIBOCount(), GL_UNSIGNED_INT, nullptr));
            m_drawCalls++;
	    }
        template<typename T>
        void DrawLined(const kuma::Mesh<T>& mesh) const {
            mesh.Bind();
            GLCall(glDrawElements(GL_LINE_STRIP, mesh.GetIBOCount(), GL_UNSIGNED_INT, nullptr));
            m_drawCalls++;
        }
        void Draw(const Skybox& skybox) const;
        void Draw(life::ParticleEmitterComponent& particleEmitterComponent) const;
		void Clear() const;
		void SetClearColor(float r, float g, float b);
		
		// Mutable, because this is mostly just a debugging thing
		mutable unsigned int m_drawCalls; 
	};

}
#endif //CLASSNAME_HPP
