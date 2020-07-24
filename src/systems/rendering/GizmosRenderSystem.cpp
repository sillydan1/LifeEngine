#include "GizmosRenderSystem.hpp"
#include <components/TransformComponent.hpp>

namespace life {
	
	GizmosRenderSystem::GizmosRenderSystem(kuma::Renderer &renderer)
	 : renderRef{&renderer}
	{
		AddComponentType(ParticleEmitterComponent::ID);
		FinalizeComponentAdding();
	}
	
	void GizmosRenderSystem::Update(const std::shared_ptr<ecs::BaseComponent> &components) {
		System::Update(components);
	}
	
	void GizmosRenderSystem::RenderSelectedObject(LayerStack *owner, RenderSystem& rendersystem) {
		auto obj = owner->GetLayerResource("LIFE_SELECTED_OBJ");
		if(!obj.second) return;
		auto entity = static_cast<std::vector<std::shared_ptr<ecs::BaseComponent>>*>(obj.second);
		kuma::Material::SetGlobalProperty("u_isselected", true);
		GLint polygonMode;
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glGetIntegerv(GL_POLYGON_MODE, &polygonMode));
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		for(auto& c : *entity) {
		    if(c->GetID() == TransformComponent::ID)
                RenderTransformGizmo(*dynamic_cast<TransformComponent&>(*c).transform, rendersystem);
			rendersystem.Update(c);
		}
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, polygonMode));
		kuma::Material::SetGlobalProperty("u_isselected", false);
	}

    std::pair<std::vector<kuma::Vertex>, std::vector<unsigned int>> right_arrow() {
	    unsigned int N = 32;
        float width = 0.2f;
	    float length = 1;
        float tiplength = 0.5f;
        std::vector<kuma::Vertex> ret = {{{0,0,0},{0,0,0},{0,0}},
                                   {{length,0,0},{0,0,0},{0,0}}};
        float twopi = 2*glm::pi<float>();
        float NthofCirc = twopi / N;
        for(int i = 0; i < N; i++) {
            glm::vec3 nadd = {length, width*glm::sin(i * NthofCirc), width*glm::cos(i * NthofCirc) };
            ret.push_back({nadd,{0,0,0},{0,0}});
        }
        ret.push_back({{length+tiplength,0,0},{0,0,0},{0,0}});
        std::vector<unsigned int> indices = {};
        indices.reserve(ret.size()+1);
        int i = 0;
        for(; i < ret.size()-1; i++) {
            indices.push_back(i);
        }
        indices.push_back(2);
        indices.push_back(ret.size()-1);
        return {ret,indices};
    }

	void GizmosRenderSystem::RenderTransformGizmo(const kuma::Transform& trans, RenderSystem& rendersystem) {
	    // Make the vertex buffer
        // X-axis arrow
        // Make an arrow
	    auto rightArrow = right_arrow();
	    kuma::Mesh my_mesh{rightArrow.first, rightArrow.second, kuma::Material::Create(getrespath()+"/shaders/default.glsl")};

        GLCall(glDisable(GL_CULL_FACE));
	    rendersystem.renderRef->DrawLined(my_mesh);
        GLCall(glEnable(GL_CULL_FACE));
	}

}
