#ifndef LIFE_ENGINE_MODELCOMPONENT_HPP
#define LIFE_ENGINE_MODELCOMPONENT_HPP
#include <kuma/kuma_math_types.hpp>
#include <ecs/ecs>
#include <kuma/kuma>
#include <kuma/LifeSelectable.hpp>
#include <util/FileSystem.hpp>

namespace life {
    struct ModelComponent : public ecs::Component<ModelComponent>, kuma::LifeSelectable {
		TYPE_PTR(kuma::AnimatedModel) model;
		
		ModelComponent() : Component(), LifeSelectable(),
						   model{std::make_shared<kuma::AnimatedModel>(getrespath()+"/models/boblamp/boblampclean.md5mesh")} {}
		ModelComponent(const std::string& model_filename) : Component(),
															model{std::make_shared<kuma::AnimatedModel>(model_filename)}
		{
		    for(auto& a : model->GetMeshes()) {
		        a.material->SetProperty("u_selectorcolor", selectorcolor);
		        a.material->SetProperty("u_material.diffuse", kuma::default_resources::get_diffuse());
		    }
		}
	};

    struct StaticModelComponent : public ecs::Component<StaticModelComponent>, kuma::LifeSelectable {
        TYPE_PTR(kuma::Model) model;
        StaticModelComponent() : Component(), LifeSelectable(),
                                 model{std::make_shared<kuma::Model>(getrespath()+"/models/boblamp/boblampclean.md5mesh")} {}
        StaticModelComponent(const std::string& model_filename) : Component(),
                                                            model{std::make_shared<kuma::Model>(model_filename)}
        {
            for(auto& a : model->GetMeshes()) {
                a.material->SetProperty("u_selectorcolor", selectorcolor);
				a.material->SetProperty("u_material.diffuse", kuma::default_resources::get_diffuse());
            }
        }
    };
}

#endif //LIFE_ENGINE_MODELCOMPONENT_HPP
