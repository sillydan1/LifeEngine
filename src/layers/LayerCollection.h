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
#ifndef LIFEENGINE_LAYERSTACK_H
#define LIFEENGINE_LAYERSTACK_H
#include <lifepch.h>
#include "Layer.h"

class layer_collection {
public:
    layer_collection();
    ~layer_collection();

    void push_layer(const std::shared_ptr<layer>& layer);
    void push_overlay(const std::shared_ptr<layer>& overlay);
    void pop_layer(const std::shared_ptr<layer>& layer);
    void pop_overlay(const std::shared_ptr<layer>& overlay);

    [[nodiscard]] inline std::vector<std::shared_ptr<layer>>::iterator begin() { return m_layers.begin(); }
    [[nodiscard]] inline std::vector<std::shared_ptr<layer>>::iterator end() { return m_layers.end(); }
    [[nodiscard]] inline size_t size() const { return m_layers.size(); }
private:
    std::vector<std::shared_ptr<layer>> m_layers;
    unsigned int m_insertionPoint = 0;
};

#endif //LIFEENGINE_LAYERSTACK_H
