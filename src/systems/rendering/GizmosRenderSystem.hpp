#ifndef LIFE_ENGINE_GIZMOSRENDERSYSTEM_HPP
#define LIFE_ENGINE_GIZMOSRENDERSYSTEM_HPP

#include <layers/LayerStack.hpp>
#include "RenderSystem.hpp"

namespace life {

	struct GizmosRenderSystem : ecs::System {
		kuma::Renderer* renderRef;
		explicit GizmosRenderSystem(kuma::Renderer& renderer);
		void RenderSelectedObject(LayerStack* owner, RenderSystem& rendersystem);
		void Update(const std::shared_ptr<ecs::BaseComponent>& components) override;
		void RenderTransformGizmo(const kuma::Transform& trans, RenderSystem& rendersystem);
	};
	
}
#endif //LIFE_ENGINE_GIZMOSRENDERSYSTEM_HPP
