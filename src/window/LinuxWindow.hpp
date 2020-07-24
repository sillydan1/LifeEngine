#ifndef LINUX_WINDOW_HPP
#define LINUX_WINDOW_HPP
#include "Window.hpp"

#include <GLFW/glfw3.h>

namespace life {

    class LinuxWindow : public Window {
    public:
        LinuxWindow(const WindowProperties& properties);
        virtual ~LinuxWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_data.width; }
        inline unsigned int GetHeight() const override { return m_data.height; }
        inline void SetWidth(unsigned int v) override { m_data.width = v; }
        inline void SetHeight(unsigned int v) override { m_data.height = v; }
		inline void SetDimensions(unsigned int w, unsigned int h) override { SetWidth(w); SetHeight(h); }
        
        inline void SetEventCallback(const EventCallbackFunc& callback) override {
            m_data.eventcallback = callback;
        }
        void SetVSync(bool enable = true) override;
        bool IsVSync() const override;
        inline void* const GetAPIWindow() override { return m_window; }
    private:
        virtual void setupCallbacks();
        virtual void init(const WindowProperties& properties);
        virtual void shutdown();

        GLFWwindow* m_window;

        struct WindowData : public WindowProperties {
            // std::string& Title;
            // unsigned int width, height;
            EventCallbackFunc eventcallback;
            bool vsync;

            WindowData()
                    : WindowProperties(), eventcallback(nullptr), vsync(false) {}
        } m_data;
    };

}
#endif //CLASSNAME_HPP
