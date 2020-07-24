#ifndef RENDERLAYER_HPP
#define RENDERLAYER_HPP
#include "Layer.hpp"
#include <memory>
#include <ecs/ecs>

namespace life {
	class RenderLayer : public Layer {
	public:
		RenderLayer(std::shared_ptr<ecs::ECS>& world_reference, bool is_editor_pipeline = false, bool is_main_camera = false);
		
		void OnEvent(Event& event) override;
        void OnUpdate() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;
    private:
		struct Impl;
		std::unique_ptr<Impl> pimpl; // Simple PIMPL idiom
	};

}
#endif //CLASSNAME_HPP
