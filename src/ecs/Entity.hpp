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
class Entity;
#include <lifepch.h>
#include <extensions/metafunctions.h>
#include <extensions/stringextensions.h>
#include "Component.hpp"

using comp_initalizer_list = std::initializer_list<std::pair<const size_t, std::shared_ptr<BaseComponent>>>;

class Entity {
public:
    Entity() noexcept : m_components{}, m_is_empty{true} {}

    explicit Entity(const comp_initalizer_list& components) {
        m_components.insert(components);
        m_is_empty = false;
    }

    inline bool IsEmpty() const { return m_is_empty || m_components.empty(); }

    template<typename T>
    std::shared_ptr<T> GetComponent() {
        static_assert(std::is_base_of_v<BaseComponent, T>,
            "GetComponent on an ECS Entity: T has to be a specialization of Component<T>");

        auto d = m_components.find(T::ID);
        if(d == m_components.end()) throw std::logic_error("Component not available on this Entity");
        return std::static_pointer_cast<T>(d->second);
    }

    template<typename T>
    std::shared_ptr<T> GetComponent(unsigned int i) {
        static_assert(std::is_base_of_v<BaseComponent, T>,
                      "GetComponent on an ECS Entity: T has to be a specialization of Component<T>");

        int count = m_components.count(T::ID);
        if(count <= 0) throw std::logic_error("Component not available on this Entity");
        if(i > count)  throw std::out_of_range(string_formatter() << "Index out of range ("<<i<<"): Entity only has "<<count<<" components of that type!");

        auto d = m_components.equal_range(T::ID);
        for (int j = 0; j < i; ++j) {
            d.first++; // Move the iterator over to the desired place.
        }
        return std::static_pointer_cast<T>(d.first->second);
    }

private:
    using component_map_t = std::unordered_multimap<size_t, std::shared_ptr<BaseComponent>>;
    component_map_t m_components;
    bool m_is_empty;
};

#endif //ENTITY_HPP
