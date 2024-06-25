#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "events/Event.hpp"

struct WindowProperties {
    std::string title;
    unsigned int width, height;
    explicit WindowProperties(std::string  title = "Life Engine", unsigned int width = 900, unsigned int height = 600)
            : title(std::move(title)), width(width), height(height) {}
};

using EventCallbackFunc = std::function<void(event&)>;

class Window {
public:
    virtual ~Window() = default;
    virtual void OnUpdate() = 0;
    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;
    virtual void SetWidth(unsigned int v) = 0;
    virtual void SetHeight(unsigned int v) = 0;
    virtual void SetDimensions(unsigned int w, unsigned int h) = 0;
    virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
    virtual void SetVSync(bool enable) = 0;
    virtual bool IsVSyncEnabled() const = 0;
    virtual void* GetAPIWindow() = 0;
    static Window* Create(const WindowProperties& properties = WindowProperties());
};

#endif // WINDOW_HPP
