#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <window/Window.hpp>

namespace life {

    class Application {
    public:
        Application();
        virtual ~Application();

        virtual void Run();
        virtual void OnEvent(Event& event);

        static Application& Get();
        inline Window& GetWindow() const { return *m_window; }
    protected:
        std::unique_ptr<Window> m_window;
        bool m_running = true;

    };

}
#endif //CLASSNAME_HPP
