#ifndef LIFE_ENGINE_SYSTEMSTAGING_HPP
#define LIFE_ENGINE_SYSTEMSTAGING_HPP
/**
 * This file is for staging development of ECS Systems.
 * No components here. Also, when a system is "done" you should make a dedicated file for it.
 * */
#include <ecs/ecs>
#include "ComponentStaging.hpp"

/*
 * TEMPLATE
struct MySystem : public ecs::System {
    MySystem();
    virtual void Update(const std::shared_ptr<ecs::BaseComponent>& components) override;
};
 * */

namespace life {

	/// PhysicsSyncSystem
	/// - Synchronizes entities with BulletRigidBodyComponent
	/// to the kuma::Transform associated with it.
    struct PhysicsSyncSystem : public ecs::System {
        PhysicsSyncSystem()
         : System()
        {
        	AddComponentType(BulletRigidBodyComponent::ID);
        	FinalizeComponentAdding();
        }
        
        void Update(const std::shared_ptr<ecs::BaseComponent>& component) override {
			auto comp = std::dynamic_pointer_cast<BulletRigidBodyComponent>(component);
			if(comp) {
				auto wtrans = comp->rigibody->getWorldTransform();
				vec3_assign(comp->transform->position,    wtrans.getOrigin());
				quat_assign(comp->transform->orientation, wtrans.getRotation());
				comp->transform->UpdateModelMatrix();
			} else {
				LIFE_WARN("Component Filtering went haywire. Something might be very wrong in the ECS system");
			}
        }
        
		void vec3_assign(glm::vec3& a, const btVector3& b) {
			a.x = b.getX() * 0.5f; // TODO: Either expand the engine to use 64-bit floats or get Bullet physics to use 32-bit floats
			a.y = b.getY() * 0.5f;
			a.z = b.getZ() * 0.5f;
		}
		
		void quat_assign(glm::quat& a, const btQuaternion& b) {
			a.x = b.getX();
			a.y = b.getY();
			a.z = b.getZ();
			a.w = b.getW();
		}
    };

}

#endif //LIFE_ENGINE_SYSTEMSTAGING_HPP
