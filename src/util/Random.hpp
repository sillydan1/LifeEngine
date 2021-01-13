/**
 * Copyright (C) 2021 Asger Gitz-Johansen

   This file is part of lifeengine.

    lifeengine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    lifeengine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with lifeengine.  If not, see <https://www.gnu.org/licenses/>.
 */
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
