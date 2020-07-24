#ifndef LIFE_ENGINE_STATICMESHCOMPONENT_HPP
#define LIFE_ENGINE_STATICMESHCOMPONENT_HPP
#include <kuma/kuma_math_types.hpp>
#include <ecs/ecs>
#include <kuma/kuma>
#include <util/Random.hpp>
#include <utility>
#include <kuma/LifeSelectable.hpp>

namespace life {
    struct StaticMeshComponent : public ecs::Component<StaticMeshComponent>, kuma::LifeSelectable {
		TYPE_PTR(kuma::StaticMesh) mesh;

		StaticMeshComponent(const TYPE_PTR(kuma::Material)& mat, TYPE_PTR(kuma::Transform) transform)
		: Component(), LifeSelectable(),
		mesh{kuma::StaticMesh::Create(kuma::default_shapes::box, kuma::default_shapes::box_indices, mat)}
		{
            Initialize(mat, std::move(transform), kuma::default_resources::get_diffuse());
		}
		StaticMeshComponent(const TYPE_PTR(kuma::Material)& mat, TYPE_PTR(kuma::Transform) transform, const std::string& texture)
		: Component(),LifeSelectable(),
		mesh{kuma::StaticMesh::Create(kuma::default_shapes::box, kuma::default_shapes::box_indices, mat)}
		{
            Initialize(mat, std::move(transform), kuma::Texture::Create(texture));
		}
        StaticMeshComponent(const TYPE_PTR(kuma::Material)& mat, TYPE_PTR(kuma::Transform) transform, TYPE_PTR(kuma::StaticMesh) m)
                : Component(),LifeSelectable(),
                  mesh{std::move(m)}
        {
            Initialize(mat, std::move(transform), kuma::default_resources::get_diffuse());
        }
        StaticMeshComponent(const TYPE_PTR(kuma::Material)& mat, TYPE_PTR(kuma::Transform) transform, const std::string& texture, TYPE_PTR(kuma::StaticMesh) m)
                : Component(),LifeSelectable(),
                  mesh{std::move(m)}
        {
            Initialize(mat, std::move(transform), kuma::Texture::Create(texture));
        }
	private:
	    void Initialize(const TYPE_PTR(kuma::Material)& mat, TYPE_PTR(kuma::Transform) transform, const TYPE_PTR(kuma::Texture)& texture) {
            mesh->GetMaterial().SetProperty("u_model", 			   transform);
            mesh->GetMaterial().SetProperty("u_material.diffuse",    texture);
            mesh->GetMaterial().SetProperty("u_material.shininess",  32.0f);
            mesh->GetMaterial().SetProperty("u_selectorcolor",     selectorcolor);
		}
	};
}

#endif //LIFE_ENGINE_STATICMESHCOMPONENT_HPP
