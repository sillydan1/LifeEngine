#ifndef ECSNEW_HPP
#define ECSNEW_HPP
#include "Component.hpp"
#include "Entity.hpp"
#include "ecs"
#include <util/metafunctions.h>
#include <unordered_set>

namespace life::ecs {

    struct entity_ptr : std::vector<std::shared_ptr<BaseComponent>> {
        std::string m_name;
        void ChangeName(const std::string& new_name) {
            m_name = new_name;
        }
    };

    using comp_pair = std::pair<const size_t, std::shared_ptr<BaseComponent>>;
    using shared_collection_t = std::vector<std::shared_ptr<BaseComponent>>;
    // ECS = Entities, Components, Systems
	class ECS {
	public:

		template<typename... Ts>
		Entity makeEntity(Ts&&... initializedTs) {
            static_assert(are_base_of<BaseComponent, Ts...>::value,
                          "Can only make entities from life::ecs::Components");
            static_assert(sizeof...(Ts) > 0,
                          "No template argument was provided.");
            comp_initalizer_list comps = {
                    comp_pair(
                            std::hash<component_id_t >{}(Ts::ID),
                            std::make_shared<Ts>(std::forward<Ts>(initializedTs)))...
            };
			components.insert(comps);
            {
                // Register all of these into the component_collections collection
                entity_ptr coll{};
                for (auto& c : comps) {
                    coll.emplace_back(c.second);
                }
                coll.ChangeName("Entity");
                size_t thing = 0;
                std::vector<component_id_t> component_ids{{Ts::ID...}};
                std::sort(component_ids.begin(), component_ids.end());
                for (auto& c : component_ids) {
                    hash_combine<component_id_t, std::hash<component_id_t>>(thing, c);
                }
                component_collections.emplace(std::pair<size_t, entity_ptr>(thing, std::move(coll)));
            }
			return Entity(comps);
		}
		
		template<typename... Ts>
		Entity makeEntity() {
            static_assert(are_base_of<BaseComponent, Ts...>::value,
                          "Can only make entities from life::ecs::Components");
            static_assert(sizeof...(Ts) > 0,
                          "No template argument was provided.");
            comp_initalizer_list comps = {
                    comp_pair(
                            std::hash<component_id_t >{}(Ts::ID),
                            std::shared_ptr<BaseComponent>(new Ts()))...};
            components.insert(comps);
            {
                // Register all of these into the component_collections collection
                entity_ptr coll{};
                for (auto& c : comps) {
                    coll.emplace_back(c.second);
                }
                coll.ChangeName("Entity");
                size_t thing = 0;
                std::vector<component_id_t> component_ids{{Ts::ID...}};
                std::sort(component_ids.begin(),component_ids.end());
                for (auto& c : component_ids) {
                    hash_combine<component_id_t, std::hash<component_id_t>>(thing, c);
                }
                component_collections.emplace(std::pair<size_t, entity_ptr>(thing, std::move(coll)));
            }
            return Entity(comps);
		}
        /// Please note: Removes an entire entity, not just a single component!
		bool RemoveEntity(size_t hash, std::vector<std::shared_ptr<BaseComponent>>* entity) {
            for(std::shared_ptr<BaseComponent>& component : *entity) {
                auto d = components.equal_range(std::hash<component_id_t>{}(component->GetID()));
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
		
		void UpdateSystems(SystemList& list) {
            // TODO: Systems with multiple requirements on an entity
		    for(auto& system : list) {
                // Extract the components from the collection that this system wants to interact with
                if(system->multi_component) {
                	if(system->filterType == ComponentFilterType::OR) {
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
		using component_collection_t            = std::unordered_multimap<size_t, std::shared_ptr<BaseComponent>>;

        component_collection_t& GetAllComponents() { return components; }
        component_collection_collection_t& GetAllEntities() { return component_collections; }
	private:
        component_collection_collection_t component_collections;
        component_collection_t            components;
	};
	
}

#endif //ECS_HPP
