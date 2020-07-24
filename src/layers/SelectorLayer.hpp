#ifndef LIFE_ENGINE_SELECTORLAYER_HPP
#define LIFE_ENGINE_SELECTORLAYER_HPP
#include "layers"
#include <memory>

namespace life::editor {
	
	class SelectorLayer : public Layer {
	public:
		SelectorLayer(std::shared_ptr<ecs::ECS>& world_reference);
		
		void OnEvent(Event& event) override; // For Mouse Click Events
		void OnUpdate() override;
		void OnAttach() override;
		void OnDetach() override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_pimpl; // Simple PIMPL idiom
	};
	
}

#endif //LIFE_ENGINE_SELECTORLAYER_HPP
