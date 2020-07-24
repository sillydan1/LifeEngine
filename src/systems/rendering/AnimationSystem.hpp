#ifndef LIFE_ENGINE_ANIMATIONSYSTEM_HPP
#define LIFE_ENGINE_ANIMATIONSYSTEM_HPP
#include <ecs/ecs>
#include <kuma/kuma>

namespace life {

    struct AnimationSystem : public ecs::System {
        AnimationSystem();
        virtual void Update(const std::shared_ptr<ecs::BaseComponent>& components) override;
    };

}

#endif //LIFE_ENGINE_ANIMATIONSYSTEM_HPP
