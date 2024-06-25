#ifndef KEYEVENTS_HPP
#define KEYEVENTS_HPP
#include "Event.hpp"

class key_event : public event {
public:
    int get_key_code() const { return keycode; }

    EVENT_CLASS_CATEGORY(event_category::EventCategoryKeyboard | event_category::EventCategoryInput);
protected:
    key_event(int keycode)
            : keycode(keycode) {}

    int keycode;
};

class key_pressed_event : public key_event {
public:
    key_pressed_event(int keycode, int repeatCount)
            : key_event{keycode}, repeat_count{repeatCount} {}

    EVENT_CLASS_TYPE(KeyPressed);

    virtual std::string to_string() const override {
        std::stringstream ss;
        ss << "key_pressed_event: " << keycode;
        if(repeat_count > 0)
            ss << "\t repeated: " << repeat_count;
        return ss.str();
    }

    inline const int getRepeatCount() const { return repeat_count; }

protected:
    int repeat_count;
};

class key_released_event : public key_event {
public:
    key_released_event(int keycode)
            : key_event(keycode) {}

    EVENT_CLASS_TYPE(KeyReleased);

    virtual std::string to_string() const override {
        return "key_released_event: " + std::to_string(keycode);
    }
};

#endif //CLASSNAME_HPP
