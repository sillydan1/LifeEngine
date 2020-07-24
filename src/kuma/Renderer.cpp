#include <util/DebugInfo.h>
#include "Renderer.hpp"
#include "utils.h"
	// Various Draw functions
	/*void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Material& material) const {
		material.Bind();
        va.Bind();
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
        m_drawCalls++;
	}*/
namespace life::kuma {

    void Renderer::Draw(ParticleEmitterComponent& d) const {
        // TODO: Billboarding
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
        GLCall(glDisable(GL_CULL_FACE)); // TODO: Fix the normals of the quads
        // Bind the geometry
        //GLCall(glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ));

        d.mesh->Bind();
        // Manually bind the uniforms, because the Material system is not designed for this...
        auto& shader = d.mesh->material->GetShader();
        auto offs_loc = shader.GetUniformLocation("u_offset");
        auto colr_loc = shader.GetUniformLocation("u_color");
        // Sort particles
        auto cp = kuma::Material::GetGlobalProperty("u_viewPos"); // The current camera position
        if(cp) { // We can't sort particles if there's no camera...
            for (auto &p : d.particles) {
                // if(p.life <= 0) continue;
                glm::vec4 world_pos4 = d.transform->GetModelMatrix() * glm::vec4(p.position, 1.0f);
                glm::vec3 world_pos3 = {world_pos4.x, world_pos4.y, world_pos4.z};
                try {
                    auto c = std::get<TYPE_PTR(glm::vec3)>(*cp);
                    p.distancetocamera = std::sqrt(glm::length2(world_pos3 - *c));
                }
                catch (const std::bad_variant_access&) { // TODO: This is pretty effing bad man... exceptions in the renderloop
                    LIFE_ERR("There's something very wrong with the rendering system / pipeline. Camera positions are not vec3's!");
                }
            }
            std::sort(d.particles.begin(), d.particles.end());
        }

        for (auto& particle : d.particles) { // TODO: Instancing
            if (particle.life > 0.0f) {
                shader.SetUniform(offs_loc, std::make_shared<glm::vec3>(particle.position)); // TODO: Should be at the emitters position + particles position
                shader.SetUniform(colr_loc, std::make_shared<glm::vec4>(particle.color));
                GLCall(glDrawElements(GL_TRIANGLES, d.mesh->GetIBOCount(), GL_UNSIGNED_INT, nullptr));
            }
        }
        d.mesh->Unbind();
        //GLCall(glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_CULL_FACE));
    }

    void Renderer::Draw(const Skybox& skybox) const {
        GLCall(glDepthFunc(GL_LEQUAL));
        skybox.Bind();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, skybox.ibo.GetCount()));
        m_drawCalls++;
    }
	
    float clear_r = 0.1f, clear_g = 0.1f, clear_b = 0.15f;
	void Renderer::Clear() const {
		DebugInfo::DrawCalls = m_drawCalls;
		glClearColor(clear_r,clear_g,clear_b,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		m_drawCalls = 0;
	}
	
	void Renderer::SetClearColor(float r, float g, float b) {
		clear_r = r;
		clear_g = g;
		clear_b = b;
	}

}
