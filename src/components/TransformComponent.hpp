#ifndef LIFE_ENGINE_TRANSFORMCOMPONENT_HPP
#define LIFE_ENGINE_TRANSFORMCOMPONENT_HPP
#include <kuma/kuma_math_types.hpp>
#include <ecs/ecs>
#include <kuma/kuma>

namespace life {
	struct TransformComponent : public ecs::Component<TransformComponent> {
		TYPE_PTR(kuma::Transform) transform;
		TransformComponent(glm::vec3 p = glm::vec3(0)) : Component(), transform{std::make_shared<kuma::Transform>(p)} {}
		TransformComponent(const kuma::Transform& t) : Component(), transform{std::make_shared<kuma::Transform>(t)} {}
		TransformComponent(const TYPE_PTR(kuma::Transform)& t) : Component(), transform{t} {}
		TransformComponent(TYPE_PTR(kuma::Transform)&& t) : Component(), transform{std::move(t)} {}
	};
}

#endif //LIFE_ENGINE_TRANSFORMCOMPONENT_HPP
