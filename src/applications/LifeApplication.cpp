#include <GL/glew.h>
#include "LifeApplication.hpp"
#include <layers/layers>
#include <util/Time.h>
#include <util/DebugInfo.h>
#include <layers/SandboxLayer.hpp>
#include <layers/SelectorLayer.hpp>

#include <layers/staging/LayerStaging.hpp>

namespace life {

    LifeApplication::LifeApplication() : Application(), m_layerstack() {
		m_ecs = std::make_shared<ecs::ECS>();
		InitOpenGL();
		Random::Init();
        m_editorLayer = new EditorLayer(m_ecs); // EditorLayer is the Editor logic
        
        PushOverlay(m_editorLayer);
        PushOverlay<editor::SelectorLayer>(m_ecs);
        PushLayer<RenderLayer>(m_ecs, true); // Editor viewport renderlayer
        PushLayer<RenderLayer>(m_ecs, false, true); // Game viewport renderlayer

        PushLayer<SandboxLayer>(m_ecs);
        PushLayer<PhysicsLayer>(m_ecs);
    }

    void LifeApplication::Run() {
        Time::GameStart();
        while(m_running) {

            for(auto layer : m_layerstack)
                layer->OnUpdate();

            m_editorLayer->Begin();
            for(auto layer : m_layerstack)
                layer->OnImGuiRender();
            m_editorLayer->End();

            Time::FrameEnd();
            m_window->OnUpdate();
        }
    }

    void LifeApplication::OnEvent(Event& event) {
        Application::OnEvent(event);
        if(!event.IsHandled()) {
			for (auto layer : m_layerstack) {
				layer->OnEvent(event);
				if(event.IsHandled()) break;
			}
		}
    }

    void LifeApplication::InitOpenGL() const {
        // TODO: This GLEW initialization should be somewhere else.!
        GLenum glewStatus = glewInit(); // initialize GLEW
        if(glewStatus != GLEW_OK) {
            LIFE_ERR("Glew Initialization error: %d", glewStatus);
        }
        auto s = glGetString(GL_VERSION);
        LIFE_LOG("OpenGL Version: %s ", s);
        DebugInfo::RenderInformation = "OpenGL " + std::string(reinterpret_cast<const char*>(s));
        /* GL_ENABLE THINGS */
        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        // glDepthFunc(GL_ALWAYS);
        // glDepthMask(GL_FALSE);
        // glDepthFunc(GL_LESS);
        // glEnable(GL_STENCIL_TEST);
        // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        //glEnable(GL_FRAMEBUFFER_SRGB);
	
        // Load the resources that are default to the graphics engine
		life::kuma::default_resources::load_default_resources();
    }
}
