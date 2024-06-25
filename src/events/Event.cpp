#include "Event.hpp"

event_category operator|(event_category a, event_category b) {
    return event_category(int(a) | int(b));
}
