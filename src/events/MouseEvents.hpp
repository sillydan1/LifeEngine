#ifndef MOUSE_EVENTS_HPP
#define MOUSE_EVENTS_HPP
#include "Event.hpp"

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float mouse_x, float mouse_y)
            : m_mousex(mouse_x), m_mousey(mouse_y) {}

    inline float GetMouseX() { return m_mousex; }
    inline float GetMouseY() { return m_mousey; }

    virtual std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_mousex << ", " << m_mousey;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved);

    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse);
private:
    float m_mousex, m_mousey;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset)
            : m_xoffset(xOffset), m_yoffset(yOffset) {}

    inline float GetXOffset() const { return m_xoffset; }
    inline float GetYOffset() const { return m_yoffset; }

    virtual std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)

    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
private:
    float m_xoffset, m_yoffset;
};

class MouseButtonEvent : public Event {
public:
    inline int getButton() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouseButton);

protected:
    MouseButtonEvent(int button) : m_button(button) {}
    int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button, float mx, float my)
            : MouseButtonEvent(button), mousex{mx}, mousey{my} {}

    inline float GetMouseX() const { return mousex; }
    inline float GetMouseY() const { return mousey; }

    virtual std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed);
private:
    float mousex, mousey;
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button, float mx, float my)
            : MouseButtonEvent(button), mousex{mx}, mousey{my} {}

    inline float GetMouseX() const { return mousex; }
    inline float GetMouseY() const { return mousey; }

    virtual std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased);
private:
    float mousex, mousey;
};

#endif //CLASSNAME_HPP
