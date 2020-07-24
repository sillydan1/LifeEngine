#ifndef LIFE_ENGINE_TIME_H
#define LIFE_ENGINE_TIME_H
#include "Timer.hpp"

namespace life {

    struct Time {
        static double GetGlobalTime();
        static float GetFrameTime();
        static void GameStart();
        static void FrameEnd();
    };

}

#endif //LIFE_ENGINE_TIME_H
