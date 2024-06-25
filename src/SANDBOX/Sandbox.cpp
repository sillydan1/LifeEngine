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
#include "Sandbox.h"
#include "SandboxLayer.h"

sandbox::sandbox() : application() {
    window->SetVSync(true);
}

void sandbox::game_start() {
    push_layer<sandbox_layer>();
    application::game_start();
}

void sandbox::handle_application_event(event &event) {
    if(event.get_event_type() == event_type::WindowResize)
        glViewport(0,0,window->GetWidth(),window->GetHeight());

    application::handle_application_event(event);
}
