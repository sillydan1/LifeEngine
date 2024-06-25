#ifndef MOUSE_EVENTS_HPP
#define MOUSE_EVENTS_HPP
#include "Event.hpp"

class mouse_moved_event : public event {
public:
    mouse_moved_event(float mouse_x, float mouse_y)
            : mouse_x{mouse_x}, mouse_y{mouse_y} {}

    inline float get_mouse_x() { return mouse_x; }
    inline float get_mouse_y() { return mouse_y; }

    virtual std::string to_string() const override {
        return "mouse_moved_event: " + std::to_string(mouse_x) + ", " + std::to_string(mouse_y);
    }

    EVENT_CLASS_TYPE(MouseMoved);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryMouse);
private:
    float mouse_x, mouse_y;
};

class mouse_scrolled_event : public event {
public:
    mouse_scrolled_event(float xOffset, float yOffset)
            : xoffset(xOffset), yoffset(yOffset) {}

    inline float get_x_offset() const { return xoffset; }
    inline float get_y_offset() const { return yoffset; }

    virtual std::string to_string() const override {
        return "mouse_scrolled_event: " + std::to_string(get_x_offset()) + ", " + std::to_string(get_y_offset());
    }

    EVENT_CLASS_TYPE(MouseScrolled)

    EVENT_CLASS_CATEGORY(event_category::EventCategoryMouse | event_category::EventCategoryInput)
private:
    float xoffset, yoffset;
};

class bouse_button_event : public event {
public:
    inline int get_button() const { return button; }

    EVENT_CLASS_CATEGORY(event_category::EventCategoryMouseButton);

protected:
    bouse_button_event(int button) : button(button) {}
    int button;
};

class mouse_button_pressed_event : public bouse_button_event {
public:
    mouse_button_pressed_event(int button, float mx, float my)
            : bouse_button_event(button), mouse_x{mx}, mouse_y{my} {}

    inline float get_mouse_x() const { return mouse_x; }
    inline float get_mouse_y() const { return mouse_y; }

    virtual std::string to_string() const override {
        return "mouse_button_pressed_event: " + std::to_string(button);
    }

    EVENT_CLASS_TYPE(MouseButtonPressed);
private:
    float mouse_x, mouse_y;
};

class mouse_button_released_event : public bouse_button_event {
public:
    mouse_button_released_event(int button, float mx, float my)
            : bouse_button_event(button), mouse_x{mx}, mouse_y{my} {}

    inline float get_mouse_x() const { return mouse_x; }
    inline float get_mouse_y() const { return mouse_y; }

    virtual std::string to_string() const override {
        return "mouse_button_released_event: " + std::to_string(button);
    }

    EVENT_CLASS_TYPE(MouseButtonReleased);
private:
    float mouse_x, mouse_y;
};

#endif //CLASSNAME_HPP
