#include <GL/glew.h>
#include "Application.hpp"
#include <assert.h>
#include <util/FileSystem.hpp>

namespace life {

#define LIFE_FN_BIND(x) std::bind(&x, this, std::placeholders::_1)

    static Application* instance = nullptr;

    Application& Application::Get() { return *instance; }

    Application::Application() {
        assert(instance == nullptr);
        instance = this;
        m_window = std::unique_ptr<Window>(Window::Create(WindowProperties::FromConfigFile(getexepath()+"/res/conf/base.conf")));
        m_window->SetEventCallback(LIFE_FN_BIND(Application::OnEvent));
    }

    Application::~Application() {

    }

    void Application::OnEvent(Event& event) {
        if(event.GetEventType() == EventType::WindowClose) {
            m_running = false;
        }
    }

    void Application::Run() {

        GLenum glewStatus = glewInit(); // initialize GLEW
        if(glewStatus != GLEW_OK) {
            LIFE_ERR("Glew Initialization error: %d", glewStatus);
        }
        LIFE_LOG("OpenGL Version: %s ", glGetString(GL_VERSION));
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

        while(m_running) {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_window->OnUpdate();
        }
    }
}

