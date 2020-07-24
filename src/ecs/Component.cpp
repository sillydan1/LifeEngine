#include "Component.hpp"

namespace life::ecs {
    component_id_t BaseComponent::current_comp_id=0;

    component_id_t BaseComponent::GetNewComponentID() noexcept  { return current_comp_id++; }
    const component_id_t life::ecs::BaseComponent::GetID() const noexcept  { return runtimeID; }

}

namespace life::ecs {
    bool operator==(const BaseComponent& a, const BaseComponent& b) {
        return std::hash<BaseComponent>{}(a) == std::hash<BaseComponent>{}(b);
    }
}