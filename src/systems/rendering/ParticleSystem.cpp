#include "ParticleSystem.hpp"
#include <util/Time.h>
#include <util/Random.hpp>

namespace life {

	ParticleSystem::ParticleSystem()
	 : System()
	{
		AddComponentType(ParticleEmitterComponent::ID);
		FinalizeComponentAdding();
	}

	float accumulator = 0.0f;
	void ParticleSystem::Update(const std::shared_ptr<ecs::BaseComponent>& component) {
		auto emitter = std::dynamic_pointer_cast<ParticleEmitterComponent>(component);
		if(!emitter) return;
		auto& particles = emitter->particles; // shortcut
		auto dt = Time::GetFrameTime();
		// Spawn new particles
		float nr_new_particles = 5 * dt;
		accumulator += nr_new_particles;
		if(accumulator > 1.0f) {
			while(accumulator > 0) {
				int unusedParticle = FirstUnusedParticle(emitter);
				RespawnParticle(particles[unusedParticle]);
				accumulator--;
			}
			accumulator = 0;
		}
		
		// Update alive particles
		for (auto& p : particles) {
			p.life -= dt; // reduce life
			if (p.life > 0.0f)
			{	// particle is alive, thus update
				p.position  += p.velocity * dt;
				p.color.a   -= dt;
			}
		}
	}
	
	int ParticleSystem::FirstUnusedParticle(const std::shared_ptr<ParticleEmitterComponent>& emitter) {
		for (unsigned int i = emitter->particle_index_buffer; i < emitter->particles.size(); ++i) {
		  	if(emitter->particles[i].life <= 0.0) {
		  		emitter->particle_index_buffer = i;
				return i;
		  	}
		}
		for (unsigned int i = 0; i < emitter->particle_index_buffer; ++i) {
			if(emitter->particles[i].life <= 0.0) {
				emitter->particle_index_buffer = i;
				return i;
			}
		}
		return 0; // Override index zero.
	}
	
	void ParticleSystem::RespawnParticle(ParticleEmitterComponent::Particle &particle) {
		/// This is the reusing of a particle.
		/// This is where you define the spawn behaviour of the particle
		// TODO: Make this injectable
		particle.position = {0,0,0};
		particle.velocity = {Random::GenerateFloat(-0.2f,0.2f), Random::GenerateFloat(0.2f,2.f), Random::GenerateFloat(-0.2f,0.2f)};
		particle.life = 1.0f;
		particle.color = {Random::GenerateVec3(0.0f,1.0f),1.f};
	}
	
}
