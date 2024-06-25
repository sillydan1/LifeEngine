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
#ifndef LIFEENGINE_LAYER_H
#define LIFEENGINE_LAYER_H
#include <lifepch.h>

#include <utility>
#include "events/Event.hpp"

class Layer {
public:
    explicit Layer(std::string layer_name)
            : m_dbgname(std::move(layer_name)) {}
    virtual ~Layer() = default;

    virtual void OnEvent(event& event) {}
    virtual void OnUpdate() {}
    virtual void OnAttach() { spdlog::trace("[{0}] Layer attached", m_dbgname); }
    virtual void OnDetach() { spdlog::trace("[{0}] Layer detached", m_dbgname); }

    inline const std::string& GetName() { return m_dbgname; }
protected:
    std::string m_dbgname;
};

#endif //LIFEENGINE_LAYER_H
