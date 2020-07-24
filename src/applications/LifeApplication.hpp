#ifndef LIFEAPPLICATION_HPP
#define LIFEAPPLICATION_HPP
#include "Application.hpp"
#include <ecs/ECS.hpp>
#include <layers/LayerStack.hpp>
#include <kuma/Camera.hpp>
#include <layers/EditorLayer.hpp>
#include <memory>

namespace life {

    class LifeApplication : public Application {
    public:
        LifeApplication();

        void Run() override;
        void OnEvent(Event& event) override;
        template<class L, typename... Args>
        void PushLayer(Args&&... params) {
			m_layerstack.PushLayer(new L(params...));
        }
        template<class L, typename... Args>
        inline void PushOverlay(Args&&... params) {
            m_layerstack.PushOverlay(new L(params...));
        }
        template<class L>
        inline void PushOverlay(L* l) {
            m_layerstack.PushOverlay(l);
        }
        template<class L>
        inline void PushLayer(L* l) {
			m_layerstack.PushLayer(l);
		}
    private:
        std::shared_ptr<ecs::ECS> m_ecs;
        EditorLayer* m_editorLayer;
        LayerStack m_layerstack;

        void InitOpenGL() const;
    };

}
#endif //CLASSNAME_HPP
