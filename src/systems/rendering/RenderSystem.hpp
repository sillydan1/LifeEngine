#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include <ecs/ecs>
#include <kuma/kuma>
#include <applications/Application.hpp>

namespace life {
	
	// Globally available world things 
	// TODO: These have to be moved somewhere else that makes more sense.
	struct LifeCamera {
		kuma::Camera camera;
		LifeCamera();
		void Update();
		void Update(float x, float y);
		void Bind();
		inline void Unbind() { bound = false; }

		TYPE_PTR(glm::mat4) projection, view, pureview;
		bool bound;
	};
	// TODO: <Orthographic Game Camera>

	struct RenderSystem : public ecs::System {
		kuma::Renderer* renderRef;
		
		RenderSystem(kuma::Renderer& renderer);
		virtual void Update(const std::shared_ptr<ecs::BaseComponent>& components) override;
	};

 
}
#endif //RENDERSYSTEM_HPP
