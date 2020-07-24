#ifndef LIFE_ENGINE_SANDBOXLAYER_HPP
#define LIFE_ENGINE_SANDBOXLAYER_HPP
#include "Layer.hpp"
#include <ecs/ecs>

namespace life {
    class SandboxLayer : public Layer {
    public:
        SandboxLayer(std::shared_ptr<ecs::ECS>& world_reference);

        void OnUpdate() override;
    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl; // Simple PIMPL idiom
    };
}

#endif //LIFE_ENGINE_SANDBOXLAYER_HPP
