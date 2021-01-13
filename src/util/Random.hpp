#ifndef LIFE_ENGINE_RANDOM_HPP
#define LIFE_ENGINE_RANDOM_HPP
#include <random>
#include <lifepch.h>

class Random {
public:
    static void Init();
    static void Destroy();
    static float GenerateFloat();
    static float GenerateFloat(float range_start, float range_end);
    static glm::vec3 GenerateVec3(float range_start, float range_end);
    static int GenerateInt();
    static int GenerateInt(int range_start, int range_end);
};

#endif //LIFE_ENGINE_RANDOM_HPP
