#ifndef KEYEVENTS_HPP
#define KEYEVENTS_HPP
#include "Event.hpp"

namespace life {

    class KeyEvent : public Event {
    public:
        int getKeyCode() const { return m_keycode; }

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput);
    protected:
        KeyEvent(int keycode)
                : m_keycode(keycode) {}

        int m_keycode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
                : KeyEvent(keycode), m_repeatCount(repeatCount) {}

        EVENT_CLASS_TYPE(KeyPressed);

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keycode;
            if(m_repeatCount > 0)
                ss << "\t repeated: " << m_repeatCount;
            return ss.str();
        }

        inline const int getRepeatCount() const { return m_repeatCount; }

    protected:
        int m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keycode)
                : KeyEvent(keycode) {}

        EVENT_CLASS_TYPE(KeyReleased);

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keycode;
            return ss.str();
        }
    };
}

#endif //CLASSNAME_HPP
