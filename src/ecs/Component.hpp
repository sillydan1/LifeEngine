#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <functional> 

namespace life::ecs {

	using component_id_t = unsigned int;

	class BaseComponent {
		friend struct std::hash<BaseComponent>;
		const component_id_t runtimeID;
	protected:
		BaseComponent(const component_id_t id) : runtimeID{id} { }
		virtual ~BaseComponent() = default;

	public:
        virtual const char* GetComponentName() { return "ERROR"; }
		static component_id_t current_comp_id;
		static component_id_t GetNewComponentID() noexcept;
		const component_id_t GetID() const noexcept;
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
}

namespace std {
    template<typename T>
    struct hash<life::ecs::Component<T>> {
        inline std::size_t operator()(const life::ecs::Component<T>& c) const noexcept {
            return std::hash<life::ecs::component_id_t>{}(c.ID);
        }
    };

    template<>
    struct hash<life::ecs::BaseComponent> {
        inline std::size_t operator()(const life::ecs::BaseComponent& c) const noexcept {
            return std::hash<life::ecs::component_id_t>{}(c.runtimeID);
        }
    };

    // hash(life::ecs::BaseComponent&&) -> std::hash<life::ecs::BaseComponent>;
}
#endif //COMPONENT_HPP
