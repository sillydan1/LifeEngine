#ifndef LINUX_WINDOW_HPP
#define LINUX_WINDOW_HPP
#include "Window.hpp"
#include <GLFW/glfw3.h>

struct WindowData : public WindowProperties {
    EventCallbackFunc eventcallback;
    bool vsync;
    WindowData() : WindowProperties(), eventcallback(nullptr), vsync(false) {}
};

class LinuxWindow : public Window {
public:
    explicit LinuxWindow(const WindowProperties& properties);
    ~LinuxWindow() override;
    void OnUpdate() override;
    [[nodiscard]] inline unsigned int GetWidth() const override { return m_data.width; }
    [[nodiscard]] inline unsigned int GetHeight() const override { return m_data.height; }
    inline void SetWidth(unsigned int v) override { m_data.width = v; }
    inline void SetHeight(unsigned int v) override { m_data.height = v; }
    inline void SetDimensions(unsigned int w, unsigned int h) override { SetWidth(w); SetHeight(h); }
    inline void SetEventCallback(const EventCallbackFunc& callback) override { m_data.eventcallback = callback; }
    void SetVSync(bool enable) override;
    [[nodiscard]] bool IsVSyncEnabled() const override;
    inline void* GetAPIWindow() override { return m_window; }
private:
    void SetupWindowCallbacks();
    void InitializeGLFW3Window(const WindowProperties& properties);
    void Shutdown();
    void SetWindowProperties(const WindowProperties& properties);
    static void InitializeGLFW3API();
    void ConfigureGLFW3ToCurrentWindow();

    GLFWwindow* m_window;
    WindowData m_data;
};

#endif //LINUX_WINDOW_HPP
