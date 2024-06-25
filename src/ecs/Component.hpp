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
#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <lifepch.h>

using component_id_t = unsigned int;

class base_component {
    friend struct std::hash<base_component>;
    const component_id_t runtimeID;
protected:
    explicit base_component(const component_id_t id) : runtimeID{id} { }
    virtual ~base_component() = default;

public:
    virtual const char* get_component_name() { return "ERROR"; }
    static component_id_t current_comp_id;
    static component_id_t get_new_component_id() noexcept;
    [[nodiscard]] component_id_t get_id() const noexcept;
};

template<typename T>
struct component : public base_component {
    static const component_id_t id;
    component() : base_component(id) { }
    virtual ~component() = default; // Enable this again when you need any form of virtuality from Component
    const char* get_component_name() override { return typeid(T).name(); }
};

template<typename T>
const component_id_t component<T>::id(base_component::get_new_component_id());
bool operator==(const base_component& a, const base_component& b);

namespace std {
    template<typename T>
    struct hash<component<T>> {
        inline std::size_t operator()(const component<T>& c) const noexcept {
            return std::hash<component_id_t>{}(c.id);
        }
    };

    template<>
    struct hash<base_component> {
        inline std::size_t operator()(const base_component& c) const noexcept {
            return std::hash<component_id_t>{}(c.runtimeID);
        }
    };
}
#endif //COMPONENT_HPP
