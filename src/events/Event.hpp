#ifndef EVENT_HPP
#define EVENT_HPP
#include <string>
#include <life_core.h>
#include <functional>
#include <sstream>

namespace life {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        RenderResize,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum class EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };
    EventCategory operator|(EventCategory a, EventCategory b);

#define EVENT_CLASS_TYPE(x) static EventType GetStaticEventType() { return EventType::x; }\
        virtual EventType GetEventType() const override { return GetStaticEventType(); }\
        virtual const char* GetName() const override { return #x ; }
#define EVENT_CLASS_CATEGORY(x) virtual EventCategory GetEventCategory() const override { return x; }
#define EVENT_DEFAULT_TOSTRING(classname) std::string ToString() const { return #classname ; }

    class Event {
        friend class EventsDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual EventCategory GetEventCategory() const = 0;
        virtual const char* GetName() const = 0;
        virtual std::string ToString() const = 0;
        virtual void SetHandled() { m_handled = true; }
        virtual bool IsHandled() { return m_handled; }

        virtual bool IsInCategory(EventCategory category) {
            return static_cast<int>(GetEventCategory()) & static_cast<int>(category);
        }

    protected:
        bool m_handled = false;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }

    /* EVENTS DISPATCHER
     * 	This dispatches events to the assigned function
     * */
    class EventsDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventsDispatcher(Event& event)
                : m_event(event) {}

        template<typename T>
        bool dispatch(EventFn<T> func) {
            if(m_event.GetEventType() == T::GetStaticEventType()) {
                m_event.m_handled = func((T&) m_event);
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };
}
#endif //CLASSNAME_HPP
