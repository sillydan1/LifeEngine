#include "Event.hpp"

EventCategory operator|(EventCategory a, EventCategory b) {
    return EventCategory(int(a) | int(b));
}
