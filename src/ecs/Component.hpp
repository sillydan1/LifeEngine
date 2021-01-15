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

class BaseComponent {
    friend struct std::hash<BaseComponent>;
    const component_id_t runtimeID;
protected:
    explicit BaseComponent(const component_id_t id) : runtimeID{id} { }
    virtual ~BaseComponent() = default;

public:
    virtual const char* GetComponentName() { return "ERROR"; }
    static component_id_t current_comp_id;
    static component_id_t GetNewComponentID() noexcept;
    [[nodiscard]] component_id_t GetID() const noexcept;
};

template<typename T>
struct Component : public BaseComponent {
    static const component_id_t ID;
    Component() : BaseComponent(ID) { }
    virtual ~Component() = default; // Enable this again when you need any form of virtuality from Component
    const char* GetComponentName() override { return typeid(T).name(); }
};

template<typename T>
const component_id_t Component<T>::ID(BaseComponent::GetNewComponentID());
bool operator==(const BaseComponent& a, const BaseComponent& b);

namespace std {
    template<typename T>
    struct hash<Component<T>> {
        inline std::size_t operator()(const Component<T>& c) const noexcept {
            return std::hash<component_id_t>{}(c.ID);
        }
    };

    template<>
    struct hash<BaseComponent> {
        inline std::size_t operator()(const BaseComponent& c) const noexcept {
            return std::hash<component_id_t>{}(c.runtimeID);
        }
    };
}
#endif //COMPONENT_HPP
