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

application::application()
 : isRunning{true},
   window{Window::Create(WindowProperties{})},
   layers{}
{
    window->SetEventCallback(FNBIND(propagate_event_across_layers));
}

void application::propagate_event_across_layers(Event& event) {
    if(event.GetEventCategory() == EventCategory::EventCategoryApplication)
        handle_application_event(event);
    for(auto& layer : layers) layer->OnEvent(event);
    event.SetHandled();
}

void application::handle_application_event(Event &event) {
    switch (event.GetEventType()) {
        case EventType::WindowClose:
            isRunning = false; break;
        default:
            break;
    }
    event.SetHandled();
}

void application::game_start() {
    Time::GameStart();
    while(isRunning) {
        for(auto& layer : layers)
            layer->OnUpdate();
        window->OnUpdate();
    }
    delete window;
}
