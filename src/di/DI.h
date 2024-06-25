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
#ifndef LIFEENGINE_DI_H
#define LIFEENGINE_DI_H
#include <lifepch.h>

struct di {
    template<typename T>
    static T Get() {
        return std::any_cast<T>(instances[typeid(T).name()]);
    }

    template<typename T>
    static void Set(T&& object) {
        instances[typeid(object).name()] = object;
    }

    template<typename T>
    static void Set(const T& object) {
        instances[typeid(object).name()] = object;
    }

private:
    static std::unordered_map<std::string, std::any> instances;
};

#endif //LIFEENGINE_DI_H
