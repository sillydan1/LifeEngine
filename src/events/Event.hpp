#ifndef EVENT_HPP
#define EVENT_HPP
#include <lifepch.h>

enum class event_type {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    RenderResize,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum class event_category {
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryMouseButton = BIT(4)
};
event_category operator|(event_category a, event_category b);

#define EVENT_CLASS_TYPE(x) static event_type get_static_event_type() { return event_type::x; }\
    virtual event_type get_event_type() const override { return get_static_event_type(); }\
    virtual const char* get_name() const override { return #x ; }
#define EVENT_CLASS_CATEGORY(x) virtual event_category get_event_category() const override { return x; }
#define EVENT_DEFAULT_TOSTRING(classname) std::string to_string() const { return #classname ; }

class event {
    friend class events_dispatcher;
public:
    virtual event_type get_event_type() const = 0;
    virtual event_category get_event_category() const = 0;
    virtual const char* get_name() const = 0;
    virtual std::string to_string() const = 0;
    virtual void set_handled() { handled = true; }
    virtual bool is_handled() { return handled; }

    virtual bool is_in_category(event_category category) {
        return static_cast<int>(get_event_category()) & static_cast<int>(category);
    }

protected:
    bool handled = false;
};

inline std::ostream& operator<<(std::ostream& os, const event& e) {
    return os << e.to_string();
}

class events_dispatcher {
    template<typename T>
    using eventfn = std::function<bool(T&)>;
public:
    events_dispatcher(event& event)
            : m_event(event) {}

    template<typename T>
    bool dispatch(eventfn<T> func) {
        if(m_event.get_event_type() == T::get_static_event_type()) {
            m_event.handled = func((T&) m_event);
            return true;
        }
        return false;
    }

private:
    event& m_event;
};

#endif //CLASSNAME_HPP
