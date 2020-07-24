#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <memory>
#include <vector>
#include <set>
#include <util/utillities>
#include "Component.hpp"

namespace life::ecs {
	
	enum class ComponentFilterType {
		AND,
		OR
	};
	
	class System {
        std::vector<component_id_t> m_comp_ids;
        bool finalized; // Mostly just for debugging
	public:
        size_t component_filter; // TODO: Optional components
        bool multi_component;
		ComponentFilterType filterType;

	    System() : finalized{false}, component_filter{}, multi_component{false}, filterType{ComponentFilterType::AND} {}
		virtual ~System() = default;
		// TODO: Systems should <act> on collections of components (entities)
		// TODO: Systems should "say" what types of entities they want to act on
		
		virtual void Update(const std::shared_ptr<BaseComponent>& component) {
			
		}

		virtual void UpdateMultiple(const std::vector<std::shared_ptr<BaseComponent>>& components) {

		}
		
		inline std::vector<component_id_t> GetComponentIDs() const {
			return m_comp_ids;
		}
		
	protected:
		void SetComponentFilterType(const ComponentFilterType& afilterType) {
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
                    for (auto &id : m_comp_ids) {
                        hash_combine<component_id_t, std::hash<component_id_t>>(component_filter, id);
                    }
                } else {
                    component_filter = std::hash<component_id_t>{}(m_comp_ids[0]);
                }
                finalized = true;
                if(filterType == ComponentFilterType::AND)
                	m_comp_ids.clear(); // not used any more.
            } else {
		        LIFE_ERR("ECS System is being finalized more than once!");
		    }
		}
	};

    class SystemList {
	public:
        template<typename T, class = std::enable_if_t<std::is_base_of_v<System, T>> >
        inline void PushSystem(const T& system) {
            m_systems.push_back(std::make_unique<T>(system));
        }
		template<typename T, class = std::enable_if_t<std::is_base_of_v<System, T>> >
		inline void PushSystem(T&& system) {
			m_systems.push_back(std::make_unique<T>(std::move(system)));
		}
        template<typename T, class = std::enable_if_t<std::is_base_of_v<System, T>> >
        inline void PushSystem() {
            m_systems.push_back(std::make_unique<T>());
        }
        template<typename T, class = std::enable_if_t<std::is_base_of_v<System, T>> >
        inline void PushSystem(std::unique_ptr<T>&& v) {
        	m_systems.push_back(std::move(v));
        }
		// SystemLists should be iterable
		auto begin() { return m_systems.begin(); }
		auto end()   { return m_systems.end(); }

	private:
		std::vector<std::unique_ptr<System>> m_systems;
	};

}
#endif //SYSTEM_HPP
