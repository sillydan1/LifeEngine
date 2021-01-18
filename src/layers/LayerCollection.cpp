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
#include "LayerCollection.h"

LayerCollection::LayerCollection() : m_layers{}, m_insertionPoint{0} {
}

void LayerCollection::PushLayer(const std::shared_ptr<Layer>& layer) {
    m_layers.emplace(begin() + m_insertionPoint, layer);
    m_insertionPoint++;
    layer->OnAttach();
}

void LayerCollection::PushOverlay(const std::shared_ptr<Layer>& overlay) {
    m_layers.emplace_back(overlay);
    overlay->OnAttach();
}

void LayerCollection::PopLayer(const std::shared_ptr<Layer>& layer) {
    auto it = std::find(begin(), end(), layer);
    if(it != end()) {
        m_layers.erase(it);
        m_insertionPoint--;
    }
    layer->OnDetach();
}

void LayerCollection::PopOverlay(const std::shared_ptr<Layer>& overlay) {
    auto it = std::find(begin(), end(), overlay);
    if(it != end())
        m_layers.erase(it);
    overlay->OnDetach();
}
