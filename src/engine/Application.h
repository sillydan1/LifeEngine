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

#ifndef LIFEENGINE_APPLICATION_H
#define LIFEENGINE_APPLICATION_H
#include <lifepch.h>
#include "window/Window.hpp"
#include "layers/LayerCollection.h"

class application {
protected:
    bool isRunning;
    Window* window;
    LayerCollection layers;
public:
    application();
    virtual ~application() = default;
    void propagate_event_across_layers(event& event);
    virtual void game_start();
    virtual void handle_application_event(event& event);
    inline void push_layer(const std::shared_ptr<layer>& layer) { layers.PushLayer(layer); }
    template<typename T, typename... Args> inline void PushLayer(Args... args) { layers.PushLayer(std::static_pointer_cast<layer>(std::make_shared<T>(args...))); }
    inline void overlay_layer(const std::shared_ptr<layer>& layer) { layers.PushOverlay(layer); }
    template<typename T, typename... Args> inline void OverlayLayer(Args... args) { layers.PushOverlay(std::static_pointer_cast<layer>(std::make_shared<T>(args...))); }
};

#endif //LIFEENGINE_APPLICATION_H
