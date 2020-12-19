#ifndef APPLICATION_EVENTS_HPP
#define APPLICATION_EVENTS_HPP
#include "Event.hpp"

namespace life {


    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
                : m_width(width), m_height(height) {}

        inline unsigned int GetWidth() const { return m_width; }
        inline unsigned int GetHeight() const { return m_height; }

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize);

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    private:
        unsigned int m_width, m_height;
    };
    // Very simple events here
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        virtual std::string ToString() const override {
            return "WindowCloseEvent";
        }

        EVENT_CLASS_TYPE(WindowClose);

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };

    class AppTickEvent : public Event {
    public:
        AppTickEvent() {}

        virtual std::string ToString() const override {
            return "AppTickEvent";
        }

        EVENT_CLASS_TYPE(AppTick);

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };

    class AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() {}

        virtual std::string ToString() const override {
            return "AppUpdateEvent";
        }

        EVENT_CLASS_TYPE(AppUpdate);

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };

    class AppRenderEvent : public Event {
    public:
        AppRenderEvent() {}

        virtual std::string ToString() const override {
            return "AppRenderEvent";
        }

        EVENT_CLASS_TYPE(AppRender);

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);
    };
    
    class RenderResizeEvent : public Event {
	public:
    	RenderResizeEvent(float x, float y) : x{x}, y{y}, user_data{""} {}
    	RenderResizeEvent(float x, float y, const char* userdata) : x{x}, y{y}, user_data{userdata} {}

    	virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "RenderResizeEvent: " << x << ", " << y;
			return ss.str();
    	}
	
		inline float GetWidth() const { return x; }
		inline float GetHeight() const { return y; }
    	
    	EVENT_CLASS_TYPE(RenderResize);
    	
    	EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication);

    	const char* GetUserData() { return user_data; }
    private:
    	float x, y;
        const char* user_data;
    };

}

#endif //CLASSNAME_HPP
