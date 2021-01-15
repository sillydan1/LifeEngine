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
#include "Component.hpp"

component_id_t BaseComponent::current_comp_id=0;

component_id_t BaseComponent::GetNewComponentID() noexcept  { return current_comp_id++; }
component_id_t BaseComponent::GetID() const noexcept  { return runtimeID; }

bool operator==(const BaseComponent& a, const BaseComponent& b) {
    return std::hash<BaseComponent>{}(a) == std::hash<BaseComponent>{}(b);
}
