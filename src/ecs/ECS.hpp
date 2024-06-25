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
#ifndef ECSNEW_HPP
#define ECSNEW_HPP
#include "Component.hpp"
#include "Entity.hpp"
#include "ecs/System.hpp"
#include "extensions/hash_combine" // TODO: use yahashcombine
#include <extensions/metafunctions.h>
#include <lifepch.h>

struct entity_ptr : std::vector<std::shared_ptr<base_component>> {
    std::string name;
    void change_name(const std::string& new_name) {
        name = new_name;
    }
};

using comp_pair = std::pair<const size_t, std::shared_ptr<base_component>>;
using shared_collection_t = std::vector<std::shared_ptr<base_component>>;
// ECS = Entities, Components, Systems
class ecs {
public:
    template<typename... Ts>
    entity make_entity(Ts&&... initializedTs) {
        static_assert(are_base_of<base_component, Ts...>::value,
                      "Can only make entities from Components");
        static_assert(sizeof...(Ts) > 0,
                      "No template argument was provided.");
        comp_initalizer_list comps = {
                comp_pair(
                        std::hash<component_id_t >{}(Ts::id),
                        std::make_shared<Ts>(std::forward<Ts>(initializedTs)))...
        };
        components.insert(comps);
        {
            // Register all of these into the component_collections collection
            entity_ptr coll{};
            for (auto& c : comps) {
                coll.emplace_back(c.second);
            }
            coll.change_name("Entity");
            size_t thing = 0;
            std::vector<component_id_t> component_ids{{Ts::id...}};
            std::sort(component_ids.begin(), component_ids.end());
            for (auto& c : component_ids)
                hash_combine<component_id_t>(thing, c);
            component_collections.emplace(std::pair<size_t, entity_ptr>(thing, std::move(coll)));
        }
        return entity(comps);
    }

    template<typename... Ts>
    entity make_entity() {
        static_assert(are_base_of<base_component, Ts...>::value,
                      "Can only make entities from Components");
        static_assert(sizeof...(Ts) > 0,
                      "No template argument was provided.");
        comp_initalizer_list comps = {
                comp_pair(
                        std::hash<component_id_t >{}(Ts::id),
                        std::shared_ptr<base_component>(new Ts()))...};
        components.insert(comps);
        {
            // Register all of these into the component_collections collection
            entity_ptr coll{};
            for (auto& c : comps) {
                coll.emplace_back(c.second);
            }
            coll.change_name("Entity");
            size_t thing = 0;
            std::vector<component_id_t> component_ids{{Ts::id...}};
            std::sort(component_ids.begin(),component_ids.end());
            for (auto& c : component_ids)
                hash_combine<component_id_t>(thing, c);

            component_collections.emplace(std::pair<size_t, entity_ptr>(thing, std::move(coll)));
        }
        return entity(comps);
    }
    /// NOTE: Removes an entire entity, not just a single component!
    bool remove_entity(size_t hash, std::vector<std::shared_ptr<base_component>>* entity) {
        for(std::shared_ptr<base_component>& component : *entity) {
            auto d = components.equal_range(std::hash<component_id_t>{}(component->get_id()));
            for(auto it = d.first; it != d.second; ++it) {
                if(it->second.get() == component.get()) {
                    components.erase(it);
                    break;
                }
            }
        }
        auto c = component_collections.equal_range(hash);
        for(auto it = c.first; it != c.second; ++it) {
            if(&it->second == entity) {
                component_collections.erase(it);
                return true;
            }
        }
        return false;
    }

    void update_systems(SystemList& list) {
        // TODO: Systems with multiple requirements on an entity
        for(auto& system : list) {
            // Extract the components from the collection that this system wants to interact with
            if(system->multi_component) {
                if(system->filterType == component_filter_type::OR) {
                    /// System acts on a lot of <similar> components.
                    auto cids = system->GetComponentIDs();
                    for(auto& id : cids) {
                        auto d = components.equal_range(std::hash<component_id_t>{}(id));
                        for(auto it = d.first; it != d.second; ++it) {
                            system->Update((*it).second);
                        }
                    }
                } else {
                    /// System acts on entities with multiple component types.
                    auto d = component_collections.equal_range(system->component_filter);
                    for (auto it = d.first; it != d.second; ++it) {
                        system->UpdateMultiple((*it).second);
                    }
                }
            } else {
                /// System act only on one component type
                auto d = components.equal_range(system->component_filter);
                for(auto it = d.first; it != d.second; ++it) {
                    system->Update((*it).second);
                }
            }
        }
    }
    using component_collection_collection_t = std::unordered_multimap<size_t, entity_ptr>;
    using component_collection_t = std::unordered_multimap<size_t, std::shared_ptr<base_component>>;

    component_collection_t& get_components() { return components; }
    component_collection_collection_t& get_entities() { return component_collections; }
private:
    component_collection_collection_t component_collections;
    component_collection_t components;
};

#endif //ECS_HPP
