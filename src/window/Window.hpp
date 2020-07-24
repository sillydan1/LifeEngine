#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <functional>
#include <string>
#include <events/Event.hpp>
#include <config/life_config>
#include <utility>

namespace life {

    struct WindowProperties {
        std::string Title;
        unsigned int width, height;

        explicit WindowProperties(std::string  title = "Life Engine", unsigned int width = 900, unsigned int height = 600)
                : Title(std::move(title)), width(width), height(height) {}

        static WindowProperties FromConfigFile(const std::string& config_file_path) {
            char wtitle[50];
            memset(wtitle, 0, 30);
            LIFE_CONF_GET(config_file_path)("windowtitle", wtitle);
            if(strlen(wtitle) <= 0)
                strcpy(wtitle, "Life Engine");
            unsigned int w = 0, h = 0;
            LIFE_CONF_GET(config_file_path)("windowdimentions", w, h);
            if(w == 0 || h == 0) {
                w = 900; h = 600;
            }
            return WindowProperties(wtitle, w, h);
        }
    };

    /* Basicaly just an interface of how windows should be handled */
    class Window {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;
		
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;
        virtual void SetWidth(unsigned int v) = 0;
        virtual void SetHeight(unsigned int v) = 0;
		virtual void SetDimensions(unsigned int w, unsigned int h) = 0;
        
        virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
        virtual void SetVSync(bool enable = true) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* const GetAPIWindow() = 0;
        static Window* Create(const WindowProperties& properties = WindowProperties());
    };

}

#endif // WINDOW_HPP
