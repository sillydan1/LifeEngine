#ifndef APPLICATION_EVENTS_HPP
#define APPLICATION_EVENTS_HPP
#include "Event.hpp"

class window_resize_event : public event {
public:
    window_resize_event(unsigned int width, unsigned int height)
            : width(width), height(height) {}

    inline unsigned int get_width() const { return width; }
    inline unsigned int get_height() const { return height; }

    virtual std::string to_string() const override {
        return "window_resize_event: " + std::to_string(width) + ", " + std::to_string(height);
    }

    EVENT_CLASS_TYPE(WindowResize);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryApplication);
private:
    unsigned int width, height;
};
// Very simple events here
class window_close_event : public event {
public:
    window_close_event() {}

    virtual std::string to_string() const override {
        return "window_close_event";
    }

    EVENT_CLASS_TYPE(WindowClose);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryApplication);
};

class app_tick_event : public event {
public:
    app_tick_event() {}

    virtual std::string to_string() const override {
        return "app_tick_event";
    }

    EVENT_CLASS_TYPE(AppTick);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryApplication);
};

class app_update_event : public event {
public:
    app_update_event() {}

    virtual std::string to_string() const override {
        return "app_update_event";
    }

    EVENT_CLASS_TYPE(AppUpdate);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryApplication);
};

class app_render_event : public event {
public:
    app_render_event() {}

    virtual std::string to_string() const override {
        return "app_render_event";
    }

    EVENT_CLASS_TYPE(AppRender);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryApplication);
};

class render_resize_event : public event {
public:
    render_resize_event(float x, float y) : width{x}, height{y}, user_data{""} {}
    render_resize_event(float x, float y, const char* userdata) : width{x}, height{y}, user_data{userdata} {}

    virtual std::string to_string() const override {
        return "render_resize_event: " + std::to_string(width) + ", " + std::to_string(height);
    }

    inline float get_width() const { return width; }
    inline float get_height() const { return height; }

    EVENT_CLASS_TYPE(RenderResize);

    EVENT_CLASS_CATEGORY(event_category::EventCategoryApplication);

    const char* GetUserData() { return user_data; }
private:
    float width, height;
    const char* user_data;
};

#endif //CLASSNAME_HPP
