#ifndef LIFE_ENGINE_PARTICLESYSTEM_HPP
#define LIFE_ENGINE_PARTICLESYSTEM_HPP
#include <ecs/ecs>
#include <kuma/kuma>
#include <kuma/openglmath>
#include <layers/staging/ComponentStaging.hpp>

namespace life {
	/// Default Basic particle system
	/// Version 1.0 - No optimizations
	///|----------------|---------------------------------------|
	///|Type: 			|CPU Particles (Billboarded quads)		|
	///|Primary use:	|Just getting some simple particles		|
	///|Physics:		|No										|
	///|VectorFields: 	|No										|
	///|Instanced:		|Yes (WIP)                              |
	///|----------------|---------------------------------------|
	struct ParticleSystem : ecs::System {
		ParticleSystem();
		int FirstUnusedParticle(const std::shared_ptr<ParticleEmitterComponent>& emitter);
		void RespawnParticle(ParticleEmitterComponent::Particle& particle);
		void Update(const std::shared_ptr<ecs::BaseComponent>& components) override;
	};
	
}

#endif //LIFE_ENGINE_PARTICLESYSTEM_HPP
