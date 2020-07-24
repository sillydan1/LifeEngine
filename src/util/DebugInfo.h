#ifndef LIFE_ENGINE_DEBUGINFO_H
#define LIFE_ENGINE_DEBUGINFO_H
#include <string>

namespace life {
    /**
     * DebugInfo
     * - A static class containing runtime debugging information.
     * */
    struct DebugInfo {
        //// TODO: In the future, this should be a variable-length collection of data instead of just static fields
        static std::string RenderInformation;
        static unsigned int DrawCalls;
    };

}

#endif //LIFE_ENGINE_DEBUGINFO_H
