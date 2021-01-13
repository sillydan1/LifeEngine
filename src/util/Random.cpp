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
#include "Random.hpp"

std::default_random_engine* generator = nullptr;

void Random::Init() {
    generator = new std::default_random_engine();
}

void Random::Destroy() {
    delete generator;
    generator = nullptr;
}

float Random::GenerateFloat() {
    std::uniform_real_distribution<float> d{std::numeric_limits<float>::min(), std::numeric_limits<float>::max()};
    return d(*generator);
}

float Random::GenerateFloat(float range_start, float range_end) {
    std::uniform_real_distribution<float> d{range_start, range_end};
    return d(*generator);
}

int Random::GenerateInt() {
    std::uniform_int_distribution<int> d{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
    return d(*generator);
}

int Random::GenerateInt(int range_start, int range_end) {
    std::uniform_int_distribution<int> d{range_start, range_end};
    return d(*generator);
}

// TODO: Right now this is just bogo-unique. It shouldn't be, but this is what I have so far.
glm::vec3 Random::GenerateVec3(float range_start, float range_end) {
    return glm::vec3(Random::GenerateFloat(range_start,range_end),
                     Random::GenerateFloat(range_start,range_end),
                     Random::GenerateFloat(range_start,range_end));
}
