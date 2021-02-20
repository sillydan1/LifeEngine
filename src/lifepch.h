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

#ifndef LIFEENGINE_LIFEPCH_H
#define LIFEENGINE_LIFEPCH_H

// General purpose STL things
#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <type_traits>

#define BIT(x) (1 << x)
#define FNBIND(x) [this](auto && PH1) { x(std::forward<decltype(PH1)>(PH1)); }

// Data structures
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <variant>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <any>

// GLM
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/exponential.hpp>
#include <glm/common.hpp>
#include <glm/packing.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/vector_relational.hpp>
#include <glm/integer.hpp>

// Extra "big" libraries that are used everywhere
#include <spdlog/spdlog.h>

#endif //LIFEENGINE_LIFEPCH_H
