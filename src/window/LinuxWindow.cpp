#include "LinuxWindow.hpp"

#include <events/ApplicationEvents.hpp>
#include <events/KeyEvents.hpp>
#include <events/MouseEvents.hpp>

namespace life {

    static bool glfw_initialized = false;

    static void GLFWErrorCallback(int error, const char* desc) {
        LIFE_ERR("Error [%d] %s", error, desc);
    }

    Window* Window::Create(const WindowProperties& properties) {
        return new LinuxWindow(properties);
    }

    LinuxWindow::LinuxWindow(const WindowProperties& properties) {
        init(properties);
    }

    LinuxWindow::~LinuxWindow() {
        shutdown();
    }

    void LinuxWindow::init(const WindowProperties& properties) {
        // Property setup
        m_data.Title = properties.Title;
        m_data.width = properties.width;
        m_data.height = properties.height;
        LIFE_LOG("Creating window '%s' (%d, %d)", m_data.Title.c_str(), m_data.width, m_data.height);
        // Initialize the GLFW3 API
        if(!glfw_initialized) {
            const int success = glfwInit();
            glfwSetErrorCallback(GLFWErrorCallback);
            LIFE_ASSERT(success);
            glfw_initialized = true;
        }
        // Create the window
        // Note: This is not doing anything fancy with full-screen or other configuration loading.
        m_window = glfwCreateWindow((int) m_data.width, (int) m_data.height, m_data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, &m_data);
        SetVSync(true);
        // Set up the Window event callbacks
        setupCallbacks();
    }

    void LinuxWindow::setupCallbacks() {

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* wi, int w, int h) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
            WindowResizeEvent event(w, h);
            data.width = w;
            data.height = h;
            data.eventcallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* wi) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
            WindowCloseEvent event;
            data.eventcallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* wi, int key, int scancode, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent e(key, 0);
                    data.eventcallback(e);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent e(key);
                    data.eventcallback(e);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent e(key, 1);
                    data.eventcallback(e);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* wi, int button, int action, int mods) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
            double x,y; glfwGetCursorPos(wi, &x, &y);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent e(button, static_cast<float>(x), static_cast<float>(y));
                    data.eventcallback(e);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent e(button, static_cast<float>(x), static_cast<float>(y));
                    data.eventcallback(e);
                    break;
                }
				default:
					LIFE_WARN("Mouse button callback event not handled. Apparently there's a third thing you can do with a button");
					break;
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* wi, double x_offset, double y_offset) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
            MouseScrolledEvent e((float) x_offset, (float) y_offset);
            data.eventcallback(e);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* wi, double x_pos, double y_pos) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
            MouseMovedEvent e((float) x_pos, (float) y_pos);
            data.eventcallback(e);
        });
    }

    void LinuxWindow::shutdown() {
        glfwDestroyWindow(m_window);
    }

    void LinuxWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    void LinuxWindow::SetVSync(bool enable) {
        if(enable)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_data.vsync = enable;
    }

    bool LinuxWindow::IsVSync() const {
        return m_data.vsync;
    }

}

