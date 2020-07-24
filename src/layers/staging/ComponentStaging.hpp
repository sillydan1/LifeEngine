#ifndef LIFE_ENGINE_COMPONENTSTAGING_HPP
#define LIFE_ENGINE_COMPONENTSTAGING_HPP
/**
 * This file is for staging development of various ECS components
 * No systems in here dude. Also, when a component is "done" you should make a dedicated file for it.
 * */
/* TEMPLATE
 struct MyComponent : public ecs::Component<MyComponent> {
    // TYPE_PTRs
    MyComponent() {}
 };
 */

#include <kuma/kuma_math_types.hpp>
#include <ecs/ecs>
#include <kuma/kuma>

#include <bullet/btBulletDynamicsCommon.h>

namespace life {
	struct WorldCube : public ecs::Component<WorldCube> {
		TYPE_PTR(kuma::Transform) transform;
		kuma::StaticMesh mesh;
		TYPE_PTR(kuma::Texture) texture;
		
		WorldCube();
		WorldCube(glm::vec3 position);
	};
	
	struct GameCard : public ecs::Component<GameCard> {
		std::string cardname;
		TYPE_PTR(kuma::Transform) transform;
		TYPE_PTR(kuma::StaticMesh) mesh;
		TYPE_PTR(kuma::Texture) card_texture;
		
		GameCard();
		GameCard(glm::vec3 start_pos);
	};
	
	struct BulletRigidBodyComponent : public ecs::Component<BulletRigidBodyComponent> {
		btRigidBody* rigibody;
		TYPE_PTR(kuma::Transform) transform;
		BulletRigidBodyComponent() = delete;
		explicit BulletRigidBodyComponent(btRigidBody* r, TYPE_PTR(kuma::Transform) t) : Component(), rigibody(r), transform(std::move(t)) {
			r->setUserPointer(this);
		}
	};
	
	/// Particle Emitter
	/// A thing that emits particles. Use together with a
	/// ParticleSystem system :-)
	struct ParticleEmitterComponent : public ecs::Component<ParticleEmitterComponent> {
		struct Particle {
			glm::vec3 position, velocity;
			glm::vec4 color;
			float life, distancetocamera;
			int operator<(Particle& other) {
				return distancetocamera > other.distancetocamera;
			}
			int operator>(Particle& other) {
				return distancetocamera < other.distancetocamera;
			}
			Particle() : position{0}, velocity{0}, color{1.0f}, life{0} {}
		};
		TYPE_PTR(kuma::StaticMesh) mesh; // Should be static, since it can be shared between all ParticleEmitterComponents
		TYPE_PTR(kuma::Transform) transform; // emitter transformation.
		unsigned int particle_amount;
		std::vector<Particle> particles;
		unsigned int particle_index_buffer;
		
		ParticleEmitterComponent() = delete;
		explicit ParticleEmitterComponent(const TYPE_PTR(kuma::Material)& mat, TYPE_PTR(kuma::Transform) t, unsigned int max_particles = 1000)
		 :  mesh{kuma::StaticMesh::Create(
				kuma::default_shapes::quad,
				kuma::default_shapes::quad_indices,
				mat)},
 	 		transform{std::move(t)},
 	 		particle_amount{max_particles},
 	 		particles{particle_amount},
 	 		particle_index_buffer{0}
 	 	{
 	 		// TODO: Have a default_resource for particles
			mesh->GetMaterial().SetProperty("u_sprite", kuma::default_resources::get_sprite());
			mesh->GetMaterial().SetProperty("u_model", transform);
		}
	};
}

#endif //LIFE_ENGINE_COMPONENTSTAGING_HPP
