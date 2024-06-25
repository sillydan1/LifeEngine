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
#ifndef ENTITY_HPP
#define ENTITY_HPP
class entity;
#include <lifepch.h>
#include <extensions/metafunctions.h>
#include <extensions/stringextensions.h>
#include "Component.hpp"

using comp_initalizer_list = std::initializer_list<std::pair<const size_t, std::shared_ptr<base_component>>>;

class entity {
public:
    entity() noexcept : components{}, empty{true} {}

    explicit entity(const comp_initalizer_list& components) {
        this->components.insert(components);
        empty = false;
    }

    inline bool is_empty() const { return empty || components.empty(); }

    template<typename T>
    std::shared_ptr<T> get_component() {
        static_assert(std::is_base_of_v<base_component, T>,
            "GetComponent on an ECS Entity: T has to be a specialization of Component<T>");

        auto d = components.find(T::id);
        if(d == components.end()) throw std::logic_error("Component not available on this Entity");
        return std::static_pointer_cast<T>(d->second);
    }

    template<typename T>
    std::shared_ptr<T> get_component(unsigned int i) {
        static_assert(std::is_base_of_v<base_component, T>,
                      "GetComponent on an ECS Entity: T has to be a specialization of Component<T>");

        int count = components.count(T::id);
        if(count <= 0)
            throw std::logic_error("Component not available on this Entity");
        // TODO: Upgrade to C++20 so that we can get std::format
        if(i > count)
            throw std::out_of_range("Index out of range ({i}): Entity only has '{count}' components of that type!");

        auto d = components.equal_range(T::id);
        for (int j = 0; j < i; ++j)
            d.first++; // Move the iterator over to the desired place.
        return std::static_pointer_cast<T>(d.first->second);
    }

private:
    using component_map_t = std::unordered_multimap<size_t, std::shared_ptr<base_component>>;
    component_map_t components;
    bool empty;
};

#endif //ENTITY_HPP
