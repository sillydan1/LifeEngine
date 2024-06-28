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
#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <lifepch.h>
#include <extensions/stringextensions.h>
#include "Component.hpp"
#include <extensions/hash_combine>

enum class component_filter_type {
    AND,
    OR
};

class life_system {
    std::vector<component_id_t> m_comp_ids;
    bool finalized; // Mostly just for debugging
public:
    size_t component_filter; // TODO: Optional components
    bool multi_component;
    component_filter_type filterType;

    life_system() : finalized{false}, component_filter{}, multi_component{false}, filterType{component_filter_type::AND} {}
    virtual ~life_system() = default;
    // TODO: Systems should <act> on collections of components (entities)
    // TODO: Systems should "say" what types of entities they want to act on

    virtual void Update(const std::shared_ptr<base_component>& component) {

    }

    virtual void UpdateMultiple(const std::vector<std::shared_ptr<base_component>>& components) {

    }

    inline std::vector<component_id_t> GetComponentIDs() const {
        return m_comp_ids;
    }

protected:
    void SetComponentFilterType(const component_filter_type& afilterType) {
        filterType = afilterType;
    }

    void AddComponentType(const component_id_t& id) {
        m_comp_ids.push_back(id);
    }

    inline void FinalizeComponentAdding() {
        if(!finalized) {
            multi_component = m_comp_ids.size() > 1;
            if(multi_component) {
                std::sort(m_comp_ids.begin(), m_comp_ids.end());
                for (auto &id : m_comp_ids)
                    hash_combine<component_id_t>(component_filter, id);
            } else
                component_filter = std::hash<component_id_t>{}(m_comp_ids[0]);

            finalized = true;
            if(filterType == component_filter_type::AND)
                m_comp_ids.clear();
        } else
            spdlog::error("ECS System is being finalized more than once!");
    }
};

class SystemList {
public:
    template<typename T, class = std::enable_if_t<std::is_base_of_v<life_system, T>> >
    inline void PushSystem(const T& system) {
        m_systems.push_back(std::make_unique<T>(system));
    }
    template<typename T, class = std::enable_if_t<std::is_base_of_v<life_system, T>> >
    inline void PushSystem(T&& system) {
        m_systems.push_back(std::make_unique<T>(std::move(system)));
    }
    template<typename T, class = std::enable_if_t<std::is_base_of_v<life_system, T>> >
    inline void PushSystem() {
        m_systems.push_back(std::make_unique<T>());
    }
    template<typename T, class = std::enable_if_t<std::is_base_of_v<life_system, T>> >
    inline void PushSystem(std::unique_ptr<T>&& v) {
        m_systems.push_back(std::move(v));
    }
    // SystemLists should be iterable
    auto begin() { return m_systems.begin(); }
    auto end()   { return m_systems.end(); }

private:
    std::vector<std::unique_ptr<life_system>> m_systems;
};

#endif //SYSTEM_HPP
