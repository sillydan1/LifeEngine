#ifndef LIFE_ENGINE_MATERIALCOMPONENT_HPP
#define LIFE_ENGINE_MATERIALCOMPONENT_HPP
#include <kuma/kuma_math_types.hpp>
#include <ecs/ecs>
#include <kuma/kuma>
#include <util/FileSystem.hpp>

namespace life {
	
	struct MaterialComponent : public ecs::Component<MaterialComponent> {
		TYPE_PTR(kuma::Material) material;
		MaterialComponent() : Component(), material{kuma::Material::Create(getrespath()+"/shaders/default.glsl")} {}
		MaterialComponent(const std::string& shader) : Component(), material{kuma::Material::Create(shader)} {}
	};
	
}

#endif //LIFE_ENGINE_MATERIALCOMPONENT_HPP
