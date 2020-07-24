#ifndef METAFUNCTIONS_H
#define METAFUNCTIONS_H
#include <type_traits>

namespace life {
	// are_base_of. This is used for recursively checking if all elements in a variadic template
	// is a base of typename Base.
	template <typename Base, typename T, typename... Ts>
	struct are_base_of :
		std::conditional<std::is_base_of_v<Base, T>, are_base_of<Base, Ts...>,
						std::false_type>::type
	{};
	
	template <typename Base, typename T>
	struct are_base_of<Base, T> : std::is_base_of<Base, T> {};

	template<typename T, typename... Ts>
	struct are_lvalue_references :
	        std::conditional<std::is_lvalue_reference_v<T>,
	                         are_lvalue_references<Ts...>,
	                         std::false_type>::type
    {};
    template<typename T>
    struct are_lvalue_references<T> : std::is_lvalue_reference<T> {};
}

#endif //METAFUNCTIONS_H
