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

class Application {
    bool isRunning;
    Window* window;
    LayerCollection layers;
public:
    Application();
    virtual ~Application() = default;
    void PropagateEventAcrossLayers(Event& event);
    virtual void GameStart();
    void HandleApplicationEvent(Event& event);
    void PushLayer(const std::shared_ptr<Layer>& layer) { layers.PushLayer(layer); }
    void OverlayLayer(const std::shared_ptr<Layer>& layer) { layers.PushOverlay(layer); }
};

#endif //LIFEENGINE_APPLICATION_H
