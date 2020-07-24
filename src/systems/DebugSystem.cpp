#include <layers/staging/ComponentStaging.hpp> // WorldCube
#include "DebugSystem.hpp"

namespace life {
	
	DebugSystem::DebugSystem(kuma::Renderer& renderer) : life::ecs::System(), renderRef{&renderer} {
		AddComponentType(WorldCube::ID);
		FinalizeComponentAdding();
	}
	void DebugSystem::Update(const std::shared_ptr<life::ecs::BaseComponent> &component) {
		renderRef->Draw(std::dynamic_pointer_cast<WorldCube>(component)->mesh);
		/*auto& trans = std::dynamic_pointer_cast<WorldCube>(components)->transform;
		auto& position = trans->position;
		trans->Translate(glm::vec3(0,glm::sin(life::Time::GetGlobalTime()+position.x+position.z*1.5f)*0.01f,0));
		trans->RotateAroundAxis(trans->GetUp(), life::Time::GetFrameTime());*/
	}

}
