#include "LinuxWindow.hpp"
#include "events/ApplicationEvents.hpp"
#include "events/KeyEvents.hpp"
#include "events/MouseEvents.hpp"

bool glfw_initialized = false;

static void GLFWErrorCallback(int error, const char* desc) {
    spdlog::error("Error [%d] %s", error, desc);
}

Window* Window::Create(const WindowProperties& properties) {
    return new LinuxWindow(properties);
}

LinuxWindow::LinuxWindow(const WindowProperties& properties) {
    InitializeGLFW3Window(properties);
}

LinuxWindow::~LinuxWindow() {
    Shutdown();
}

void LinuxWindow::InitializeGLFW3Window(const WindowProperties& properties) {
    SetWindowProperties(properties);
    spdlog::trace("Creating window '{0}' ({1}, {2})", m_data.title.c_str(), m_data.width, m_data.height);
    InitializeGLFW3API();
    m_window = glfwCreateWindow((int) m_data.width, (int) m_data.height, m_data.title.c_str(), nullptr, nullptr);
    ConfigureGLFW3ToCurrentWindow();
    SetVSync(true);
    SetupWindowCallbacks();
}

void LinuxWindow::ConfigureGLFW3ToCurrentWindow() {
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, &m_data);
}

void LinuxWindow::SetWindowProperties(const WindowProperties& properties) {
    m_data.title = properties.title;
    m_data.width = properties.width;
    m_data.height = properties.height;
}

void LinuxWindow::InitializeGLFW3API() {
    if(!glfw_initialized) {
        const int success = glfwInit();
        glfwSetErrorCallback(GLFWErrorCallback);
#ifdef __APPLE__
        /* We need to explicitly ask for a 3.2 context on OS X */
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        assert(success);
        glfw_initialized = true;
    }
}

void LinuxWindow::SetupWindowCallbacks() {

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* wi, int w, int h) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
        window_resize_event event(w, h);
        data.width = w;
        data.height = h;
        data.eventcallback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* wi) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
        window_close_event event;
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
            default:
                spdlog::critical("Keyboard button not handled.");
                break;
        }
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* wi, int button, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
        double x,y; glfwGetCursorPos(wi, &x, &y);
        switch (action) {
            case GLFW_PRESS: {
                mouse_button_pressed_event e(button, static_cast<float>(x), static_cast<float>(y));
                data.eventcallback(e);
                break;
            }
            case GLFW_RELEASE: {
                mouse_button_released_event e(button, static_cast<float>(x), static_cast<float>(y));
                data.eventcallback(e);
                break;
            }
            default:
                spdlog::critical("Mouse button callback event not handled.");
                break;
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* wi, double x_offset, double y_offset) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
        mouse_scrolled_event e((float) x_offset, (float) y_offset);
        data.eventcallback(e);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* wi, double x_pos, double y_pos) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(wi));
        mouse_moved_event e((float) x_pos, (float) y_pos);
        data.eventcallback(e);
    });
}

void LinuxWindow::Shutdown() {
    glfwDestroyWindow(m_window);
}

void LinuxWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void LinuxWindow::SetVSync(bool enable) {
    glfwSwapInterval(enable ? 1 : 0);
    m_data.vsync = enable;
}

bool LinuxWindow::IsVSyncEnabled() const {
    return m_data.vsync;
}

