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
#include <lifepch.h>
#include <window/Window.hpp>
#include <util/Time.h>
#include "Application.h"

Application::Application()
 : close{false},
   window{Window::Create(WindowProperties{})},
   layers{}
{
    window->SetEventCallback(FNBIND(PropagateEventAcrossLayers));
}

void Application::PropagateEventAcrossLayers(Event& event) {
    if(event.GetEventCategory() == EventCategory::EventCategoryApplication)
        HandleApplicationEvent(event);
    for(auto& layer : layers) layer->OnEvent(event);
    event.SetHandled();
}

void Application::HandleApplicationEvent(Event &event) {
    switch (event.GetEventType()) {
        case EventType::WindowClose:
            close = true; break;
        default:
            break;
    }
    event.SetHandled();
}

void Application::GameStart() {
    Time::GameStart();
    while(!close) {
        for(auto& layer : layers)
            layer->OnUpdate();
        window->OnUpdate();
    }
    delete window;
}
