#ifndef LIFE_ENGINE_DEBUGSYSTEM_HPP
#define LIFE_ENGINE_DEBUGSYSTEM_HPP
#include <ecs/ecs>
#include <kuma/kuma>

namespace life {
	struct DebugSystem : public ecs::System {
		kuma::Renderer* renderRef;
		DebugSystem (kuma::Renderer& renderer);
		void Update(const std::shared_ptr<ecs::BaseComponent>& components) override;
	};
}

#endif //LIFE_ENGINE_DEBUGSYSTEM_HPP
