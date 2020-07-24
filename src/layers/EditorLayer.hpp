#ifndef EDITORLAYER_HPP
#define EDITORLAYER_HPP
#include "Layer.hpp"
#include <ecs/ECS.hpp>

namespace life {

    class EditorLayer : public Layer {
    public:
        EditorLayer(std::shared_ptr<ecs::ECS>& world_reference);

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

        void Begin();
        void End();

    private:
		void DrawDockspaceAndMenuBar(bool *p_open);
		void DeleteSelectedObject();
		void DrawDebugConsole();
    	
    	struct Impl;
    	std::unique_ptr<Impl> m_pimpl;
    };

}

#endif //EDITORLAYER_HPP
