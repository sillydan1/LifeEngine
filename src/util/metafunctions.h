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
