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

class layer {
public:
    explicit layer(std::string layer_name)
            : layer_name(std::move(layer_name)) {}
    virtual ~layer() = default;

    virtual void on_event(event& event) {}
    virtual void on_update() {}
    virtual void on_attach() { spdlog::trace("[{0}] Layer attached", layer_name); }
    virtual void on_detach() { spdlog::trace("[{0}] Layer detached", layer_name); }

    inline const std::string& get_name() { return layer_name; }
protected:
    std::string layer_name;
};

#endif //LIFEENGINE_LAYER_H
