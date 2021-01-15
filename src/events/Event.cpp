#include "Event.hpp"

EventCategory operator|(EventCategory a, EventCategory b) {
    // TODO: Not sure if this'll impact performance, please check if it does
    return EventCategory(int(a) | int(b));
}
