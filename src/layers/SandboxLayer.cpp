#include "SandboxLayer.hpp"
#include <systems/rendering/RenderSystem.hpp>
#include <systems/rendering/ParticleSystem.hpp>

#include <layers/staging/ComponentStaging.hpp>
#include <layers/staging/LayerStaging.hpp>
#include <layers/staging/SystemStaging.hpp>

namespace life {

    struct SandboxLayer::Impl {
        std::shared_ptr<ecs::ECS> world;
        ecs::SystemList sandboxSystems;
        
        /// Initialize things in here
        explicit Impl(std::shared_ptr<ecs::ECS>& wref) : world(wref), sandboxSystems{} {
			/// -------- GAME SCENE -------- ///
        }
    };

    /// Update things in here
    void SandboxLayer::OnUpdate() {
        pimpl->world->UpdateSystems(pimpl->sandboxSystems);
    }

    SandboxLayer::SandboxLayer(std::shared_ptr<ecs::ECS>& world_reference)
            : Layer("SandboxLayer"),
              pimpl{std::make_unique<SandboxLayer::Impl>(world_reference)}
    {

    }
}
