#include <util/Time.h>
#include "RenderSystem.hpp"
#include "layers/staging/ComponentStaging.hpp"
#include <components/components>

#include <algorithm>

namespace life {
	
	LifeCamera::LifeCamera() : camera{glm::vec3{0,1.5f,3}},
		projection{std::make_shared<glm::mat4>(glm::perspective(glm::radians(camera.Zoom), 
												(float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), 
												0.1f, 100.0f))},
		view{std::make_shared<glm::mat4>(camera.GetViewMatrix())},
		pureview{std::make_shared<glm::mat4>(glm::mat4(glm::mat3(camera.GetViewMatrix())))}
	{
		camera.Pitch = -30.0f;
		camera.updateCameraVectors();
	}
	
	void LifeCamera::Update() {
		*projection = glm::perspective(glm::radians(camera.Zoom), 
												(float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight(), 
												0.1f, 100.0f);
		*view = camera.GetViewMatrix();
		*pureview = glm::mat4(glm::mat3(*view));
	}
	
	void LifeCamera::Update(float x, float y) {
		float r = x/y;
		*projection = glm::perspective(glm::radians(camera.Zoom),
									   r,
									   0.1f, 100.0f);
		*view = camera.GetViewMatrix();
		*pureview = glm::mat4(glm::mat3(*view));
	}
	
	void LifeCamera::Bind() {
		kuma::Material::SetGlobalProperty("u_view", 		view);
		kuma::Material::SetGlobalProperty("u_pureview", 	pureview);
		kuma::Material::SetGlobalProperty("u_proj", 		projection);
		kuma::Material::SetGlobalProperty("u_viewPos", 	camera.Position);
		bound = true;
	}
	
	RenderSystem::RenderSystem(kuma::Renderer& renderer) : life::ecs::System(), renderRef{&renderer} {
		SetComponentFilterType(ecs::ComponentFilterType::OR);
		AddComponentType(ModelComponent::ID);
		AddComponentType(StaticModelComponent::ID);
		AddComponentType(StaticMeshComponent::ID);
		AddComponentType(WorldCube::ID);
		AddComponentType(ParticleEmitterComponent::ID);
		
		AddComponentType(GameCard::ID);
		FinalizeComponentAdding();
	}
	void RenderSystem::Update(const std::shared_ptr<ecs::BaseComponent>& component) {
		{ /// Model Components (Animated)
			auto d = std::dynamic_pointer_cast<ModelComponent>(component);
			if(d) {
				for (auto &m : d->model->GetMeshes()) {
					renderRef->Draw(m);
				}
				return;
			}
		}
        { /// Model Components (Not Animated)
            auto d = std::dynamic_pointer_cast<StaticModelComponent>(component);
            if(d) {
                for (auto &m : d->model->GetMeshes()) {
                    renderRef->Draw(m);
                }
            }
        }
		{ /// StaticMeshComponents
			auto d = std::dynamic_pointer_cast<StaticMeshComponent>(component);
			if (d) {
				renderRef->Draw(*d->mesh);
			}
		}
		{ /// WorldCube Components
			auto d = std::dynamic_pointer_cast<WorldCube>(component);
			if (d) {
				renderRef->Draw(d->mesh);
			}
		}
		{ /// ParticleEmitter Components
			auto d = std::dynamic_pointer_cast<ParticleEmitterComponent>(component);
			if(d) {
                renderRef->Draw(*d);
			}
		}
		{ /// Game Card Components
			auto d = std::dynamic_pointer_cast<GameCard>(component);
			if(d) {
				// Draw the mesh. It's really very simple.
				renderRef->Draw(*d->mesh);
			}
		}
	}

}
